/**
 *  Test for Spectron company.
 *
 *  (c) Stan, 2021
 */

#include "main.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* Board support package - drivers, which are hardware related.
 */
#include "bsp.h"

/* Addition tasks (simulation, debug...)
 */
#include "bsp_lpuart.h"

#include "pwm_launcher_inputs_simulator.h"
#include "temp_range_indicator_test.h"

void default_task(void* pvParameters);
void handle_temp_task(void* pvParameters);
void control_pwm_task(void* pvParameters);
void measure_temp_task(void* pvParameters);

/**
 *  Common queue between task where temperature is being measured and
 *  task where temperature is handled.
 */
xQueueHandle temp_sensor_queue_;

/*
 *  Each task periodically increments their counter,
 *  in default task we check that counters are not frozen.
 */
volatile int16_t wdg_counter_measure_temp_task_ = 0;
volatile int16_t wdg_counter_handle_temp_task_  = 0;
volatile int16_t wdg_counter_control_pwm_task_  = 0;

using namespace bsp;

int main(void) {
    // Init system and bsp devices.
    bsp::init();
    adc::init();
    watchdog::init();
    lpuart::init();

    // Create the queue(s)
    temp_sensor_queue_ = xQueueCreate(10, sizeof(int));

    // Create the threads
    xTaskCreate(default_task,      "Watchdog handle",  1024, NULL, 1, NULL);
    xTaskCreate(handle_temp_task,  "Temp handle",      1024, NULL, 2, NULL);
    xTaskCreate(control_pwm_task,  "Pwm control",      1024, NULL, 2, NULL);
    xTaskCreate(measure_temp_task, "Temp measure",     1024, NULL, 2, NULL);

    // Uncomment to simulate pwm launcher inputs.
//    pwm_launcher_inputs_simulator::init();

    // Uncomment to test temp range indicator.
//    temp_range_indicator_test::init();

    /* Start scheduler */
    vTaskStartScheduler();

    /* We should never get here as control is now taken by the scheduler */
    /* Infinite loop */
    while (1) {
    }
}

/** Here we collected watchdog data from others tasks, and if they are ok, reset
 *  hardware watchdog.
 */

void default_task(void* pvParameters) {
    (void)pvParameters;
    static int prev_wdg_1 = wdg_counter_measure_temp_task_;
    static int prev_wdg_2 = wdg_counter_handle_temp_task_;
    static int prev_wdg_3 = wdg_counter_control_pwm_task_;

    for(;;) {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
        //  Check that all tasks are alive. If so - refresh system watchdog.
        if (   prev_wdg_1 != wdg_counter_measure_temp_task_
            && prev_wdg_2 != wdg_counter_handle_temp_task_
            && prev_wdg_3 != wdg_counter_control_pwm_task_) {

            prev_wdg_1 = wdg_counter_measure_temp_task_;
            prev_wdg_2 = wdg_counter_handle_temp_task_;
            prev_wdg_3 = wdg_counter_control_pwm_task_;
            watchdog::refresh();
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

/**
 *  Measure temperature sensor and send gotten value to the common queue
 *  (temp_sensor_queue_). The temperature value is in Celsius.
 */

void measure_temp_task(void* pvParameters) {
    (void)pvParameters;
    const int delay_ms = pdMS_TO_TICKS(500);
    float celsius;

    while(1) {
        celsius = adc::get_chip_temp();
        log("[MEASURE_TEMP] Measure %.1f C\r\n", celsius);
        xQueueSend(temp_sensor_queue_, &celsius, portMAX_DELAY);
        wdg_counter_measure_temp_task_++;
        vTaskDelay(delay_ms);
    }
}

/**
 *  1) Take temperature sensor value from the common queue,
 *  2) Update output pin state according gotten temperature and [min_c, max_c]
 *     range (see #temp_range_indicator:update() for details)
 *  3) Send gotten temperature to uart (see #uart::send() for details)
 */
void handle_temp_task(void* pvParameters) {
    (void)pvParameters;
    const float min_c = 10; // celsius
    const float max_c = 50; // celsius
    float celsius = 0.;

    temp_range_indicator::init(min_c, max_c);
    uart::init();
    while (1) {
        if (xQueueReceive(temp_sensor_queue_, &celsius, portMAX_DELAY) == pdTRUE) {
            while (xQueueReceive(temp_sensor_queue_, &celsius, 0)) {
                // Just get most fresh value of the temperature (if any)
            }
            log("[HANDLE_TEMP] Gotten %.1f C\r\n", celsius);

            xQueueReset(temp_sensor_queue_);
            temp_range_indicator::update(celsius);
            uart::send(celsius);
            wdg_counter_handle_temp_task_++;
        }
    }
}

/**
 *  1) Wait until state on pins {stop, start, start_finite} is changed (see
 *  #pwm_launcher::has_command_accepted())

 *  2) Control on/off pwm according gotten state (see pwm:set_mode()):
 *     - Stop falling - stop pwm,
 *     - Start falling - start pwm
 *     - Start finite rising - start pwm for a 5 seconds.
 */

void control_pwm_task(void* pvParameters) {
    (void)pvParameters;
    const int delay_ms = pdMS_TO_TICKS(10);
    const int enable_time_ms = 5000;

    pwm_launcher::init();
    pwm::init(enable_time_ms);
    while (1) {
        if (pwm_launcher::has_command_accepted()) {
            pwm::set_mode(pwm_launcher::get_command());
        }
        wdg_counter_control_pwm_task_++;
        vTaskDelay(delay_ms);
    }
}
