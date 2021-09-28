/**  Interrupt handlers
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32g4xx_it.h"
#include "bsp.h"
#ifdef DEBUG_NUCLEO64_STM32G474
#include "bsp_button.h"
#endif

extern TIM_HandleTypeDef htim15;

/******************************************************************************/
/* Cortex-M4 Processor Interruption and Exception Handlers                    */
/******************************************************************************/

/** This function handles Non maskable interrupt.
  */
extern "C" {
void NMI_Handler(void) {
    __HAL_RCC_CLEAR_IT(RCC_IT_CSS);

    bsp::init_system_with_hsi();
}
}

/** This function handles Hard fault interrupt.
  */
extern "C" {
void HardFault_Handler(void) {
    while (1) {
    }
}
}

/** This function handles Memory management fault.
  */
extern "C" {
void MemManage_Handler(void) {
    while (1) {
    }
}
}

/** This function handles Prefetch fault, memory access fault.
  */
extern "C" {
void BusFault_Handler(void) {
    while (1) {
    }
}
}

/** This function handles Undefined instruction or illegal state.
  */
extern "C" {
void UsageFault_Handler(void) {
  while (1) {
  }
}
}

/** This function handles Debug monitor.
  */
extern "C" {
void DebugMon_Handler(void) {
}
}

/******************************************************************************/
/* STM32G4xx Peripheral Interrupt Handlers                                    */
/******************************************************************************/

/** Handlers for TIM1 break interrupt and TIM15 global interrupt.
  */
extern "C" {
void TIM1_BRK_TIM15_IRQHandler(void) {
    HAL_TIM_IRQHandler(&htim15);
}
}

/** Handler for TIM3 interruprt and ...
  */

extern "C" {
void TIM3_IRQHandler(void) {
    using namespace bsp;
    pwm::finite_timer_irq_callback();
}
}

/** Helper for processing EXTI[15:10] callback.
 *
 *  Is irq flag for pin \a pin is set - clean it and run \a callback function.
 *  Use assert to check that pin and exti_handler suit each other.
 */
static void inline exti15_10_callback(uint32_t exti_line, void (*callback)()) {
    assert_param(exti_line == GPIO_PIN_10
    		|| exti_line == GPIO_PIN_11
    		|| exti_line == GPIO_PIN_12
    		|| exti_line == GPIO_PIN_13
    		|| exti_line == GPIO_PIN_14
			|| exti_line == GPIO_PIN_15);
    if (EXTI->PR1 & exti_line) {
        EXTI->PR1 = exti_line;
        callback();
    }
}

/**  This function handles EXTI line[15:10] interrupts.
  *
  *  This handler isn't compatible with HAL rules. The function call path
  *    "EXTI15_10_IRQHandler -> HAL_GPIO_EXTI_IRQHandler -> HAL_GPIO_EXTI_Callback -> callback"
  *  is changed with
  *    "EXTI15_10_IRQHandler -> bsp::exti15_10_callback -> callback"
  */
extern "C" {
void EXTI15_10_IRQHandler(void) {
#ifdef DEBUG_NUCLEO64_STM32G474
    using namespace bsp;
    exti15_10_callback(button::pin_, button::irq_callback);
#endif    
}
}

/** Helper for processing an EXTI[9:5] handler.
 *
 *  Is irq flag for pin \a pin is set - clean it and run \a callback function.
 *  Use assert to check that pin and exti_handler suit each other.
 */

static void inline exti9_5_callback(uint32_t exti_line, void (*callback)()) {
    assert_param(exti_line == GPIO_PIN_5
    		|| exti_line == GPIO_PIN_6
    		|| exti_line == GPIO_PIN_7
    		|| exti_line == GPIO_PIN_8
			|| exti_line == GPIO_PIN_9);
    if (EXTI->PR1 & exti_line) {
        EXTI->PR1 = exti_line;
        callback();
    }
}


/**  This function handles EXTI line[9:5] interrupts.
  *
  *  This handler isn't compatible with HAL rules (see comments for
  *  #EXTI_15_10_IRQHandler).
  */
extern "C" {
void EXTI9_5_IRQHandler(void) {
    using namespace bsp;
    exti9_5_callback(pwm_launcher::stop_pin_,  pwm_launcher::stop_pin_irq_callback);
    exti9_5_callback(pwm_launcher::start_pin_, pwm_launcher::start_pin_irq_callback);
    exti9_5_callback(pwm_launcher::start_finite_pin_, pwm_launcher::start_finite_pin_irq_callback);
}
}
