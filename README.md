# spectron-test

Example of test task implementation.

##Project info

  - STM32CubeIDE (Eclipse)
  - Gnu gcc + g++ compilers
  - STM32 HAL as Hardware abstraction library
  - Tested on STM32G474 Nucleo-G474RE board.

##Folder structure

Inc - application includes
  bsp - application board support library includes
  test - tasks for test something

Src - application sources
  bsp - application board support library sources
  test - tasks for test sources

Startup/
  startup_stm32g474retx.s - table of vectors

Libraries/
  CMSIS - CMSIS library
  FreeRTOS - FreeRTOS library
  STM32G4xx_HAL_Driver - STM32 HAL
  ThreadSafe - STM32 related 
