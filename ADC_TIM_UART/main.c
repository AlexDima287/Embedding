#include "stm32f4xx.h"                  // Device header
#include "stdio.h"

int data;
double voltage;
double celcius;
int USART2_Write(int ch);
void USART_Init(void);

int main()
{
	USART_Init();
	
	RCC->AHB1ENR |= 1;  

	// ADC setup
	RCC->APB2ENR |= 0x100; // ADC1 clock access
	ADC->CCR |= 0x800000; // VBATE must be disabled for the temp sensor 
	ADC1->SR = 0;
	ADC1->SMPR1 |= 0x4000000; // set the sampling rate to 84 cpu cycles/sample ~ 5 us
	ADC1->SQR3 = 18; // using ADC channel 18 - channel 18 is the first in the sequence to be converted	
	ADC1->CR2 = 1; 
	ADC1->CR2 |= 0x40000000;
	
	
	
	
	while (1)
	{
		while (ADC1->SR & 2) {} // wait until conversion is complete (EOC)
		data = ADC1->DR;
		voltage = 3.3*((double)data/4096);
		celcius = (voltage - 0.76)/0.025 + 25;			
			
		printf("%d, %.2f\370C\r\n", data, celcius);
		
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


