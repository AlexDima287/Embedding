#include "stm32f4xx.h"                  // Device header
#include "stdio.h" 


// LIS3DH at high reolution mode, 400 kHz
// PB8 SCL, PB9 SDA

// SLAVE ADDRESSES - slave 1 has SDO pad connected to ground, slave 2 has 1k external pullup to 3V3
const char SLAVE_ADDR1 = 0x18; // put the 8-bit slave addresses here, in 8 bit format (code accounts for r/w bit)
const char SLAVE_ADDR2 = 0x19;

// Slave control registers
const char CTRL_REG1 = 0x20;
const char CTRL_REG2 = 0x21;
const char CTRL_REG3 = 0x22;
const char CTRL_REG4 = 0x23;
const char CTRL_REG5 = 0x24;
const char CTRL_REG6 = 0x25;

// Slave status register
const char STATUS_REG = 0x27;

// Slave output registers
const char OUT_X_L = 0x28;
const char OUT_X_H = 0x29;
const char OUT_Y_L = 0x2A;
const char OUT_Y_H = 0x2B;
const char OUT_Z_L = 0x2C;
const char OUT_Z_H = 0x2D;

// FUNCTIONS
void I2C_Init(void); // initialize I2C1 on stm32f410rb
void USART_Init(void); // initialize USART2 on stm32f410rb
int USART2_Write(int ch); // USART TX

uint8_t I2C_Read_One_Byte(char slave_add, char reg_add); // Read one Byte from slave
void I2C_Write_One_Byte(char slave_add, char reg_add, char input); // Write one byte to slave
	
void SystickDelayMs(int delay); // delay in ms

// globals
volatile int tmp;

volatile int8_t data1_x_l;
volatile int8_t data1_y_l;
volatile int8_t data1_z_l;
volatile int8_t data1_x_h;
volatile int8_t data1_y_h;
volatile int8_t data1_z_h;

volatile int16_t data1_x;
volatile int16_t data1_y;
volatile int16_t data1_z;

volatile int8_t data2_x_l;
volatile int8_t data2_y_l;
volatile int8_t data2_z_l;
volatile int8_t data2_x_h;
volatile int8_t data2_y_h;
volatile int8_t data2_z_h;

volatile int16_t data2_x;
volatile int16_t data2_y;
volatile int16_t data2_z;


// MAIN 
int main()
{
	
	uint8_t test1;
	uint8_t test2;
	uint16_t count = 1;
	
	I2C_Init();
	USART_Init();
	SystickDelayMs(10000);
	
	// device 1 config
	I2C_Write_One_Byte(SLAVE_ADDR1, CTRL_REG4, 0x08); // enable High Res, Little endian, continuous update, FS = +/-2g, block update
	I2C_Write_One_Byte(SLAVE_ADDR1, CTRL_REG1, 0x67); // take device out of power down, and ODR = 200 Hz, all axes enabled
	
	
	// device 2 config
	I2C_Write_One_Byte(SLAVE_ADDR2, CTRL_REG4, 0x08); // enable High Res, Little endian, continuous update, FS = +/-2g, block update
	I2C_Write_One_Byte(SLAVE_ADDR2, CTRL_REG1, 0x67); // take device out of power down, and ODR = 200 Hz, all axes enabled
	
	
	while (1){
	
	// read slave 1
	test1 = I2C_Read_One_Byte(SLAVE_ADDR1, STATUS_REG);
	
	while (!(test1 & 0x08)){} // wait until X, Y, and Z new data available for slave 1
		


	
	
	data1_x_l = I2C_Read_One_Byte(SLAVE_ADDR1, OUT_X_L);
	data1_x_h = I2C_Read_One_Byte(SLAVE_ADDR1, OUT_X_H);
	data1_y_l = I2C_Read_One_Byte(SLAVE_ADDR1, OUT_Y_L);
	data1_y_h = I2C_Read_One_Byte(SLAVE_ADDR1, OUT_Y_H);
	data1_z_l = I2C_Read_One_Byte(SLAVE_ADDR1, OUT_Z_L);
	data1_z_h = I2C_Read_One_Byte(SLAVE_ADDR1, OUT_Z_H);
		
		
  data1_x = (data1_x_h << 8) | (data1_x_l & 0xff);
	data1_y = (data1_y_h << 8) | (data1_y_l & 0xff);
  data1_z = (data1_z_h << 8) | (data1_z_l & 0xff);
		
	// read slave 2
		
	test2 = I2C_Read_One_Byte(SLAVE_ADDR2, STATUS_REG);
	while (!(test2 & 0x08)){} // wait until X, Y, and Z new data available for slave 2
		
	data2_x_l = I2C_Read_One_Byte(SLAVE_ADDR2, OUT_X_L);
	data2_x_h = I2C_Read_One_Byte(SLAVE_ADDR2, OUT_X_H);
	data2_y_l = I2C_Read_One_Byte(SLAVE_ADDR2, OUT_Y_L);
	data2_y_h = I2C_Read_One_Byte(SLAVE_ADDR2, OUT_Y_H);
	data2_z_l = I2C_Read_One_Byte(SLAVE_ADDR2, OUT_Z_L);
	data2_z_h = I2C_Read_One_Byte(SLAVE_ADDR2, OUT_Z_H);
		
		
  data2_x = (data2_x_h << 8) | (data2_x_l & 0xff);
	data2_y = (data2_y_h << 8) | (data2_y_l & 0xff);
  data2_z = (data2_z_h << 8) | (data2_z_l & 0xff);
		
		

	// print statement format: (slave [FIRST]) (sample number) (X1 data) (Y1 data) (Z1 data) (slave [SECOND]) (X2 data) (Y2 data) (Z2 data)
		
	printf("F %d %d %d %d S %d %d %d\r\n", count, data1_x, data1_y, data1_z, data2_x, data2_y, data2_z);

	I2C_Write_One_Byte(SLAVE_ADDR1, STATUS_REG, 0x0);
		
	count++;

	if (count >65000){
	break;}
	
	}
	
	
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
	I2C1->CCR = 0b1100000000001101; // fast mode, 16/9 duty, 625kHz low period SCL
	I2C1->TRISE = 6; // max rise time
	I2C1->CR1 |= 0x400; // enable acks
	I2C1->CR1 |= 0x1; 
}


uint8_t I2C_Read_One_Byte(char slave_add, char reg_add){ // read one byte from slave
	// all addresses as arguments are input as 8 bit format

	
	while (I2C1->SR2 & 2){} // make sure bus is not busy
	
	I2C1->CR1 |= 0x100; // generate start
		
	while (!(I2C1->SR1 & 1)){} // wait for start flag to be raised
		
	
	I2C1->DR = (slave_add << 1); //put the slave address into the data register, reading

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
		
		
	I2C1->CR1 |= 0x200; //generate stop
	
	while (!(I2C1->CR1 & 0x200)){} // wait for stop
		
	return I2C1->DR;
		
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
		
		
	I2C1->CR1 |= 0x200; //generate stop
	
	while (!(I2C1->CR1 & 0x200)){} // wait for stop
		
}

void USART_Init(void)
{
	// General Setup
	
RCC->APB1ENR |= 0x020000;    // clock enable for USART2
RCC->AHB1ENR |= 1;          // clock enable for GPIOA
USART2->CR1   = 0x2000;    // UART enable
USART2->BRR   =	0x008B;   // 115 200 baud rate @ 16MHz

	// TX setup: PA2
	
GPIOA->MODER |= 0x020;    // makes GPIO PA2 Alternate function
GPIOA->AFR[0]|= 0x0700;  // makes GPIO PA2 the USART AF (AF7)
USART2->CR1  |= 0x008;  // Transmitter enable

	
}


struct __FILE{int handle;};
FILE __stdout = {1};

int fputc(int c, FILE *f)
{
	return USART2_Write(c);
}
	
int USART2_Write(int ch)
{
	while (!(USART2->SR & 0x0080)){}
	USART2->DR = ch;
	return ch;
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
