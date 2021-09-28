/**
 *  Change temp range limits and check temp_range_indicator ouput.
 *
 *  Be sure that lpuart is already init before call #simulate().
 */


#include <FreeRTOS.h>
#include <task.h>
#include <string.h>
#include "bsp_system.h"
#include "bsp_gpio.h"
#include "bsp_lpuart.h"
#include "bsp_temp_range_indicator.h"
#include "temp_range_indicator_test.h"


namespace bsp {
namespace temp_range_indicator_test {

constexpr bool IS_PIN_SET = true;
constexpr bool IS_PIN_RESET = false;

void init() {
    xTaskCreate(simulate, "Temp range test", 1024, NULL, 2, NULL);
}

static void check(bool expected_is_pin_set, uint16_t timeout_ms = 0) {
    using namespace bsp::temp_range_indicator;
    bool is_pin_set = gpio::is_set(port_, pin_);
    log("[TEMP_RANGE_TEST] Test %s : Min = %.1f, Max = %.1f, Actual = %.1f, Out = %s\r\n", 
        is_pin_set == expected_is_pin_set ? "PASSED" : "FAILED",
        min_temp_c_, max_temp_c_, temp_c_, is_pin_set ? "HIGH" : "LOW");
    vTaskDelay(pdMS_TO_TICKS(timeout_ms));
}

static void change_range(float min_temp_c, float max_temp_c, uint16_t timeout_ms = 2000) {
    temp_range_indicator::init(min_temp_c, max_temp_c);
    vTaskDelay(pdMS_TO_TICKS(timeout_ms));
}

void simulate(void* pvParameters) {
    using namespace bsp::temp_range_indicator;
  
    log("[TEMP_RANGE_TEST] Init \r\n");
    while (1) {
        log("[TEST_RANGE_TEST] Start cycle\n\r");
        change_range(-10, 50);
        check(!IS_PIN_SET);
        change_range(-10, -9);
        check(IS_PIN_SET);
        change_range(50, 100);
        check(IS_PIN_SET);
  }
}

} // namespace temp_range_indicator_test
} // namespace bsp
