#include "tm4c123gh6pm.h"
#include "PLL.h"
#include "UART.h"
#include "stdint.h"
void RGB_LED_Init(void)
{
		volatile unsigned long delay;
	//1-initialize clock of portF
	SYSCTL_RCGCGPIO_R |=0x20;
	//2-wait until clock is reached
		 delay=SYSCTL_RCGCGPIO_R;
	//commit and lock Register
	GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;   			  
  GPIO_PORTF_CR_R |= 0x0E;    //PF4,PF3,PF2
	//3-disable Aleternativefn
		GPIO_PORTF_AFSEL_R=0;
		GPIO_PORTF_PCTL_R=0;
	//4-configure DEN and AMSEL (for analog and digital) for PF4
		GPIO_PORTF_AMSEL_R &=~0x0E;
		GPIO_PORTF_DEN_R |=0x0E;
	
	//5-configure DIR
		GPIO_PORTF_DIR_R |=0x0E;
}

void SystemInit(void){
	PLL_Init();
	UART0_Init();
	RGB_LED_Init();
}

int main(void){
	uint8_t in[40];

	while(1){
	UART_receiveString(in);
		if (in[0] == 'r' && in[1] == 'e' && in[2] == 'd' && in[3] == 'o' && in[4] == 'n')
		{
				GPIO_PORTF_DATA_R = 0x02;
		}
		else if (in[0] == 'b' && in[1] == 'l' && in[2] == 'u' && in[3] == 'e' && in[4] == 'o' && in[5] == 'n')
		{
				GPIO_PORTF_DATA_R = 0x04;
		}
		else if (in[0] == 'g' && in[1] == 'r' && in[2] == 'e' && in[3] == 'e' && in[4] == 'n' && in[5] == 'o' && in[6] == 'n')
		{
				GPIO_PORTF_DATA_R = 0x08;
		}
		else if (in[0] == 'r' && in[1] == 'e' && in[2] == 's' && in[3] == 'e' && in[4] == 't')
		{
				GPIO_PORTF_DATA_R = 0;
		}
		else if(in[0]=='w' && in[1]=='h' && in[2]=='i' && in[3]=='t' && in[4]=='e' && in[5]=='o' && in[6]=='n')
		{
			GPIO_PORTF_DATA_R =0x0E;
		}
	}
}