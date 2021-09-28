/*  BSP PWM STM32 HAL implementation.
 *
 */


#include "bsp_system.h"
#include "bsp_gpio.h"
#include "bsp_pwm.h"
#include "bsp_lpuart.h"

namespace bsp {
namespace pwm {

static TIM_HandleTypeDef h;        // Pwm timer
static TIM_HandleTypeDef h_finite; // One shot Pwm enable timer

/** Calculate prescaler value to suit 1Mhz clock to timer.
 */
static inline uint32_t get_prescaler_1Mhz() {
    uint32_t prescaler = SystemCoreClock / 1'000'000U;
    return prescaler < 2 ? 1 : prescaler - 1;
}

/** Generate update and then clear update to avoid interrupt that occurs
 *  immediately after start of a timer.
 */

static inline void
generate_then_clear_update(TIM_TypeDef* timer) {
	timer->EGR |= TIM_EGR_UG;
	timer->SR &= TIM_SR_UIF;
}

/** Configure finite timer to count finite_time_ms and then off.
 */

static bool init_finite_timer(uint16_t finite_time_ms) {
    bool success = true;

    enable_timer_clk(finite_timer_);

    h_finite.Instance           = finite_timer_;
    h_finite.Init.Prescaler     = get_prescaler_1Mhz();
    h_finite.Init.CounterMode   = TIM_COUNTERMODE_UP;
    h_finite.Init.Period        = finite_time_ms + 100;
    h_finite.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    h.Init.AutoReloadPreload    = TIM_AUTORELOAD_PRELOAD_DISABLE;
    success = success && HAL_TIM_OC_Init(&h_finite) == HAL_OK;

    TIM_MasterConfigTypeDef m = {0};
    m.MasterOutputTrigger  = TIM_TRGO_RESET;
    m.MasterOutputTrigger2 = TIM_TRGO2_RESET;
    m.MasterSlaveMode      = TIM_MASTERSLAVEMODE_DISABLE;
    success = success && HAL_TIMEx_MasterConfigSynchronization(&h_finite, &m) == HAL_OK;
      
    TIM_OC_InitTypeDef oc = {0};
    oc.OCMode       = TIM_OCMODE_TOGGLE;
    oc.Pulse        = finite_time_ms-1;
    oc.OCPolarity   = TIM_OCPOLARITY_LOW;
    oc.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
    oc.OCFastMode   = TIM_OCFAST_DISABLE;
    oc.OCIdleState  = TIM_OCIDLESTATE_RESET;
    oc.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    success = success && HAL_TIM_OC_ConfigChannel(&h_finite, &oc, timer_channel_) == HAL_OK;

    generate_then_clear_update(finite_timer_);
    return success;
}

static bool start_finite_timer() {
    generate_then_clear_update(finite_timer_);
    return HAL_TIM_OC_Start_IT(&h_finite, finite_timer_channel_) == HAL_OK;
}

static bool stop_finite_timer() {
    return HAL_TIM_OC_Stop(&h_finite, finite_timer_channel_) == HAL_OK;
}


/** Configured PWM to has 1us clock, #pwm_period_ms_ period, and #pwm_ontime_ms_
 *  on time.
 */

static bool init_pwm_timer() {
    bool success = true;
    enable_timer_clk(timer_);

    h.Instance               = timer_;
    h.Init.Prescaler         = get_prescaler_1Mhz();
    h.Init.CounterMode       = TIM_COUNTERMODE_UP;
    h.Init.Period            = pwm_period_ms_-1;
    h.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    h.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    success = success && HAL_TIM_Base_Init(&h) == HAL_OK;

    TIM_MasterConfigTypeDef m = {0};
    m.MasterOutputTrigger  = TIM_TRGO_RESET;
    m.MasterOutputTrigger2 = TIM_TRGO2_RESET;
    m.MasterSlaveMode      = TIM_MASTERSLAVEMODE_DISABLE;
    success = success && HAL_TIMEx_MasterConfigSynchronization(&h, &m) == HAL_OK;
      
    TIM_OC_InitTypeDef oc = {0};
    oc.OCMode       = TIM_OCMODE_PWM1;
    oc.Pulse        = pwm_ontime_ms_ - 1;
    oc.OCPolarity   = TIM_OCPOLARITY_HIGH;
    oc.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
    oc.OCFastMode   = TIM_OCFAST_DISABLE;
    oc.OCIdleState  = TIM_OCIDLESTATE_RESET;
    oc.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    success = success && HAL_TIM_PWM_ConfigChannel(&h, &oc, timer_channel_) == HAL_OK;
    return success;
}

/** Configured output to be low, when PWM is off.
 */

static inline void init_output_pin() {
    enable_gpio_clk(out_port_);
    gpio::init_alternate(out_port_, out_pin_, GPIO_MODE_AF_PP, out_pin_af_, GPIO_PULLDOWN, GPIO_SPEED_FREQ_VERY_HIGH);
}    

void init(uint16_t finite_time_ms) {
    init_finite_timer(finite_time_ms);
    init_pwm_timer();
    init_output_pin();
};

void set_mode(bsp::pwm_launcher::Command command) {
    using namespace bsp::pwm_launcher;
    switch (command) {
    case Command::stop :
        stop_finite_timer();
        HAL_TIM_PWM_Stop(&h, timer_channel_);
        log("[PWM] Stopped\r\n");
        break;
    case Command::start :
        stop_finite_timer();
        HAL_TIM_PWM_Start(&h, timer_channel_);
        log("[PWM] Started\r\n");
        break;
    case Command::start_finite :
        stop_finite_timer();
        HAL_TIM_PWM_Start(&h, timer_channel_);
        start_finite_timer();
        log("[PWM] Started finite\r\n");
        break;
    }
}

void finite_timer_irq_callback() {
    HAL_TIM_IRQHandler(&pwm::h_finite);
    pwm::set_mode(pwm_launcher::Command::stop);
}

} // namespace pwm
} // namspace bsp

