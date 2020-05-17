#include "stm32f4xx.h"                  // Device header

// Doesn't work, I think there is a problem with the Tim5 setup


int main()
{
	RCC->AHB1ENR |= 1;
	GPIOA->MODER |= 0x00000800; // set PA5 to alternate function
	GPIOA->AFR[0] |= 0x0200000;	// set PA5 to AF2
	
	// enable TIM5
	
	RCC->APB1ENR |= 0x08; // enable clock for TIM5
	TIM5->PSC = 10-1;
	TIM5->ARR = 26667-1;  // makes a 60 Hz timer
	
	TIM5->CNT = 0;  				// resets the counter value
	TIM5->CCMR1 |= 0x04;    // enable PWM mode, ch1 is configured as OUTPUT
	TIM5->CCER |= 1;  			// enable ch1, since ch1 is configured as output
	TIM5->CCR1 = 8889-1;    // set pulse width to 1/3 the period
	TIM5->CR1 =1;           // enable timer's counter
	
	
	while (1)
	{}
	
}
