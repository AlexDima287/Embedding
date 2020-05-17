#include "stm32f4xx.h" 								// Device header

//PA3 is RX for USART
//PA2 is TX for USART


void USART_Init(void);
char USART_Read(void);
void USART_Write(char ch);

int main()
{
char ch;
USART_Init();
	
while (1)
{

ch = USART_Read();
USART_Write(ch);
	
}
	
}

void USART_Init(void)
{
	// General Setup
	
RCC->APB1ENR |= 0x020000;    // clock enable for USART2
RCC->AHB1ENR |= 1;          // clock enable for GPIOA
USART2->CR1   = 0x2000;    // UART enable
USART2->BRR   = 0x0683;   // 9600 @ 16MHz

	// TX setup: PA2
	
GPIOA->MODER |= 0x020;    // makes GPIO PA2 Alternate function
GPIOA->AFR[0]|= 0x0700;  // makes GPIO PA2 the USART AF (AF7)
USART2->CR1  |= 0x008;  // Transmitter enable

	// RX setup: PA3
	
GPIOA->MODER |= 0x080;    // makes GPIO PA3 Alternate function
GPIOA->AFR[0]|= 0x7000;  // makes GPIO PA3 the USART AF (AF7)
USART2->CR1  |= 0x004;  // Reciever enable
	
}

char USART_Read(void)
{
while (!(USART2->SR & 0x020))
{}
return USART2->DR;
	
}

void USART_Write(char ch)
{
while (!(USART2->SR & 0x080))
{}
USART2->DR = ch;
}

