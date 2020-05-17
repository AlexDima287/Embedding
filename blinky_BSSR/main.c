//Should actually be BSRR (Bit set reset)
#include "stm32f4xx.h"                  // Device header

int main()
{
	
	RCC->AHB1ENR |= 1;
	GPIOA-> MODER |= 0x400;

while (1)
{
	GPIOA->BSRR =0x20;  //set PA5 high    0b 0010 0000
	delayMs(100);
	
	GPIOA->BSRR = 0x200000;     //0000 0000 0010 0000 0000 0000 0000 0000  set PA5 low
	





}







}

