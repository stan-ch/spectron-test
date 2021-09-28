/*  BSP PWM Launcher. STM32 HAL implementation.
 */

#include "bsp_system.h"
#include "bsp_hal.h"
#include "bsp_gpio.h"
#include "bsp_pwm_launcher.h"

#include <bsp_lpuart.h>

namespace bsp {
namespace pwm_launcher {

/*  Pin events. Updated in IRQ.
 */

static volatile int stop_event_;         //!< Set when #stop_pin rised
static volatile int start_event_;        //!< Set when #start_pin falled
static volatile int start_finite_event_; //!< Set when #start_finite_pin falled

static Command command_;                 //!< Last command suitable last event.


/* Helper for init event and pins.
 */
static inline void init_event_and_pin(volatile int& pin_event, GPIO_TypeDef* port, uint32_t pin, uint32_t mode) {
    enable_gpio_clk(port);
    gpio::init(port, pin, mode);
    enable_exti_irq(pin);
    pin_event = false;
}

/*  Init pins as inputs, enable IRQ to get rising/falling events.
 */
void init() {
    init_event_and_pin(stop_event_,         stop_port_,         stop_pin_,         GPIO_MODE_IT_FALLING);
    init_event_and_pin(start_event_,        start_port_,        start_pin_,        GPIO_MODE_IT_FALLING);
    init_event_and_pin(start_finite_event_, start_finite_port_, start_finite_pin_, GPIO_MODE_IT_RISING);
    command_ = Command::stop;
}

bool has_command_accepted() {
    Command prev_command = command_;
    if (stop_event_) {
        command_ = Command::stop;
        stop_event_ = false;
        start_event_ = false;
        start_finite_event_ = false;
        log("[LAUNCHER] Detect STOP command\r\n");
    } else if (start_event_) {
        command_ = Command::start;
        start_event_ = false;
        start_finite_event_ = false;
        log("[LAUNCHER] Detect START command\r\n");
    } else if (start_finite_event_) {
        command_ = Command::start_finite;
        start_finite_event_ = false;
        log("[LAUNCHER] Detect START_INFINITE command\r\n");
    }
    return (prev_command != command_);
}

/** Returns last command
 */
Command get_command() {
    return command_;
}

/** This callback must be called from interrupt hadler on falling edge event for
 *  proper pin.
 */
void stop_pin_irq_callback() {
    bsp::pwm_launcher::stop_event_ = true;
}

/** This callback must be called from interrupt hadler on falling edge event for
 *  proper pin.
 */
void start_pin_irq_callback() {
    bsp::pwm_launcher::start_event_ = true;
}

/** This callback must be called from interrupt hadler on rising edge event for proper pin.
 */
void start_finite_pin_irq_callback() {
    bsp::pwm_launcher::start_finite_event_ = true;
}

} // namespace pwm_launcher
} // namespace bsp
