#include "stdint.h"
#include "tm4c123gh6pm.h"




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

void Systick_Init(void){
	NVIC_ST_CTRL_R = 0;//disable
	NVIC_ST_RELOAD_R = 0x00FFFFFF;//put max value
	NVIC_ST_CURRENT_R = 0;//to make flag=0 to start count
	NVIC_ST_CTRL_R =5;
}
void delay_ms(void)
{
	NVIC_ST_RELOAD_R=15999;//1 ms for 16 MHZ
	NVIC_ST_CURRENT_R=0;
	while((NVIC_ST_CTRL_R & 0x10000)!=0x10000){};
}
void delay_1mssec(uint32_t delay)
{
	uint32_t i;
	for(i=0;i<delay;i++)
	{
		delay_ms();
	}
}
void SystemInit(void)
{
	Systick_Init();
	RGB_LED_Init();
	
}

int main(void)
{
	uint32_t i,j;
	GPIO_PORTF_DATA_R=0x04;
	
		for(i=0;i<3;i++)//repeat for 3 times
		{
			for(j=0;j<10;j++)
					delay_1mssec((uint32_t)1000);//wait 10 sec
			GPIO_PORTF_DATA_R=0x02;
				
			for(j=0;j<10;j++)
					delay_1mssec((uint32_t)1000);//wait 10 sec
			GPIO_PORTF_DATA_R=~0x02;
			
			for(j=0;j<10;j++)
					delay_1mssec((uint32_t)1000);//wait 10 sec
		}
	
	GPIO_PORTF_DATA_R=0x08;
}