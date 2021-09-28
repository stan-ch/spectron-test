/*  BSP. ADC driver
 *
 *  Provide measure from temperature sensor (chip temperature).
 */

#ifndef BSP_ADC_H_
#define BSP_ADC_H_

#include <bsp_hal.h>

namespace bsp {
namespace adc {

//---------------------------------------------------------- Used resources ---

/** Adc instance.
 */
static ADC_TypeDef* adc_        [[gnu::unused]] = ADC1;

//-----------------------------------------------------------------------------

/** Enable ADC and initialize it.
 *
 *  @return true if initialization was successful, false otherwise
 */

bool init();

/** Get value from temperature sensor converted to celsius.
 *
 *  Start adc, wait until it's finished, convert to celsius and return result.
 *
 *  @return chip temperature in celsius degree. If an error was occured return -273.
 */

float get_chip_temp();

} // namespace adc
} // namespace bsp


#endif /* BSP_ADC_H_ */
