#include "stm32f4xx_hal.h"              // Keil::Device:STM32Cube HAL:Common
#include "LIS3DH.h"


void LIS3DH_GPIO_Init(void)
{

	// CS PD14
	
	__GPIOD_CLK_ENABLE(); // enable clock for GPIOD

	GPIO_InitTypeDef GPIO_InitStruct; // create a variable of the GPIO_InitTypeDef structure
	
	GPIO_InitStruct.Pin = GPIO_PIN_14; // configure structure parameters
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct); // HAL init the structure we have built
	
}

	SPI_HandleTypeDef hspi; // create a global variable of the SPI_HandleTypeDef structure type 

void LIS3DH_Init(void)
{
		hspi.Instance = SPI1; 
		hspi.Init.Mode = SPI_MODE_MASTER;
		hspi.Init.Direction = SPI_DIRECTION_1LINE;
		hspi.Init.DataSize = SPI_DATASIZE_8BIT;
		hspi.Init.CLKPolarity = SPI_POLARITY_LOW;
		hspi.Init.CLKPhase = SPI_PHASE_1EDGE;
		hspi.Init.NSS = SPI_NSS_SOFT;
		hspi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2; // no prescaler
		hspi.Init.FirstBit = SPI_FIRSTBIT_MSB;
		hspi.Init.TIMode = SPI_TIMODE_DISABLED;
		hspi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED;
		hspi.Init.CRCPolynomial = 10;
	
		HAL_SPI_Init(&hspi);
	
}


void LIS3DH_send_8bit_data(unsigned char data) // send 8 bit data
{
	HAL_SPI_Transmit(&hspi, &data, 1, 0x1);
}

void LIS3DH_recieve_8bit_data(unsigned char data) // receive 8 bit data
{
	HAL_SPI_Receive(&hspi, &data, 1, 0x1);
}

void LIS3DH_recieve_16bit_data(unsigned char *data) // receive 8 bit data
{
	HAL_SPI_Receive(&hspi, data, 2, 0x1);
}


void LIS3DH_send_16bit_data(uint8_t msb, uint8_t lsb)
{
	uint8_t masData[] = {lsb,msb};
	HAL_SPI_Transmit(&hspi, masData, 1, 0x1);
}

void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
	// SPI1_SCK PA5 -- alternate function
	// SPI1_MISO PA6 -- alternate function
	// SPI1_MOSI PA7 -- alternate function


	__SPI1_CLK_ENABLE(); 
	
	GPIO_InitTypeDef GPIO_InitStruct; 
	
	GPIO_InitStruct.Pin = GPIO_PIN_5; // configure structure parameters
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
	
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); // HAL init the structure we have built for PA5
	
	GPIO_InitStruct.Pin = GPIO_PIN_6; 

	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); // HAL init the structure we have built for PA6
	
	GPIO_InitStruct.Pin = GPIO_PIN_7; 

	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); // HAL init the structure we have built for PA7
	
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef *hspi) // de-initialize the pins
{
	HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5);
	HAL_GPIO_DeInit(GPIOA, GPIO_PIN_6);
	HAL_GPIO_DeInit(GPIOA, GPIO_PIN_7);
	
}

