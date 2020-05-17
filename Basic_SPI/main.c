#include "stm32f4xx.h"                  // Device header
// PA7 MOSI
// PA6 MISO
// PA5 SCK
// PA4 SS
void SPI_Write_Data(unsigned char data);
void SPI_Read_Data(unsigned char data);

int main()
{





}

void SPI_Init(void)
{
	RCC->AHB1ENR |= 1; // clock access to GPIOA
	RCC->APB2ENR |= 0x1000; // clock access to SPI1
	
	GPIOA->MODER &=~ 0x0000CC00; // deselect the mode
	GPIOA->MODER |= 0x0000A800;	// enable alternate functions for PA5,PA6,PA7

	GPIOA->AFR[0] &=~ 0xFFF00000; // deselect the mode
	GPIOA->AFR[0] |= 0x55500000; // make PA5, PA6, PA7 SPI1
	
	GPIOA->MODER &=~ 0x00000300; // deselect the mode
	GPIOA->MODER |= 0x00000100;	// enable standard output for PA4: SS
	
	// SPI config
	SPI1->CR1 = 0x31C; // set baud rate 1 MHz, 8-bit data frame, master mode
	SPI1->CR2 = 0; 
	SPI1->CR1 |= 0x40; // peripheral enable
}


void SPI_Write_Data(unsigned char data)
{
	while (!(SPI1->SR & 2)){} // wait until transmit buffer empty
	GPIOA->BSRR |= 0x100000; // assert the slave
	
	SPI1->DR = data; //send data
	
	while (!(SPI1->SR & 0x80)){} // wait until SPI is not busy (transmission completion)
	GPIOA->BSRR |= 0x00010; // de-assert the slave
}

void SPI_Read_Data(unsigned char data)
{
	GPIOA->BSRR |= 0x100000; // assert the slave
	while (!(SPI1->SR & 1)){} // wait until recieve buffer not empty
	
	data = SPI1->DR; //recieve data
	
	while (!(SPI1->SR & 0x80)){} // wait until SPI is not busy (reception completion)
	GPIOA->BSRR |= 0x00010; // de-assert the slave
}



