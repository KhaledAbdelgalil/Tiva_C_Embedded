#include "tm4c123gh6pm.h"
#include "PLL.h"
#include "UART.h"
#include "stdint.h"

void SystemInit(void){
	PLL_Init();
	UART0_Init();
}

int main(void){
	uint8_t in;
	uint8_t out;
	while(1){
		in = UART0_InChar_Read();
		out = in - 0x20;
		UART0_OutChar_Write(out);
	}
}
