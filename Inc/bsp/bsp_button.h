/*  BSP on board button (Nucleo-G474RE).
 *
 */

#ifndef BSP_BUTTON_H
#define BSP_BUTTON_H

#include "bsp_hal.h"

namespace bsp {
namespace button {

//---------------------------------------------------------- Used resources ---

/* Input pin.
 */

static GPIO_TypeDef* port_      [[gnu::unused]] = GPIOC;
constexpr uint32_t pin_                         = GPIO_PIN_13;

/** Process pressing of the button. */

void irq_callback();

// --------------

/** Init button's pin as input. Enable interrupt.
 */
void init();

} // namespace buttons
} // namespace bsp

#endif /* BSP_BUTTON_H */
