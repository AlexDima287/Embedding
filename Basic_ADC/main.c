#include "stm32f4xx.h"                  // Device header
#include "stdio.h"

// ADC connected to PA1, APB2
// this program uses the on board ADC on PA1 to output decimal values to UART2 (TX)

int USART2_Write(int ch);
void USART_Init(void);
int analog_value;


int main()
{
	USART_Init();
	RCC->APB2ENR |= 0x0100; // enable clock access for ADC1
	RCC->AHB1ENR |= 1;			// enable clock access for GPIOA
	
	GPIOA->MODER |= 0xC; // make PA1 analog mode
	
	ADC1->SMPR1 |= 0x38; // 30 us = 480 cycles
	ADC1->CR2 = 0;
	ADC1->SQR3  = 1; // channel 1 is the 1st in the sequence to be converted
	ADC1->CR2 |= 1;

	while (1)
	{
		ADC1->CR2 |= 0x40000000;
		
		while (!(ADC1->SR)){}		// wait for EOC flag (conversion to be complete)
		analog_value = ADC1->DR;
		printf("Mic reading: %.2f\r\n",(double)analog_value);
	}
	


}

void USART_Init(void)
{
	// General Setup
	
RCC->APB1ENR |= 0x020000;    // clock enable for USART2
RCC->AHB1ENR |= 1;          // clock enable for GPIOA
USART2->CR1   = 0x2000;    // UART enable
USART2->BRR   = 0x0683;   // 9600 @ 16MHz

	// TX setup: PA2
	
GPIOA->MODER |= 0x020;    // makes GPIO PA2 Alternate function
GPIOA->AFR[0]|= 0x0700;  // makes GPIO PA2 the USART AF (AF7)
USART2->CR1  |= 0x008;  // Transmitter enable

}

struct __FILE{int handle;};
FILE __stdout = {1};

int fputc(int c, FILE *f)
{
	return USART2_Write(c);
}
	
int USART2_Write(int ch)
{
	while (!(USART2->SR & 0x0080)){}
	USART2->DR = ch;
	return ch;
}

