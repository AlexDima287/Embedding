#include "stm32f4xx.h"                  // Device header


int main()
{
RCC->AHB1ENR |= 1;
GPIOA->MODER = 0x400;
	
// TIM5 config
	
RCC->APB1ENR |= 8;
TIM5->PSC     = 1600-1;   // CPU frequency = 16MHZ so divide by 1600 to get counter of frequency 10 000/second
TIM5->ARR     = 10000-1; // divide 10000 bu 10000 = 1Hz frequency timer
TIM5->CNT     = 0;      // clear timer counter
TIM5->CR1     = 1;     // enable TIM5
	
while (1)
{
	while (TIM5->SR & 1)  // check if an update happened with the GPTIM
	{
		TIM5->SR &=~ 1; // clear the flag
		GPIOA->ODR ^= 0x20; // toggle led
	}


}
	
	
}