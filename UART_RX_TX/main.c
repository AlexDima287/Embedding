#include "stm32f4xx.h"                  // Device header
#include <stdio.h>											// standard io


void USART2_Init(void);

int main()
{
USART2_Init();
	
int n;
char str[100];
printf("Hello from the other side\r\n");
fprintf(stdout, "Test for stdout\r\n");
fprintf(stderr, "Test for stderr\r\n");

while (1)
{
	printf("How old are you?: ");
	scanf("%d", &n);
	printf("Your age is %d\r\n", n);
	printf("Enter your first name: ");
	gets(str);
	printf("I like your style: ");
	puts(str);
	printf("\r\n");
}

}
	
void USART2_Init(void)
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

int USART_Read(void)
{
while (!(USART2->SR & 0x020))
{}
return USART2->DR;
	
}

int USART_Write(int ch)
{
while (!(USART2->SR & 0x080))
{}
USART2->DR = ch;
return ch;
}

// Interfacing with the stdio, to use printf and other functions

struct __FILE{int handle;};
FILE __stdin = {0};
FILE __stdout = {1};
FILE __stderr = {2};


int fgetc(FILE *f)
{
int c;
c = USART_Read();
	if (c=='\r')
	{
		USART_Write(c);
		c = '\n';
	}	
	
USART_Write(c);
return c;
}

int fputc(int c, FILE *f)
{
	return USART_Write(c);
}

// --------------------------------------------------------------------------