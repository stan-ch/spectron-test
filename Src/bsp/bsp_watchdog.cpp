/*  BSP Watchdog. Based on STM32 HAL.
 *
 */

#include "bsp_hal.h"
#include "bsp_watchdog.h"

namespace bsp {
namespace watchdog {

static IWDG_HandleTypeDef h;

void init() {
    h.Instance = iwdg;
    h.Init.Prescaler = IWDG_PRESCALER_64;
    h.Init.Window = 4095;
    h.Init.Reload = 4095;
    if (HAL_IWDG_Init(&h) != HAL_OK) {
        Error_Handler();
    }
}

void refresh() {
    HAL_IWDG_Refresh(&h);
}

} // namespace watchdog
} // namspace bsp
