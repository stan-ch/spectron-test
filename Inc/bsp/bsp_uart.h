/*  BSP UART.
 *
 *  This UART is used to send message (temperature with CRC) outside. All
 *  functionality (pack data to frame, crc calculation and transfer message)
 *  is implenented here (yet).
 *
 *  Messages can be only transmitted.
 *
 *  The message contains two 16 bits words:
 *    #0 - float, 16 bit - chip temperature in celsius
 *    #1 - unsigned, CRC16 (modbus).
 *
 *  UART trasmitter:
 *  - TX pin      : PC10
 *  - Baudrate    : 9600
 *  - Word length : 8
 *  - Parity      : None
 *  - Stop bit    : 1
 */

#ifndef BSP_UART_H
#define BSP_UART_H

#include "bsp_hal.h"

namespace bsp {
namespace uart {

//---------------------------------------------------------- Used resources ---

/** BSP UART instance (only TX output is used).
 */

static USART_TypeDef* uart_     [[gnu::unused]] = UART4;

/*  Tx output.
 */

static GPIO_TypeDef* tx_port_   [[gnu::unused]] = GPIOC;
constexpr uint32_t tx_pin_                      = GPIO_PIN_10;
constexpr uint8_t tx_pin_af_                    = GPIO_AF5_UART4;

/* RX pin not used. just reserved.
 */

static GPIO_TypeDef* rx_port_   [[gnu::unused]] = GPIOC;
constexpr uint32_t rx_pin_                      = GPIO_PIN_13;
constexpr uint8_t rx_pin_af_                    = GPIO_AF5_UART4;

//-------------------------------------------------------------------Params ---

constexpr uint32_t baudrate_                    = 9600;
constexpr uint32_t length_                      = UART_WORDLENGTH_8B;
constexpr uint32_t stop_bits_                   = UART_STOPBITS_1;
constexpr uint32_t parity_                      = UART_PARITY_NONE;

//-----------------------------------------------------------------------------

/** Initialize UART.
 */

bool init();

/** Send temperature to UART. 
 *
 */

void send(float temp_c);

} // namespace uart
} // namespace bsp

#endif /* BSP_UART_H */
