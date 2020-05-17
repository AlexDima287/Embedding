#include "stm32f4xx_hal.h"              // Keil::Device:STM32Cube HAL:Common
#include "LIS3DH.h"

void LIS3DH_GPIO_Init(void);
void LIS3DH_Init(void);
void LIS3DH_send_8bit_data(unsigned char data);
void LIS3DH_recieve_8bit_data(unsigned char data);
void LIS3DH_recieve_16bit_data(unsigned char *data);
void LIS3DH_send_16bit_data(unsigned char msb, unsigned char lsb);
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi);
void HAL_SPI_MspDeInit(SPI_HandleTypeDef *hspi);
void SystickDelayMs(int delay);



// Slave control registers
const char CTRL_REG1 = 0x20;
const char CTRL_REG2 = 0x21;
const char CTRL_REG3 = 0x22;
const char CTRL_REG4 = 0x23;
const char CTRL_REG5 = 0x24;
const char CTRL_REG6 = 0x25;

// Slave status register
const char STATUS_REG = 0x27;

// Slave output registers
const char OUT_X_L = 0x28;
const char OUT_X_H = 0x29;
const char OUT_Y_L = 0x2A;
const char OUT_Y_H = 0x2B;
const char OUT_Z_L = 0x2C;
const char OUT_Z_H = 0x2D;

SPI_HandleTypeDef hspi; // create a global variable of the SPI_HandleTypeDef structure type 

uint8_t spiTxbuff[2];
uint8_t spiRxbuff[2];

int main()
{

	SystickDelayMs(2000);
	// initialization
	
	HAL_Init();
	
	LIS3DH_GPIO_Init();
	LIS3DH_Init();
	HAL_SPI_MspInit(&hspi);
	
	// SPI write
	spiTxbuff[0] = CTRL_REG4;
	spiTxbuff[1] = 0x08;
	
	LIS3DH_Chip_Select;
	HAL_SPI_Transmit(&hspi, spiTxbuff, 2, 50);							// enable High Res, Little endian, continuous update, FS = +/-2g, block update
	LIS3DH_Chip_DeSelect;
	
	spiTxbuff[0] = CTRL_REG1;
	spiTxbuff[1] = 0x67;
	
	LIS3DH_Chip_Select;
	HAL_SPI_Transmit(&hspi, spiTxbuff, 2, 50);             // take device out of power down, and ODR = 200 Hz, all axes enabled
	LIS3DH_Chip_DeSelect;
	
	// SPI read
	
	spiTxbuff[0] = OUT_X_H | 0x80;    // MSB set to 1 for reading
	
	LIS3DH_Chip_Select;
	HAL_SPI_Transmit(&hspi, spiTxbuff, 1, 50);  
	HAL_SPI_Receive(&hspi, spiRxbuff, 1, 50);
	LIS3DH_Chip_DeSelect;
	
	HAL_SPI_MspDeInit(&hspi);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void LIS3DH_GPIO_Init(void)
{

	// CS PD14
	
	__GPIOD_CLK_ENABLE(); // enable clock for GPIOD

	GPIO_InitTypeDef GPIO_InitStruct; // create a variable of the GPIO_InitTypeDef structure
	
	GPIO_InitStruct.Pin = GPIO_PIN_14; // configure structure parameters
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct); // HAL init the structure we have built
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
}


void LIS3DH_Init(void)
{
		hspi.Instance = SPI1; 
		hspi.Init.Mode = SPI_MODE_MASTER;
		hspi.Init.Direction = SPI_DIRECTION_1LINE;
		hspi.Init.DataSize = SPI_DATASIZE_16BIT;
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
	GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
	
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

void SystickDelayMs(int delay)
{
	int i;
	
	SysTick->LOAD = 16000; // number of clock cycles per millisecond
	SysTick->VAL = 0;
	SysTick->CTRL = 0x5;
	for (i = 0; i<delay; i++) // for each millisecond
	{
	while ((SysTick->CTRL & 0x10000) == 0)  // wait for 16000 clock cycles
	{}
	}
	
	SysTick->CTRL = 0;
}




