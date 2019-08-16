#include "tm4c123gh6pm.h"
#include "stdint.h"

#define GPIO_PA10_M 0x03

void UART0_Init(void)
{
	//1-initialize clk of uart0
	SYSCTL_RCGCUART_R |= 0x01;
	//2-initialize clk of port used in our case it's PORTA
	SYSCTL_RCGCGPIO_R |= 0x01;
	//3-disable the UART0
	UART0_CTL_R &= ~0x01;
	
	//4-configure the paramters(BaudRate-......)
	UART0_IBRD_R = 520;    
  UART0_FBRD_R = 53;
	UART0_LCRH_R = 0x70;
	UART0_CTL_R |= 0x01;
	
	//5-configure the PORT to be UART alternative fn
	GPIO_PORTA_AFSEL_R |= 0x03;
	GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R&0xFFFFFF00) | 0x11;
	GPIO_PORTA_DEN_R |= 0x03;
}



uint8_t UART0_InChar_Read(void)
{
	//check whether FiFo is empty or not if not empty read data
		while((UART0_FR_R &0x0010)!=0){};
		return (uint8_t)(UART0_DR_R & 0xFF);
}

void UART0_OutChar_Write(uint8_t data)
{
	//check if Fifo have a place to write in it
		while((UART0_FR_R &0x0020)!=0){};
		UART0_DR_R =data;
}
