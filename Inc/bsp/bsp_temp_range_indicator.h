/*  BSP. Temp range indicator
 *
 *  The simple output is used to indicate either temperature sensor is in range
 *  or not: 
 *  - ouput is high when temperature is in the range
 *  - ouput is low  when temperature is out of the range
 *
 *  The temperature range is specified in #init().
 *
 *  Usage:
 *
 *    bsp::temp_range_indicator::init(-10, 50);
 *    bsp::temp_range_indicator::update(new_temp);
 */

#ifndef BSP_TEMP_RANGE_IND_H
#define BSP_TEMP_RANGE_IND_H

#include "bsp_hal.h"

namespace bsp {
namespace temp_range_indicator {

//---------------------------------------------------------- Used resources ---

/* Output pin to indicate is temperature is range (set) or not (reset).
 */

static GPIO_TypeDef* port_      [[gnu::unused]] = GPIOC;
constexpr uint32_t pin_                         = GPIO_PIN_4;

// For debug purpose placed here, not in cpp.
extern float min_temp_c_;
extern float max_temp_c_;
// Intended for debug purporse only.
extern float temp_c_;

// -----------------------------------------------------------------------------

/** Initialize Temp range indicator with valid range.
 *
 *  The indicator's pin is initiated as low output. 
 *
 *  @return true if success, false otherwise
 */

bool init(float min_temp_c, float max_temp_c);

/** Update GPIO state according a new temperature \a temp_c (celsius).
 */

void update(float temp_c);

} // namespace temp_range_indicator
} // namespace bsp


#if EXAMPLE_HOW_TO_IMPLEMENT_THIS_WITH_CLASS___NOT_FOR_COMPILE

//-----------------------------------------------------------------------------
//  Just a sample how to implement the same as a class
//-----------------------------------------------------------------------------

/** Temp range indicator.
 *
 *  The simple output is used to indicate either temperature sensor is in range
 *  or not: 
 *  - ouput is high when temperature is in the range
 *  - ouput is low  when temperature is out of the range
 *
 *  The range is specified in #init().
 *
 *  Usage:
 *
 *    Temp_range_indicator x(-10, 50);
 *    x.init();
 *    x.update(new_temp)
 */

class Temp_range_indicator {
public:

    /** Create the object with specified range [min_temp_c; max_temp_c].
     */

    Temp_range_indicator(float min_temp_c, float max_temp_c);

    /** Initialize Temp range indicator. The indicator's pin is initiated as low
     *  output.
     */

    void init();

    /** Update gpio state according \a temp_C (celsius).
     */

    void update(float temp_C);

    /** Get min limit of the range, C.
     */

    float get_min_temp_c();

    /** Get max limit of the range, C.
     */

    float get_max_temp_c();

private:
    /*  Output pin to indicate is temperature is range (set) or not (reset).
     */

    constexpr uint32_t pin_                     = GPIO_PIN_5;
    static GPIO_TypeDef* port_                  = GPIOA;
    float min_temp_c_;
    float max_temp_c_;
};

#endif

#endif /* BSP_TEMP_RANGE_IND_H */
