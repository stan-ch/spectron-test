/*  BSP UART. Based on  STM32 HAL.
 */

#include <stdio.h>
#include <FreeRTOS.h>
#include <semphr.h>
#include "bsp_hal.h"
#include "bsp_gpio.h"
#include "bsp_system.h"
#include "modbus_crc16.h"
#include "bsp_uart.h"

namespace bsp {
namespace uart {

static UART_HandleTypeDef h;

static SemaphoreHandle_t mutex_;

static void init_pins() {
    enable_gpio_clk(tx_port_);
    gpio::init_alternate(tx_port_, tx_pin_, GPIO_MODE_AF_PP, tx_pin_af_);
}

bool init() {
    bool success = true;
    init_pins();

    enable_usart_clk(uart_);
    h.Instance                    = uart_;
    h.Init.BaudRate               = baudrate_;
    h.Init.WordLength             = length_;
    h.Init.StopBits               = stop_bits_;
    h.Init.Parity                 = parity_;
    h.Init.Mode                   = UART_MODE_TX;
    h.Init.HwFlowCtl              = UART_HWCONTROL_NONE;
    h.Init.OverSampling           = UART_OVERSAMPLING_16;
    h.Init.OneBitSampling         = UART_ONE_BIT_SAMPLE_DISABLE;
    h.Init.ClockPrescaler         = UART_PRESCALER_DIV1;
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

static void send_buf(uint8_t* buf, size_t bytes_length) {
    const int32_t timeout_ms = 10L + 2L * bytes_length * 10 * 1000 / (h.Init.BaudRate == 0 ? 9600 : h.Init.BaudRate);
    xSemaphoreTake(mutex_, portMAX_DELAY);
    HAL_UART_Transmit(&h, buf, bytes_length, timeout_ms);
    xSemaphoreGive(mutex_);
}

void send(float temp_c) {
    union Data {
        struct {
            float temp_c;
            uint16_t crc;
        } fields;
        uint8_t buf[4];
    } data;

    data.fields.temp_c = temp_c;
    data.fields.crc    = modbus_crc16(&temp_c, 2); // or use STM32 CRC unit
    send_buf(data.buf, 4);
}


} // namespace uart
} // namspace bsp
