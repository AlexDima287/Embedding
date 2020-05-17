#include "stm32f4xx.h"                  // Device header
// TIM5 is AF2 on AFR[0]
// this doesn't work right now, not sure why
int main()
{
	RCC->AHB1ENR |= 1;
	
	GPIOA->MODER |= 0x800; // set PA5 to alternate function mode
	GPIOA->AFR[0] |= 0x200000; // set PA5 to AF2-TIM5
	
	RCC->APB1ENR |= 8;		//
	TIM5->PSC = 1600-1;		// dividing the CPU clock to get a 1 Hz frequency
	TIM5->ARR = 10000-1;	//
	
	
	TIM5->CCMR1 |= 0x30;  // set output to toggle on match
	TIM5->CCR1 = 0;			// set match mode - condition for the down count
	TIM5->CCER |= 1;		//enable ch1 compare mode
	
	
	TIM5->CNT = 0;		// Clear the counter
	TIM5->CR1 = 1;		// Enable TIM5
	
	while (1)
	{
	
	}
}
