#ifndef LIS3DH
#define LIS3DH
#include <stdint.h>
#include "stm32f4xx_hal.h"              // Keil::Device:STM32Cube HAL:Common

#define LIS3DH_Chip_Select HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET)
#define LIS3DH_Chip_DeSelect HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET)


#endif

