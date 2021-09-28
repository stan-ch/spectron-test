# spectron-test

Example of test task implementation.

## Project environment

- STM32CubeIDE (Eclipse)
- Gnu gcc + g++ compilers
- STM32 HAL as Hardware abstraction library
- Tested on STM32G474 Nucleo-G474RE board.

## MCU STM32G747 resources

### GPIO

  | GPIO | Type        |                         | Description                    |
  | ---- | ----------- | ----------------------- | ------------------------------ |
  | PA6  | input       | pwm_launcher            | start input                    |
  | PA8  | input       | pwm_launcher            | start finite input             |
  | PA9  | input       | pwm_launcher            | stop input                     |
  | PA0  | output      | pwm                     | PWM output                     |
  | PA5  | output      | temp_range_indicator    | indicator output               |
  | PC10 | output      | uart                    | UART TX output                 |
  | PC11 | (reserverd) | uart                    | Reserved for UART RX input     |
  | PA2  | output      | nucleo                  | Nucleo's UART TX output        |
  | PA3  | input       | nucleo                  | Nucleo's UART RX input         |
  | PA5  | output      | nucleo                  | Nucleo's led                   |
  | PC13 | input       | nucleo                  | Nucleo's button                |
  | PC0  | output      | pwm_launcher_inputs_sim | feeds start input (PA6)        |
  | PC1  | output      | pwm_launcher_inputs_sim | feeds start finite input (PA8) |
  | PC2  | output      | pwm_launcher_inputs_sim | feeds stop input (PA9)         |

### Modules

  | Module  | Extra info | Description                      |
  | ------- | ---------- | -------------------------------- |
  | ADC1    | channel 1  | measuring chip temperature       |
  | TIM2    | channel 1  | PWM timer                        |
  | TIM3    | channel 1  | one pulse timer (5s)             |
  | UART4   | TX         | application uart (9600, 8, n, 1) |
  | LPUART1 | TX         | debug uart (115200, 8, n, 1)     |
  | IWDG    |            | watchdog                         |

## Folder structure

- Inc - application includes
  - Inc/bsp - application board support library includes
  - Inc/test - tasks for test something

- Src - application sources
  - Src/bsp - application board support library sources
  - Src/test - tasks for test sources

- Startup/
  - Startup/startup_stm32g474retx.s - table of vectors

- Libraries/
  - Libraries/CMSIS - CMSIS library
  - Libraries/FreeRTOS - FreeRTOS library
  - Libraries/STM32G4xx_HAL_Driver - STM32 HAL
  - Libraries/ThreadSafe - STM32 related
