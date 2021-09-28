/*  BSP. PWM launcher (on/off controller).
 *
 *  Three pins are used to form a command to PWM:
 *  - Falling edge of signal on stop pin stops pwm
 *  - Falling edge of signal on start pin start pwm
 *  - Rising edge of signal on start_finite pin start pwm for a finite time
 */

#ifndef BSP_PWM_LAUNCHER_H
#define BSP_PWM_LAUNCHER_H

#include "bsp_hal.h"

namespace bsp {
namespace pwm_launcher {

//---------------------------------------------------------- Used resources ---

static GPIO_TypeDef* stop_port_         [[gnu::unused]] = GPIOA;
constexpr uint32_t stop_pin_                            = GPIO_PIN_9;

static GPIO_TypeDef* start_port_        [[gnu::unused]] = GPIOA;
constexpr uint16_t start_pin_                           = GPIO_PIN_6;

static GPIO_TypeDef* start_finite_port_ [[gnu::unused]] = GPIOA;
constexpr uint16_t start_finite_pin_                    = GPIO_PIN_8;

/** Handle an event on the stop pin.
 */

void stop_pin_irq_callback();

/** Handle an event on the start pin.
 */

void start_pin_irq_callback();

/** Handle an event on the start_finite pin.
 */

void start_finite_pin_irq_callback();

//-----------------------------------------------------------------------------

/** Pwm Launcher commands
 */
enum Command {
    stop,               //!< Stop pwm
    start,              //!< Start pwm
    start_finite        //!< Start pwm for a 5 seconds
};

/** Initialize PWM launcher.
 *
 *  Initialize input pins and enable interrupts. After initialization the
 *  launcher command is STOP_PWM.
 */
void init();

/** Check has been state of inputs pins changed or no.
 *
 *  When state has been changed the new command is formed according with events
 *  priority (from highest to lowest): 
 *  - #Command::stop - when signal at stop input falling
 *  - #Command::start - when signal at start input falling
 *  - #Command::start_finite - when signal at start_finite input rising
 *
 *  @return true if state has been changed, false otherwise.
 */

bool has_command_accepted();

/** Return last launcher command.
 */

Command get_command();

} // namespace pwm_launcher
} // namespace bsp

#endif /* BSP_PWM_LAUNCHER_H */
