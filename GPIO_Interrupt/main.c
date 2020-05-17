#include "stm32f4xx.h"                  // Device header

//LED: PA5
//Button: PC13

void SystickDelayMs(int delay);

int counter = 0;

int main()
{
	__disable_irq();
	
	RCC->AHB1ENR |= 1;   			 // enable clock for port A
	RCC->AHB1ENR |= 4;	 			// enable clock for port C
	RCC->APB2ENR |= 0x4000;	 // enable SYSCF clock
	
	GPIOA->MODER |= 0x400;				// set PA5 to output (LED), note PC13 is already input because it is set to 0
	SYSCFG->EXTICR[3] |= 0x0020; //configure PC13 as external interrupt
	EXTI->IMR |= 0x2000;        // unmask EXTI13
	EXTI->FTSR |= 0x2000;   	 // select falling edge trigger - because button is pulled up initially so we look for a fall edge (inter. condition)
	
	NVIC_EnableIRQ(EXTI15_10_IRQn);
	
	__enable_irq();

	while(1){}
}

void EXTI15_10_IRQHandler(void)  // callback function, recognized by the compiler: the code that runs when you interrupt
{
	GPIOA->ODR |= 0x20;// toggle led on
	SystickDelayMs(300);
	GPIOA->ODR &=~ 0x20;// toggle led off
	SystickDelayMs(300);
	GPIOA->ODR |= 0x20;// toggle led on
	SystickDelayMs(300);
	GPIOA->ODR &=~ 0x20;// toggle led off
	
	counter++;
	
	EXTI->PR = 0x2000;		// clear interrupt flag
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
