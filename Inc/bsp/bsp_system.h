/** BSP system related functions.
 */

#ifndef BSP_SYSTEM_H
#define BSP_SYSTEM_H

#include "bsp_hal.h"

namespace bsp {

/** System configuration (clocks, bus, peripherals).
 */
void init(void);

/** Init system with hsi source clock.
 */
bool init_system_with_hsi();

/** Enable clock for specified GPIO port.
 */

static inline void enable_gpio_clk(GPIO_TypeDef* port) {
    if (port == GPIOA) {
        __HAL_RCC_GPIOA_CLK_ENABLE();
    } else if (port == GPIOB) {
        __HAL_RCC_GPIOB_CLK_ENABLE();
    } else if (port == GPIOC) {
        __HAL_RCC_GPIOC_CLK_ENABLE();
    } else if (port == GPIOD) {
        __HAL_RCC_GPIOD_CLK_ENABLE();
    } else if (port == GPIOE) {
        __HAL_RCC_GPIOE_CLK_ENABLE();
    } else if (port == GPIOF) {
        __HAL_RCC_GPIOF_CLK_ENABLE();
    } else {
        assert_param(false);
    }
}

/** Enable clock for ADC modules.
 */

static inline void enable_adc_clk(ADC_TypeDef* adc) {
    if (adc == ADC1 || adc == ADC2) {
        __HAL_RCC_ADC12_CLK_ENABLE();
    } else if (adc == ADC3 || adc == ADC4 || adc == ADC5) {
        __HAL_RCC_ADC345_CLK_ENABLE();
    } else if (false) {
        assert_param(false);
    }
}

/** Enable clock for timers.
 *
 *  NOT ALL STM32 TIMER INSTANCES ARE IMPLEMENTED!
 */
static inline void enable_timer_clk(TIM_TypeDef* timer) {
    if (timer == TIM3) {
        __HAL_RCC_TIM3_CLK_ENABLE();
    } else if (timer == TIM2) {
        __HAL_RCC_TIM2_CLK_ENABLE();
    } else if (timer == TIM7) {
        __HAL_RCC_TIM7_CLK_ENABLE();
    } else {
        assert_param(false);
    }
}

/** Enable clock for USART.
 */

static inline void enable_usart_clk(USART_TypeDef* usart) {
    if (usart == LPUART1) {
        __HAL_RCC_LPUART1_CLK_ENABLE();
    } else if (usart == UART4) {
        __HAL_RCC_UART4_CLK_ENABLE();
    } else {
        assert_param(false);
    }
}

//  IRQ related section

/** Enable exti for specified pin \a pin.
 *
 *  There is no check is interrupt is already enabled. Just enable in any case.
 */
    
inline void enable_exti_irq(uint32_t pin) {
    uint32_t default_preempt_priority = 5;
    uint32_t default_sub_priority = 0;
        
    if (pin == GPIO_PIN_0) {
        HAL_NVIC_SetPriority(EXTI0_IRQn, default_preempt_priority, default_sub_priority);
        HAL_NVIC_EnableIRQ(EXTI0_IRQn);

    } else if (pin == GPIO_PIN_1) {
        HAL_NVIC_SetPriority(EXTI1_IRQn, default_preempt_priority, default_sub_priority);
        HAL_NVIC_EnableIRQ(EXTI1_IRQn);

    } else if (pin == GPIO_PIN_2) {
        HAL_NVIC_SetPriority(EXTI2_IRQn, default_preempt_priority, default_sub_priority);
        HAL_NVIC_EnableIRQ(EXTI2_IRQn);

    } else if (pin == GPIO_PIN_3) {
        HAL_NVIC_SetPriority(EXTI3_IRQn, default_preempt_priority, default_sub_priority);
        HAL_NVIC_EnableIRQ(EXTI3_IRQn);

    } else if (pin == GPIO_PIN_4) {
        HAL_NVIC_SetPriority(EXTI4_IRQn, default_preempt_priority, default_sub_priority);
        HAL_NVIC_EnableIRQ(EXTI4_IRQn);

    } else if (pin == GPIO_PIN_5
               || pin == GPIO_PIN_6
               || pin == GPIO_PIN_7
               || pin == GPIO_PIN_8
               || pin == GPIO_PIN_9) {
        HAL_NVIC_SetPriority(EXTI9_5_IRQn, default_preempt_priority, default_sub_priority);
        HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

    } else if (pin == GPIO_PIN_10
               || pin == GPIO_PIN_11
               || pin == GPIO_PIN_12
               || pin == GPIO_PIN_13
               || pin == GPIO_PIN_14
               || pin == GPIO_PIN_15) {
        HAL_NVIC_SetPriority(EXTI15_10_IRQn, default_preempt_priority, default_sub_priority);
        HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

    } else {
        assert_param(false);
    }
}

/** Enable interupt for timers.
 */

inline void enable_timer_irq(TIM_TypeDef* timer) {
    uint32_t default_preempt_priority = 5;
    uint32_t default_sub_priority = 0;
    if (timer == TIM2) {
        HAL_NVIC_SetPriority(TIM2_IRQn, default_preempt_priority, default_sub_priority);
        HAL_NVIC_EnableIRQ(TIM2_IRQn);
    } else if (timer == TIM3) {
        HAL_NVIC_SetPriority(TIM3_IRQn, default_preempt_priority, default_sub_priority);
        HAL_NVIC_EnableIRQ(TIM3_IRQn);
    } else if (timer == TIM7) {
        HAL_NVIC_SetPriority(TIM7_DAC_IRQn, default_preempt_priority, default_sub_priority);
        HAL_NVIC_EnableIRQ(TIM7_DAC_IRQn);
    } else {
        assert_param(false);
    }
}

} // namespace bsp


/** Send message to terminal (by means of LPUART)
 */

static inline void log(const char* format, ...) {

#ifdef LOG_ENABLE
    static char message_buf[128];
    static char buf[256];
    static uint32_t prev_time = 0;
    
    va_list args;
    va_start(args, format);
    vsnprintf(message_buf, sizeof(message_buf), format, args);

    uint32_t now = HAL_GetTick();
    uint16_t diff_time_ms = static_cast<uint16_t>(now - prev_time);
    prev_time = now;
    uint16_t diff_time_ms_part = diff_time_ms % 1000;
    uint16_t diff_time_s_part  = (diff_time_ms - diff_time_ms_part) / 1000;
    
    snprintf(buf, sizeof(buf), "[%2u,%03u] %s", diff_time_s_part, diff_time_ms_part, message_buf);
    bsp::lpuart::send(buf);
    va_end(args);
#endif
} 


#endif
