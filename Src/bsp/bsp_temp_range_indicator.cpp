/** BSP ADC Driver. Based on STM32 HAL.
 *
 */

#include "bsp_system.h"
#include "bsp_gpio.h"
#include "bsp_temp_range_indicator.h"

namespace bsp {
namespace temp_range_indicator {

float min_temp_c_;
float max_temp_c_;
float temp_c_;

/*  Config TEMP_RANGE_IND_Pin
 */
bool init(float min_temp_c, float max_temp_c) {
    enable_gpio_clk(port_);
    gpio::reset(port_, pin_);
    gpio::init(port_, pin_, GPIO_MODE_OUTPUT_PP);
    min_temp_c_ = min_temp_c;
    max_temp_c_ = max_temp_c;
    return true;
}

void update(float temp_c) {
    temp_c_ = temp_c;
    if (temp_c < min_temp_c_ || temp_c > max_temp_c_) {
        gpio::set(port_, pin_);
    } else {
        gpio::reset(port_, pin_);
    }
}

} // namespace temp_range_indicator
} // namespace bsp
