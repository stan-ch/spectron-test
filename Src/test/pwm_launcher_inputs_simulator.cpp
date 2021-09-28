/**
 *  Simulate for pwm launcher.
 *
 *  Feed control PWM pins (need external connection).
 */


#include <FreeRTOS.h>
#include <task.h>
#include <string.h>
#include "bsp_gpio.h"
#include "bsp_system.h"
#include "bsp_lpuart.h"
#include "pwm_launcher_inputs_simulator.h"


namespace bsp {
namespace pwm_launcher_inputs_simulator {

static void init_pins() {
    enable_gpio_clk(port_);
    gpio::reset(port_, all_pins_);
    gpio::init(port_, all_pins_, GPIO_MODE_OUTPUT_PP);
}

void init() {
    init_pins();
    xTaskCreate(simulate, "Simulate launcher", 1024, NULL, 2, NULL);
}

static void log_pin(GPIO_TypeDef* port, uint16_t pin, bool set, const char* pin_name, uint16_t timeout_ms) {
    set ? gpio::set(port, pin) : gpio::reset(port, pin);
    log("[SUMULATE] %s has been %s\r\n", pin_name, set ? "risen" : "fallen");
    vTaskDelay(pdMS_TO_TICKS(timeout_ms));
}

static void start_pin(bool set, uint16_t timeout_ms) {
    log_pin(port_, start_pin_, set, "START", timeout_ms);
};

static void start_finite_pin(bool set, uint16_t timeout_ms) {
    log_pin(port_, start_finite_pin_, set, "START_FINITE", timeout_ms);
};

static void stop_pin(bool set, uint16_t timeout_ms) {
    log_pin(port_, stop_pin_, set, "STOP", timeout_ms);
};

void simulate(void* pvParameters) {
    log("[SUMULATE] Init state : all pins zero\n\r");
    start_pin(0, 0);
    start_finite_pin(0, 0);
    stop_pin(0, 0);
    
    while (1) {
        log("[SUMULATE] Start cycle\n\r");
       
        start_pin(1, 1000);
        start_pin(0, 1000);

        start_finite_pin(1, 1000);
        start_finite_pin(0, 8000);

        start_pin(1, 1000);
        start_pin(0, 7000);
        stop_pin(1, 1000);
        stop_pin(0, 1000);
    }
}

} // namespace pwm_launcher_inputs_simulator
} // namespace bsp

