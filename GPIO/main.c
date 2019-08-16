#include "stdint.h"
#include "tm4c123gh6pm.h"

void SW_Init(void)
{
	
	//1-initialize clock of portF
	SYSCTL_RCGCGPIO_R |=0x20;
	//2-wait until clock is reached
	while((SYSCTL_PRGPIO_R &0x20)==0){};

	//commit and lock Register
	GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;   			  
  GPIO_PORTF_CR_R |= 0x10;
	//3-disable Aleternativefn
		GPIO_PORTF_AFSEL_R=0;
		GPIO_PORTF_PCTL_R=0;
	//4-configure DEN and AMSEL (for analog and digital) for PF4
		GPIO_PORTF_AMSEL_R &=~0x10;
		GPIO_PORTF_DEN_R |=0x10;
	//5-configure PUR
		GPIO_PORTF_PUR_R |=0x10;
	//6-configure DIR
		GPIO_PORTF_DIR_R &=~0x10;
}


void LED_Init(void)
{
		volatile unsigned long delay;
	//1-initialize clock of portF
	SYSCTL_RCGCGPIO_R |=0x20;
	//2-wait until clock is reached
		 delay=SYSCTL_RCGCGPIO_R;
	//commit and lock Register
	GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;   			  
  GPIO_PORTF_CR_R |= 0x04;    
	//3-disable Aleternativefn
		GPIO_PORTF_AFSEL_R=0;
		GPIO_PORTF_PCTL_R=0;
	//4-configure DEN and AMSEL (for analog and digital) for PF2
		GPIO_PORTF_AMSEL_R &=~0x04;
		GPIO_PORTF_DEN_R |=0x04;
	
	//5-configure DIR
		GPIO_PORTF_DIR_R |=0x04;
}


void SystemInit(void)
{
	LED_Init();
	SW_Init();
}

int main(void){
	while(1)
	{
		if((GPIO_PORTF_DATA_R & 0x10)==0)//pull up switch negative logic
			GPIO_PORTF_DATA_R |=0x04;//set led
		else
			GPIO_PORTF_DATA_R &=~0x04;//clear led
			
	}
}