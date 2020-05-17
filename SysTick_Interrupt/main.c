#include "stm32f4xx.h"                  // Device header

// configure SysTick interrupt to toggle LED at 1Hz, clock is 16MHz


int main()
{
	__disable_irq();
	
	RCC->AHB1ENR |= 1;
	GPIOA->MODER |= 0x400;
	
	SysTick->LOAD = 16000000-1; // 1Hz
	SysTick->VAL = 0;
	SysTick->CTRL =7;
	
	__enable_irq();

	while (1){}
}

void SysTick_Handler(void)
{
	GPIOA->ODR ^= 0x20;
	
}