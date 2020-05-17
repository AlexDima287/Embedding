//led is connected to PA5
//AHB1 is the bus that GPIO port A is connected to
//RCC_AHB1ENR is the register for enabling the clock for GPIO ports
// Access the bus with RCC->AHB1ENR b/c RCC is a structure with the bus as its member
//GPIOx_MODER is the configuration register
//GPIOx_ODR is the output data register

#include "stm32f4xx.h"    // Device header

void delayMs(int delay);

int main()
{
	
	// enable clock access to port A:
	
		RCC->AHB1ENR |= 1; // this means we have set the RCC register to 00000000000000000000000001 (31 0's and a 1)
	
		GPIOA->MODER |= 0x400; // in binary: 0b  0000 0000 0000 0000 0000 0100 0000 0000
													// since it takes 2 inputs, the rightmost 2 bits are PA0, the next are PA1, etc.
	
	while (1)
	{
		
		GPIOA->ODR |= 0x20; //set pin 5 to high   0b 0010 0000
		delayMs(1000);
		GPIOA->ODR &=~ 0x20; //set pin 5 to low   0b 0000 0000
		delayMs(1000);
		
	}
}


void delayMs(int delay)
{
	volatile int i;

	for (;delay>0;delay--){
		
		for (i=0;i<3195;i++){
			
}
}
}