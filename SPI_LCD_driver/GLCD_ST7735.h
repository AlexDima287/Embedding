#ifndef __GLCD_ST7735
#define __GLCD_ST7735
#include <stdint.h>
#include "stm32f4xx_hal.h"              // Keil::Device:STM32Cube HAL:Common


#define LCD_RST1 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET)
#define LCD_RST0 HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET)
#endif


