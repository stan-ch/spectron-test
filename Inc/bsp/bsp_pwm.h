/*  BSP PWM generator.
 *
 *  One channel PWM generator which is managed by #bsp::pwm_launcher:
 *  - pwm::launcher::Command::start - starts PWM
 *  - pwm::launcher::Command::start_finite - starts PWM for a specified time,
 *    then stop it. Time is specified as parameter in #init().
 *  - pwm::launcher::Command::stop - stops PWM
 *
 *  Signal is out via #out_pin_ pin.
 *
 *  Additional timer (finit_timer_) is used to calculate period between
 *  start_finite command and pwm off..
 */

#ifndef BSP_PWM_H
#define BSP_PWM_H

#include "bsp_pwm_launcher.h"

namespace bsp {
namespace pwm {

//---------------------------------------------------------- Used resources ---

/*  Pwm timer (channel). Used to get PWM output signal.
 */

static TIM_TypeDef* timer_      [[gnu::unused]] = TIM2;
constexpr uint32_t timer_channel_               = TIM_CHANNEL_1;

/*  Pwm output pin.
 */

constexpr uint32_t out_pin_                     = GPIO_PIN_0;
static GPIO_TypeDef* out_port_  [[gnu::unused]] = GPIOA;
constexpr uint8_t out_pin_af_                   = GPIO_AF1_TIM2;

/*  The finite timer (channel) is one shot timer to get time period after
 *  start_finite command has been accepted. 
 *
 *  The callback function must be called from proper interrupt handler.
 */

static TIM_TypeDef* finite_timer_ [[gnu::unused]] = TIM3;
constexpr uint32_t finite_timer_channel_          = TIM_CHANNEL_1;
void finite_timer_irq_callback();

//------------------------------------------------------------------ Params ---

/** Constant PWM period, ms.
 */
constexpr uint16_t pwm_period_ms_                 = 1000;

/** Constant Pwm ontime, ms.
 */
constexpr uint16_t pwm_ontime_ms_                 = 300;

//-----------------------------------------------------------------------------

/** Initialize PWM.
 *
 *  Init pwm channel and its output. \a finite_time_ms specifies time to auto
 *  off PWM (when it is was started with #start_finite command). After
 *  initialixation the pwm output is low.
 */

void init(uint16_t finite_time_ms);

/** Set PWM mode according with \a command from #pwm_launcher::Command.
 */

void set_mode(pwm_launcher::Command command);

} // namespace pwm
} // namespace bsp

#endif /* BSP_PWM_H */
