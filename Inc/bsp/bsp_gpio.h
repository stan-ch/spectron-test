/** Common operations with GPIO based on STM32 HAL.
 *
 *  We use "internal" interface with GPIO to make less work when hal library
 *  will be changed.
 */

#ifndef BSP_GPIO_H
#define BSP_GPIO_H

#include "bsp_hal.h"

namespace bsp {
namespace gpio {

static inline void init(GPIO_TypeDef* port,
                        uint32_t pin_mask,
                        uint32_t mode,
                        uint32_t pull = GPIO_NOPULL,
                        uint32_t speed = GPIO_SPEED_FREQ_LOW) {
    GPIO_InitTypeDef s = {0};
    s.Pin = pin_mask;
    s.Mode = mode;
    s.Pull = pull;
    s.Speed = speed;
    HAL_GPIO_Init(port, &s);
}

static inline void init_alternate(GPIO_TypeDef* port,
                        uint32_t pin_mask,
                        uint32_t mode,
                        uint32_t alternate,
                        uint32_t pull = GPIO_NOPULL,
                        uint32_t speed = GPIO_SPEED_FREQ_LOW) {
    GPIO_InitTypeDef s = {0};
    s.Pin = pin_mask;
    s.Mode = mode;
    s.Pull = pull;
    s.Speed = speed;
    s.Alternate = alternate;
    HAL_GPIO_Init(port, &s);
}


static inline void reset(GPIO_TypeDef* port, uint32_t pin_mask) {
    HAL_GPIO_WritePin(port, pin_mask, GPIO_PIN_RESET);
}

static inline void set(GPIO_TypeDef* port, uint32_t pin_mask) {
    HAL_GPIO_WritePin(port, pin_mask, GPIO_PIN_SET);
}

static inline bool is_set(GPIO_TypeDef* port, uint32_t pin_mask) {
    return HAL_GPIO_ReadPin(port, pin_mask) == GPIO_PIN_SET;
}

static inline bool is_reset(GPIO_TypeDef* port, uint32_t pin_mask) {
    return HAL_GPIO_ReadPin(port, pin_mask) == GPIO_PIN_RESET;
}

} // namespace gpio
} // namespace bsp

#endif
