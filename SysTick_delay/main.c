#include "stm32f4xx.h"                  // Device header

void SystickDelayMs(int delay);

int main()
{
RCC->AHB1ENR = 1;
GPIOA->MODER |= 0x0400;

while (1)
{
	GPIOA->ODR = 0x020;
	SystickDelayMs(5000);
	GPIOA->ODR &=~ 0x020;
	SystickDelayMs(5000);
}
	
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


