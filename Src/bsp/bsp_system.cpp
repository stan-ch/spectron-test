/** System core and bsp initialization.
 *
 *  System clock source : PLL (HSE) || PLL (HSI)
 *  SYSCLK(Hz)          : 170Mhz
 *  HSI                 : 16MHz
 *  HSE                 : 24MHz
 */

#include "bsp_hal.h"

namespace bsp {

/** Configure the main internal voltage regulator.
 */

static inline bool config_internal_voltage_regulator() {
    HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);
    return true;
}

/** Initialize SCC oscillators and PLL with HSE input.
 */

static inline bool init_rcc_oscillators_hse() {
    bool success = true;
    /* Select HSI as system clock source to allow modification of the PLL
     * configuration */
    RCC_ClkInitTypeDef cs = {0};
    cs.ClockType    = RCC_CLOCKTYPE_SYSCLK;
    cs.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;    
    success = success && HAL_RCC_ClockConfig(&cs, FLASH_LATENCY_1) == HAL_OK;
      
    RCC_OscInitTypeDef s = {0};
    s.OscillatorType      = RCC_OSCILLATORTYPE_HSE;
    s.HSEState            = RCC_HSE_ON;
    s.PLL.PLLState        = RCC_PLL_ON;
    s.PLL.PLLSource       = RCC_PLLSOURCE_HSE;
    s.PLL.PLLM            = RCC_PLLM_DIV6;
    s.PLL.PLLN            = 85;
    s.PLL.PLLP            = RCC_PLLP_DIV2;
    s.PLL.PLLQ            = RCC_PLLQ_DIV2;
    s.PLL.PLLR            = RCC_PLLR_DIV2;
    success = success && HAL_RCC_OscConfig(&s) == HAL_OK;
    return success;
}

/** Init SCC oscillators and PLL with HSI input.
 *
 *  Firstly, select HSI as system clock source to allow modification of the PLL
 *  configuration.
 */

static inline bool init_rcc_oscillators_hsi() {

    RCC_ClkInitTypeDef cs = {0};
    cs.ClockType    = RCC_CLOCKTYPE_SYSCLK;
    cs.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;    
    HAL_RCC_ClockConfig(&cs, FLASH_LATENCY_1);

    RCC_OscInitTypeDef s = {0};
    s.OscillatorType      = RCC_OSCILLATORTYPE_HSI;
    s.HSIState            = RCC_HSI_ON;
    s.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    s.PLL.PLLState        = RCC_PLL_ON;
    s.PLL.PLLSource       = RCC_PLLSOURCE_HSI;
    s.PLL.PLLM            = RCC_PLLM_DIV4;
    s.PLL.PLLN            = 85;
    s.PLL.PLLP            = RCC_PLLP_DIV2;
    s.PLL.PLLQ            = RCC_PLLQ_DIV2;
    s.PLL.PLLR            = RCC_PLLR_DIV2;

    return HAL_RCC_OscConfig(&s) == HAL_OK;
}

/* Init buses clocks (CPU, AHB, APB).
 */
static inline bool init_cpu_ahb_apb_clocks() {
    RCC_ClkInitTypeDef s = {0};
    s.ClockType      = (RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK 
                        | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    s.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    s.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    s.APB1CLKDivider = RCC_HCLK_DIV1;
    s.APB2CLKDivider = RCC_HCLK_DIV1;
    return HAL_RCC_ClockConfig(&s, FLASH_LATENCY_4) == HAL_OK;
}

static inline bool init_peripherals_clocks() {
    RCC_PeriphCLKInitTypeDef s = {0};
    s.PeriphClockSelection   = (RCC_PERIPHCLK_UART4 | RCC_PERIPHCLK_LPUART1 | RCC_PERIPHCLK_ADC12);
    s.Uart4ClockSelection    = RCC_UART4CLKSOURCE_PCLK1;
    s.Lpuart1ClockSelection  = RCC_LPUART1CLKSOURCE_PCLK1;
    s.Adc12ClockSelection    = RCC_ADC12CLKSOURCE_SYSCLK;
    return HAL_RCCEx_PeriphCLKConfig(&s) == HAL_OK;
}

/** Initialize system with HSE source.
 *
 *  @return true if success, false if system failure occur.
 */

static inline bool init_system_with_hse() {
    bool success = true;
    success = success && init_rcc_oscillators_hse();
    success = success && init_cpu_ahb_apb_clocks();
    success = success && init_peripherals_clocks();

    for (uint32_t i = 0; i < 10000L && !HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSERDY); i++) { }
    return HAL_IS_BIT_SET(RCC->CR, RCC_CR_HSERDY);
}

static inline bool disable_hse() {
    RCC_OscInitTypeDef s = {0};
    s.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    s.HSEState       = RCC_HSE_OFF;
    s.PLL.PLLState   = RCC_PLL_NONE;
    return HAL_RCC_OscConfig(&s) == HAL_OK;
}

/** Initialize system with HSI source.
 *
 *  When work from HSI there is no need in HSE so it is disabled.
 *
 *  @return true - success, false - error occured
 */

bool init_system_with_hsi() {
    bool success = true;
    success = success && init_rcc_oscillators_hsi();
    success = success && init_cpu_ahb_apb_clocks();
    success = success && init_peripherals_clocks();
    success = success && disable_hse();
    return success;
}

/** Init HAL, clocks, peripherals.
 *
 *  System starts with HSI clock source, this function must be called to setup
 *  system and clocks:

 *    1) Initialize HAL, enable the APB2 peripheral and power clocks, config
 *       PendSV_IRQn.
 *    2) Config voltage regulator.
 *    3) Enable Clock Security System (CSS) - to automaticaly switch the system
 *       clock to the HSI oscillator if a failure with HSE is detected. In this
 *       case also the NMI will be generated - don't forget to clear CSSC bit in
 *       RCC_CIR.
 *    4) Try start PLL with HSE oscillator. 
 *    5) If HSE oscillator didn't start, disable PLL, returns to HSE and
 *       initialize system with HSE source.
 *
 *  @return true if init procedure has been executed without errors, 
 *          false otherwise.
 */
bool init() {
    bool success = true;

    HAL_Init();
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();
    HAL_NVIC_SetPriority(PendSV_IRQn, 15, 0);

    config_internal_voltage_regulator();

    HAL_RCC_EnableCSS();

    if (!init_system_with_hse()) {
        success = init_system_with_hsi();
    }

    return success;
}


} // namespace bsp



// Follow is for HAL support

/** HAL Msp Init
  *
  *  This function is called at eht end of HAL_Init() and must be used to init system and power clock.
  *  Today, this part of code is places to bsp::init(), so MspInit is empty.
  */

void HAL_MspInit(void) {
}

/** Function which used for handling errors when work with HAL functions.
 */
void Error_Handler(void) {
  __disable_irq();
  while (1) {
  }
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM15 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM15) {
        HAL_IncTick();
    }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line) {
}
#endif /* USE_FULL_ASSERT */
