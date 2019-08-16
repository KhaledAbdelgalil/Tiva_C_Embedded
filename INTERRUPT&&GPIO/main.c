#include "tm4c123gh6pm.h"
#include <stdint.h>
#include "PLL.h"
uint8_t i=0;
void 	WaitForInterrupts(void);
void DisableInterrupts(void);
void EnableInterrupts(void);
void SW_Init(void);
void GPIOF_Handler(void);
void RGB_LED_Init(void);

void SystemInit(void)
{
	SW_Init();
	RGB_LED_Init();
}

int main(void)
{
 GPIO_PORTF_DATA_R=0;
 while(1)
 {
	WaitForInterrupts();
	 switch(i)
	 {
		 case 0: GPIO_PORTF_DATA_R=0x02;break;
		 case 1: GPIO_PORTF_DATA_R=0x04;break;
		 case 2: GPIO_PORTF_DATA_R=0x08;break;
		 case 3: GPIO_PORTF_DATA_R=0x0E;break;
		 case 4: GPIO_PORTF_DATA_R=0x0A;break;
	 }
 }
}

void GPIOF_Handler(void)
{
	if((GPIO_PORTF_RIS_R & 0x0010)==0x0010)//if interrupt from PF4 not neccessary in this code to do that
	{
		GPIO_PORTF_ICR_R |= 0x10;
		//while((GPIO_PORTF_DATA_R &0x10)==0x01){};
		i = (i + 1) % 5;
	}
}

void SW_Init(void)
{
	
	//1-configuration of pins
		SYSCTL_RCGCGPIO_R |=0x20;
		while((SYSCTL_PRGPIO_R &0x20)==0){};
		//GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;   			  
		//GPIO_PORTF_CR_R |= 0x10;
		GPIO_PORTF_DIR_R &=~0x10;
		GPIO_PORTF_AFSEL_R=0;
		GPIO_PORTF_PCTL_R=0;
		GPIO_PORTF_AMSEL_R &=~0x10;
		GPIO_PORTF_DEN_R |=0x10;
		GPIO_PORTF_PUR_R |=0x10;
	/*----------------additional configuration to interrupts-------------------*/
		GPIO_PORTF_IS_R &= ~0x10;//sense edge not level
		GPIO_PORTF_IBE_R &= ~0x10;//sense single edge either falling or rising edge
		GPIO_PORTF_IEV_R &= ~0x10;//sense falling edge
		GPIO_PORTF_ICR_R |= 0x10;//clear flag
		GPIO_PORTF_IM_R |= 0x10;	//ok I am ready to listen to this interrupt
	//2-configuration of NVIC
		NVIC_PRI7_R = (NVIC_PRI7_R & 0xFF00FFFF) | 0xFFA0FFFF; // PRI7 bits 23:21
		NVIC_EN0_R=0x40000000;//enable portF_interrupt
		
	//3-global enable for interrupts
		EnableInterrupts();
			
		
}
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
