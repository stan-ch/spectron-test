# spectron-test

Example of test task implementation.

## Project info

* STM32CubeIDE (Eclipse)
* Gnu gcc + g++ compilers
* STM32 HAL as Hardware abstraction library
* Tested on STM32G474 Nucleo-G474RE board.

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
