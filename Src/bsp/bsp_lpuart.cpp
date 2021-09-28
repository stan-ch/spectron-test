/** BSP LPUART.
 */

#include <stdio.h>
#include <FreeRTOS.h>
#include <semphr.h>
#include "bsp_hal.h"
#include "bsp_system.h"
#include "bsp_gpio.h"
#include "bsp_lpuart.h"


namespace bsp {
namespace lpuart {

static UART_HandleTypeDef h;

static SemaphoreHandle_t mutex_;

static void init_pins() {
    enable_gpio_clk(tx_port_);
    enable_gpio_clk(rx_port_);
    gpio::init_alternate(tx_port_, tx_pin_, GPIO_MODE_AF_PP, tx_pin_af_);
    gpio::init_alternate(rx_port_, rx_pin_, GPIO_MODE_AF_PP, rx_pin_af_);
    
}

bool init(void) {
    bool success = true;
    init_pins();

    enable_usart_clk(uart_);
    h.Instance = uart_;
    h.Init.BaudRate = baudrate_;
    h.Init.WordLength = length_;
    h.Init.StopBits = stop_bits_;
    h.Init.Parity = parity_;
    h.Init.Mode = UART_MODE_TX_RX;
    h.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    h.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    h.Init.ClockPrescaler = UART_PRESCALER_DIV1;
    h.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    success = success && HAL_UART_Init(&h) == HAL_OK;
    success = success && HAL_UARTEx_SetTxFifoThreshold(&h, UART_TXFIFO_THRESHOLD_1_8) == HAL_OK;
    success = success && HAL_UARTEx_SetRxFifoThreshold(&h, UART_RXFIFO_THRESHOLD_1_8) == HAL_OK;
    success = success && HAL_UARTEx_DisableFifoMode(&h) == HAL_OK;
    if (success) {
        mutex_ = xSemaphoreCreateMutex();
        success = mutex_ != nullptr;
    }
    return success;
}

// Use double value of time that need to send the full message. 10ms min.
void send(char* buf) {
    send((const char*)(buf));
}

// Use double value of timeout_ms that need to send the full message. 10ms min.
void send(const char* buf) {
    const uint16_t len = strlen(buf);
    const int32_t timeout_ms = 10L + 2L * len * 10 * 1000 / (h.Init.BaudRate == 0 ? 9600 : h.Init.BaudRate);
    xSemaphoreTake(mutex_, portMAX_DELAY);
    HAL_UART_Transmit(&h, (uint8_t*)buf, len, timeout_ms);
    xSemaphoreGive(mutex_);
}


} // namespace lpuart
} // namespace bsp
