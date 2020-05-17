#include "stm32f4xx.h"                  // Device header
// SCL - PB8
// SDA - PB9

// SLAVE ADDRESS
const char SLAVE_ADDR = 0x18; // put the 8-bit slave address here, in 8 bit format (code accounts for r/w bit)

// FUNCTIONS
void I2C_Init(void); // initialize I2C on stm32f410rb

char I2C_Read_One_Byte(char slave_add, char reg_add, char *data);
void I2C_Write_One_Byte(char slave_add, char reg_add, char input);
int I2C_Read_More_Bytes(char slave_add, char reg_add, char *data, int num_bytes);
	
void SystickDelayMs(int delay);

volatile int tmp;


// MAIN 
int main()
{
	char data;
	
	I2C_Init();
	SystickDelayMs(3000);
	I2C_Read_More_Bytes(SLAVE_ADDR, 0x28, &data, 2);

	
}


void I2C_Init(void) // initializing the I2C driver
{
	RCC->AHB1ENR |= 2;	// enable clock access to GPIO port B
	RCC->APB1ENR |= 0x00200000; // enable clock access to I2C
	
	GPIOB->MODER &=~ 0xF0000; // deselect the mode
	GPIOB->MODER |= 0xA0000; // set both PB8 AND PB9 to alternate function mode
	
	GPIOB->AFR[1] &=~ 0xFF;	// deselect the mode
	GPIOB->AFR[1] |= 0x44;	// set PB8 and PB9 to AF4-I2C
	
	GPIOB->OTYPER |= 0x300; // configure PB8 and PB9 to open drain
	
	GPIOB->PUPDR &=~ 0xF0000; // deselect the mode
	GPIOB->PUPDR |= 0x50000;  // enable internal pull ups for PB8, PB9
	
	// I2C config
	
	I2C1->CR1 = 0x8000; // software reset I2C
	I2C1->CR1 &=~ 0x8000; // exit reset state
	
	I2C1->CR2 |= 0x10; // set I2C to 16MHz which is our clock frequency
	I2C1->CCR = 80; // standard mode, 100kHz SCL
	I2C1->TRISE = 17; // max rise time
	
	I2C1->CR1 |= 0x400; // enable acks
	I2C1->CR1 |= 0x1; // enable peripheral
}


char I2C_Read_One_Byte(char slave_add, char reg_add, char *data){ // read one byte from slave
	// all addresses as arguments are input as 8 bit format

	
	while (I2C1->SR2 & 2){} // make sure bus is not busy
	
	I2C1->CR1 |= 0x100; // generate start
		
	while (!(I2C1->SR1 & 1)){} // wait for start flag to be raised
		
	
	I2C1->DR = slave_add << 1; //put the slave address into the data register, transmitting

	while (!(I2C1->SR1 & 2)){} // waiting for the acknowledgement bit (end of transmission)
		
	tmp = I2C1->SR2; // clears the ADDR flag
	
	while(!(I2C1->SR1 &0x80)){} // waiting for the data register to be empty 
	
	
	
	I2C1->DR = reg_add; //put one of the slaves internal register address into the data register
	
		
	while(!(I2C1->SR1 &0x80)){} // waiting for the data register to be empty 
		
		
	
	I2C1->CR1 |= 0x100; // generate repeated start
		
	while (!(I2C1->SR1 & 1)){} // wait for start flag to be raised
		
		
	I2C1->DR = (slave_add << 1) + 1; //put the slave address into the data register, recieving

	while (!(I2C1->SR1 & 2)){} // waiting for the acknowledgement bit (end of transmission)
		
	tmp = I2C1->SR2; // clears the ADDR flag
	
		
	while (!(I2C1->SR1 & 0x40)){} // waiting for data register to fill up
		
	*data = I2C1->DR;
		
	I2C1->CR1 |= 0x200; //generate stop
	
	while (!(I2C1->CR1 & 0x400)){}// wait for stop
		
	return (*data);
	
}

void I2C_Write_One_Byte(char slave_add, char reg_add, char input){
		// all addresses as arguments are input as 8 bit format

	while (I2C1->SR2 & 2){} // make sure bus is not busy
	
	I2C1->CR1 |= 0x100; // generate start
		
	while (!(I2C1->SR1 & 1)){} // wait for start flag to be raised
		
	
	I2C1->DR = slave_add << 1; //put the slave address into the data register, transmitting

	while (!(I2C1->SR1 & 2)){} // waiting for the acknowledgement bit (end of transmission)
		
	tmp = I2C1->SR2; // clears the ADDR flag
	
	while(!(I2C1->SR1 &0x80)){} // waiting for the data register to be empty 
	
	
	
	I2C1->DR = reg_add; //put one of the slaves internal register address into the data register
		
	while(!(I2C1->SR1 &0x80)){} // waiting for the data register to be empty 
		
		
	I2C1->DR = input; // write data
	
	while(!(I2C1->SR1 &0x80)){} // waiting for the data register to be empty 
		
	I2C1->DR = 0xFF; // write data
	
	while(!(I2C1->SR1 &0x80)){} // waiting for the data register to be empty 
		
		
	I2C1->CR1 |= 0x200; //generate stop
	
	while (!(I2C1->CR1 & 0x200)){}// wait for stop
		
}

int I2C_Read_More_Bytes(char slave_add, char reg_add, char *data, int num_bytes){
// all addresses as arguments are input as 8 bit format

	
	while (I2C1->SR2 & 2){} // make sure bus is not busy
	
	I2C1->CR1 |= 0x100; // generate start
		
	while (!(I2C1->SR1 & 1)){} // wait for start flag to be raised
		
	
	I2C1->DR = slave_add << 1; //put the slave address into the data register, transmitting

	while (!(I2C1->SR1 & 2)){} // waiting for the acknowledgement bit (end of transmission)
		
	tmp = I2C1->SR2; // clears the ADDR flag
	
	while(!(I2C1->SR1 &0x80)){} // waiting for the data register to be empty 
	
	
	
	I2C1->DR = reg_add; //put one of the slaves internal register address into the data register, assert MSB for auto increment
	
		
	while(!(I2C1->SR1 &0x80)){} // waiting for the data register to be empty 
		
		
	
	I2C1->CR1 |= 0x100; // generate repeated start
		
	while (!(I2C1->SR1 & 1)){} // wait for start flag to be raised
		
		
	I2C1->DR = (slave_add << 1) + 1; //put the slave address into the data register, recieving

	while (!(I2C1->SR1 & 2)){} // waiting for the acknowledgement bit (end of transmission)
		
	tmp = I2C1->SR2; // clears the ADDR flag
	
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//reading bytes using auto increment
		
	for(;num_bytes>0;num_bytes--)
	{
	while (!(I2C1->SR1 & 0x40)){} // waiting for data register to fill up
		
	*data = I2C1->DR;
		
	}
		
	I2C1->CR1 |= 0x200; //generate stop
	
	while (!(I2C1->CR1 & 0x400)){} // wait for stop
	
	
	
	
	
	return 0;



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
