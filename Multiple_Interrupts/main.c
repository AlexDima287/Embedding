#include "stm32f4xx.h"                  // Device header



void SystickDelayMs(int delay);

volatile int pb10_counter = 0;
volatile int pc13_counter = 0;

int main()
{
	__disable_irq();
	
	RCC->AHB1ENR |= 7; // enable clock for port a/b/c
	RCC->APB2ENR |= 0x4000; // enable clock for SYSCFG
	
	GPIOA->MODER |= 0x400; // set PA5 general output
	
	SYSCFG->EXTICR[3] |= 0x0020; // select port C for EXTI13
	EXTI->IMR |= 0x2000;  // unmask EXTI13
	EXTI->FTSR |= 0x2000; // enable falling edge trigger for EXTI13
	
	GPIOB->PUPDR |= 0x100000; // enable pull-up resistor for PB10
	
	SYSCFG->EXTICR[2] |= 0x0100; // select port B for EXT10
	EXTI->IMR |= 0x0400; // unmask EXTI10
	EXTI->FTSR |= 0x0400; // select falling edge trigger for EXTI10
	
	NVIC_EnableIRQ(EXTI15_10_IRQn);
	
	__enable_irq();
	
	
	while (1){}

}

void EXTI15_10_IRQHandler(void)
{
	if (EXTI->PR == 0x2000)		// if PC13 falling edge trigger is detected
	{
		GPIOA->ODR |= 0x20;
		SystickDelayMs(200);
		GPIOA->ODR &=~ 0x20;
		pc13_counter++;
		EXTI->PR = 0x2000;   // clears both interrupt pending flags for PC13 and PB10
	}
	else if (EXTI->PR == 0x0400) // if PB10 falling edge trigger is detected
	{
		GPIOA->ODR |= 0x20;
		SystickDelayMs(200);
		GPIOA->ODR &=~ 0x20;
		SystickDelayMs(200);
		GPIOA->ODR |= 0x20;
		SystickDelayMs(200);
		GPIOA->ODR &=~ 0x20;
	
		pb10_counter++;
		EXTI->PR = 0x0400;   // clears both interrupt pending flags for PC13 and PB10
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

	
