#include "stm32f4xx.h"                  // Device header

// USART2 is connected to APB1 on bit 17 (clock)
// USART is connected to PA2


void USART2_Init(void);
void USART_Write(char ch);
void delayMS(int delay);
	
int main()
{
	USART2_Init();
	while (1)
	{
	USART_Write('H');
	USART_Write('i');
	USART_Write('\r');
	USART_Write('\n');
	delayMS(100);
	}
	

}


void USART2_Init(void)
{
RCC->APB1ENR |= 0x20000;  // enables the clock for USART2
RCC->AHB1ENR |= 1;       // enables the clock for GPIO port A
	
GPIOA->AFR[0] = 0x0700;  // enable USART (AF7) on GPIO PA2
GPIOA->MODER |= 0x020;  // set PA2 to alternate function
USART2->BRR   = 0x0683;   // Baud rate of 9600 (taking into account the system is at 16MHz)
USART2->CR1   = 0x0008;  // access CR1 register of USART and set TE to high - enable TX
USART2->CR1  |= 0x2000;
}

void USART_Write(char ch)
{
while (!(USART2->SR & 0x0080)) //Using status register to check TXE (Transmit data register empty)
{}                            // wait while 	TX buffer is empty
USART2->DR = (ch & 0xFF);

}


void delayMS(int delay)
{
int i;
	
for (; delay>0;delay--)
	{
		for (i=0;i<3195;i++)
		{
		
		}

	}

}
