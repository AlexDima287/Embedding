#include "stm32f4xx.h"                  // Device header

int main()
{
	
RCC->AHB1ENR |= 1; // 0b 0000 0000 0000 0000 0000 0000 0000 0000 0001 // enable port a
RCC->AHB1ENR |= 4; // 0b 0000 0000 0000 0000 0000 0000 0000 0000 0101 // enable port c
	
GPIOA->MODER |= 0x400;   // 0b 0000 0000 0000 0000 0000 0000 0100 0000 0000

	
	
while (1)
{
if (GPIOC->IDR == 0x00)     // 0b 0000 0000 0000 0000 0000 0010 0000 0000 0000  if PC13 low, aka B1 pressed (PU)
{
	GPIOA->ODR |= 0x20;        // 0b 0000 0000 0000 0000 0000 0000 0000 0010 0000  LED is set to hi
	continue;
}

GPIOA->ODR &= 0x00; //set LED low



}

}
