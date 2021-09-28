/*  BSP. ADC Driver. STM32 HAL based implementation.
 */
#include "bsp_system.h"
#include "bsp_adc.h"

namespace bsp {
namespace adc {

static ADC_HandleTypeDef h;

bool init() {
    bool success = true;
    enable_adc_clk(adc_);

    // Common config
    h.Instance                    = adc_;
    h.Init.ClockPrescaler         = ADC_CLOCK_ASYNC_DIV128;
    h.Init.Resolution             = ADC_RESOLUTION_12B;
    h.Init.DataAlign              = ADC_DATAALIGN_RIGHT;
    h.Init.GainCompensation       = 0;
    h.Init.ScanConvMode           = ADC_SCAN_DISABLE;
    h.Init.EOCSelection           = ADC_EOC_SINGLE_CONV;
    h.Init.LowPowerAutoWait       = DISABLE;
    h.Init.ContinuousConvMode     = DISABLE;
    h.Init.NbrOfConversion        = 1;
    h.Init.DiscontinuousConvMode  = DISABLE;
    h.Init.ExternalTrigConv       = ADC_SOFTWARE_START;
    h.Init.ExternalTrigConvEdge   = ADC_EXTERNALTRIGCONVEDGE_NONE;
    h.Init.DMAContinuousRequests  = DISABLE;
    h.Init.Overrun                = ADC_OVR_DATA_OVERWRITTEN;
    h.Init.OversamplingMode       = DISABLE;
    success = success && HAL_ADC_Init(&h) == HAL_OK;

    // Configure the ADC multi-mode
    ADC_MultiModeTypeDef mm = {0};
    mm.Mode = ADC_MODE_INDEPENDENT;
    success = success && HAL_ADCEx_MultiModeConfigChannel(&h, &mm) == HAL_OK;

    // Configure Temperature sensor channel
    ADC_ChannelConfTypeDef s = {0};
    s.Channel      = ADC_CHANNEL_TEMPSENSOR_ADC1;
    s.Rank         = ADC_REGULAR_RANK_1;
    s.SamplingTime = ADC_SAMPLETIME_92CYCLES_5;
    s.SingleDiff   = ADC_SINGLE_ENDED;
    s.OffsetNumber = ADC_OFFSET_NONE;
    s.Offset       = 0;
    success = success && HAL_ADC_ConfigChannel(&h, &s) == HAL_OK;
    HAL_ADCEx_Calibration_Start(&h, ADC_SINGLE_ENDED);
    return success;
}

float temp_sensor_code_to_celsius(uint16_t code);

float get_chip_temp() {
    bool success = true;
    success = success && HAL_ADC_Start(&h) == HAL_OK;
    success = success && HAL_ADC_PollForConversion(&h, HAL_MAX_DELAY) == HAL_OK;
    HAL_ADC_Stop(&h);
    return success ? temp_sensor_code_to_celsius(HAL_ADC_GetValue(&h)) : -273.;
}

/*
 *  Temperature sensor value conversion for STM32G474
 *  in assumption that Vref = 3.0V. For other chips the procedure
 *  (and address of calibration data) will differ.
 */

#define TS_CAL1         (uint16_t *)0x1FFF'75A8;
#define TS_CAL2         (uint16_t *)0x1FFF'75CA;
#define VREFINT         (uint16_t *)0x1FFF'75AA;

float temp_sensor_code_to_celsius(uint16_t code) {
    float value;
    constexpr int16_t tcal1 = 30;
    constexpr int16_t tcal2 = 110;
    constexpr int16_t dt = tcal2 - tcal1;
    const int16_t tscal2 = *TS_CAL2;
    const int16_t tscal1 = *TS_CAL1;
    const int16_t dts = tscal2 - tscal1;

    float slope = (float) dt / dts;
    value = ((float)code - tscal1) * slope + tcal1;
    return value;
}


} // namespace adc
} // namespace bsp
