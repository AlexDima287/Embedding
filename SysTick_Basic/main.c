#include "stm32f4xx.h"                  // Device header


int main()
{
	RCC->AHB1ENR |= 1;
	GPIOA->MODER |= 0x400;
	
	// configure SysTick to generate 400 ms delay
	
	SysTick->LOAD |= 6400000 -1;  // set amount of CYCLES
	SysTick->VAL   = 0;						// clear the value register
	SysTick->CTRL  = 5;						// enable the counter, and choose the clock (AHB)
	
	while (1)
	{
		// if the count flag is set
		
		if (SysTick->CTRL & 0x10000)
		{
			GPIOA->ODR ^= 0x20;
		
		
		}
	
	
	}

}