#include "stm32f4xx.h"                  // Device header


void USART_Init(void);
void delayMs(int delay);
void LED_Play(int value);
char USART2_Read(void);


int main()
{
char ch;
	
USART_Init();
while (1)
{
	ch = USART2_Read();
	LED_Play(ch);

}
}

void USART_Init(void)
{
RCC->AHB1ENR = 1;         //GPIOA clock
RCC->APB1ENR = 0x20000;  //USART2 clock

// Configure PA3 as USART RX
	
GPIOA->MODER |= 0x080;       // enable PA3 as Alternate Function
GPIOA->AFR[0]|= 0x07000;    // enables alternate function low register for USART on GPIOA3
USART2->BRR   = 0x008B;    // 115 200 bps @ 16 MHz
USART2->CR1   = 0x04;     // enables reception
USART2->CR1  |= 0x2000;  // enables UART
	
// Configure GPIO PA5 for output (LED)
	
GPIOA->MODER |= 0x0400; // sets PA5 to general purpose output mode
	
}

char USART2_Read(void)
{
	
while	(!(USART2->SR & 0x020))
{} // wait until character arrives
	
return USART2->DR;
	
}


void LED_Play(int value)
{

value %= 16;
	for (;value>0; value--)
	{
		GPIOA->ODR |= 0x0020;  // turn on LED  0b   0000 0000 0000 0000 0000 0000 0010 0000
		delayMs(100);
		GPIOA->ODR &=~ 0x0020; // turn off LED
		delayMs(100);
	}
}

void delayMs(int delay)
{
int i;
for (; delay>0;delay--)
{
	for (i=0;i<3195;i++)
	{}

}
}


