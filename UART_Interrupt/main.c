#include "stm32f4xx.h"                  // Device header


void UART2_Init(void);
void SystickDelayMs(int delay);
void LED_Play(int value);

int main()
{
	__disable_irq();
	
	RCC->AHB1ENR|= 1;
	GPIOA->MODER |= 0x400;
	UART2_Init();
	USART2->CR1 |= 0x0020; // enable RX interrupt (RXNEIE)
	NVIC_EnableIRQ(USART2_IRQn);
	
	__enable_irq();
	
	
	while (1){}
}


void UART2_Init(void)
{
	RCC->AHB1ENR |= 1;
	RCC->APB1ENR |= 0x20000; // enable clock access to UART2
	
	GPIOA->AFR[0] |= 0x7000; // AF7 UART2 PA3 (RX)
	GPIOA->MODER |= 0x80; // PA3 alternate function mode
	
	USART2->BRR = 0x008B; // 115200 baud rate @16MHz
	USART2->CR1 |= 0x2004; // enable reciever, enable uart
	
}


void LED_Play(int value)
{

value %= 16;
	for (;value>0; value--)
	{
		GPIOA->ODR |= 0x0020;  // turn on LED  0b   0000 0000 0000 0000 0000 0000 0010 0000
		SystickDelayMs(100);
		GPIOA->ODR &=~ 0x0020; // turn off LED
		SystickDelayMs(100);
	}
	SystickDelayMs(400);
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

void USART2_IRQHandler(void)
{
	char c;
	
	if (USART2->SR & 0x0020)
	{
		c = USART2->DR;
		LED_Play(c);
	}


}


