/*  BSP LPUART. Used to out log messages to terminal.
 *
 *  UART trasmitter:
 *  - TX pin      : PA2
 *  - Baudrate    : 115200
 *  - Word length : 8
 *  - Parity      : None
 *  - Stop bit    : 1
 */

#ifndef BSP_LPUART_H
#define BSP_LPUART_H

#include <string.h>
#include <stdio.h>
#include <cstdarg>
#include "bsp_hal.h"

namespace bsp {
namespace lpuart {

//---------------------------------------------------------- Used resources ---

/** UART instance (only TX output is used).
 */

static USART_TypeDef* uart_     [[gnu::unused]] = LPUART1;

/* TX output.
 */

static GPIO_TypeDef* tx_port_   [[gnu::unused]] = GPIOA;
constexpr uint32_t tx_pin_                      = GPIO_PIN_2;
constexpr uint8_t tx_pin_af_                    = GPIO_AF12_LPUART1;

/* RX input (not used, just reserved).
 */

static GPIO_TypeDef* rx_port_   [[gnu::unused]] = GPIOA;
constexpr uint32_t rx_pin_                      = GPIO_PIN_3;
constexpr uint8_t rx_pin_af_                    = GPIO_AF12_LPUART1;

//-------------------------------------------------------------------Params ---

constexpr uint32_t baudrate_                    = 115200;
constexpr uint32_t length_                      = UART_WORDLENGTH_8B;
constexpr uint32_t stop_bits_                   = UART_STOPBITS_1;
constexpr uint32_t parity_                      = UART_PARITY_NONE;

/** Init UART.
 *
 *  @return true if success, false otherwise.
 */

bool init();

/** Send string buffer (ends with \0) to UART.
 */

void send(char* buf);

/** Send const string buffer (ends with \0) to UART.
 */

void send(const char* buf);

} // namespace lpuart
} // namespace bsp

#endif /* BSP_LPUART_H */
