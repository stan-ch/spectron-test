/**
 *  BSP Onboard Button
 */

#include "bsp_hal.h"
#include "bsp_system.h"
#include "bsp_gpio.h"
#include "bsp_button.h"

namespace bsp {
namespace button {

void init() {
    bsp::enable_gpio_clk(port_);
    gpio::init(port_, pin_, GPIO_MODE_IT_RISING);
    bsp::enable_exti_irq(pin_);

//    HAL_NVIC_SetPriority(irqn_, 5, 0);
//    HAL_NVIC_EnableIRQ(irqn_);
}

void irq_callback() {
    
}

} // namespace button
} // namespace bsp
