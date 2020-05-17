#include "stm32f4xx_hal.h"              // Keil::Device:STM32Cube HAL:Common
#include "GLCD_ST7735.h"

void ST7735_GPIO_Init(void)
{
	// SPI2 pins
	// RES PB5
	// DC PB4
	// CS PB3
	
	__GPIOB_CLK_ENABLE(); // enable clock for GPIOB

	GPIO_InitTypeDef GPIO_InitStruct; // create a variable of the GPIO_InitTypeDef structure
	
	GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_3; // configure structure parameters
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct); // HAL init the structure we have built
	
}

	SPI_HandleTypeDef hspi2; // create a global variable of the SPI_HandleTypeDef structure type 

void ST7735_Init(void)
{
		hspi2.Instance = SPI2; 
		hspi2.Init.Mode = SPI_MODE_MASTER;
		hspi2.Init.Direction = SPI_DIRECTION_1LINE;
		hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
		hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
		hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
		hspi2.Init.NSS = SPI_NSS_SOFT;
		hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2; // no prescaler
		hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
		hspi2.Init.TIMode = SPI_TIMODE_DISABLED;
		hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED;
		hspi2.Init.CRCPolynomial = 10;
	
		HAL_SPI_Init(&hspi2);
	
}

void SPI2_8BIT_Init(void)
{
	hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
	HAL_SPI_Init(&hspi2);
}

void SPI2_16BIT_Init(void)
{
	hspi2.Init.DataSize = SPI_DATASIZE_16BIT;
	HAL_SPI_Init(&hspi2);
}

void lcd7735_send_8bit_data(unsigned char data) // send 8 bit data
{
	HAL_SPI_Transmit(&hspi2, &data, 1, 0x1);
}

void lcd7735_recieve_8bit_data(unsigned char data) // receive 8 bit data
{
	HAL_SPI_Receive(&hspi2, &data, 1, 0x1);
}


void lcd7735_send_16bit_data(uint8_t msb, uint8_t lsb )
{
	uint8_t masData[] = {lsb,msb};
	HAL_SPI_Transmit(&hspi2, masData, 1, 0x1);
}

void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
	// SPI2_MOSI - PC3
	// SPI2_SCK - PB10

	__SPI2_CLK_ENABLE(); 
	
	GPIO_InitTypeDef GPIO_InitStruct; 
	
	GPIO_InitStruct.Pin = GPIO_PIN_10; // configure structure parameters
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
	
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct); // HAL init the structure we have built for PB10
	
	GPIO_InitStruct.Pin = GPIO_PIN_3; 

	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct); // HAL init the structure we have built for PC13
	
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef *hspi) // de-initialize the pins
{
	HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10);
	HAL_GPIO_DeInit(GPIOC, GPIO_PIN_3);
}

