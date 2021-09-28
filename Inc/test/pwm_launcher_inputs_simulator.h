/**
 *  Simulate of pwm launcher inputs.
 *
 *  Feed control PWM pins with test sequence and check work of bsp::pwm_launcher
 *  and bsp::pwm.
 *  
 *  This task has been tested with Nucleo-G747RE with follow wire connections:
 *    - PA6 <- PC0 - for start pin path
 *    - PA8 <- PC1 - for start_finite pin path
 *    - PA9 <- PC2 - for stop pin path
 */

#ifndef PWM_LAUNCHER_INPUT_SIMULATOR_H
#define PWM_LAUNCHER_INPUT_SIMULATOR_H

#include "bsp_hal.h"

namespace bsp {
namespace pwm_launcher_inputs_simulator {

//---------------------------------------------------------- Used resources ---

// Note: all pins are from the one port!

static GPIO_TypeDef* port_      [[gnu::unused]] = GPIOC;
constexpr uint32_t start_pin_                   = GPIO_PIN_0;
constexpr uint32_t start_finite_pin_            = GPIO_PIN_1;
constexpr uint32_t stop_pin_                    = GPIO_PIN_2;
constexpr uint32_t all_pins_                    = start_pin_ | start_finite_pin_ | stop_pin_;

//-----------------------------------------------------------------------------

void init();
void simulate(void* pvParameters);

} // namespace pwm_launcher_inputs_simulator
} // namespace bsp

#endif // TEST_RANGE_INDICATOR_TEST_H
