#include "tm4c123gh6pm.h"
#include <stdint.h>
#include "PLL.h"
uint8_t count=0;

void PushButton_Init(void);
void Timer3_Init(void);
void TIMER3_Handler(void);
void EnableInterrupts(void);
void RGB_LED_Init(void);
void TIMER3A_Handler(void);
void SystemInit(void)
{
}
int main(void){
  //PLL_Init();
	Timer3_Init();
	EnableInterrupts();
	RGB_LED_Init();
	GPIO_PORTF_DATA_R=0x02;
	while(1)
	{
		if(count==100)
		{
			count=0;
			GPIO_PORTF_DATA_R ^=0x02;
		}
	}
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


void Timer3_Init(void){
	volatile unsigned long delay;
	SYSCTL_RCGCTIMER_R |= 0x08; // enable clock for Timer 3 module
	delay = SYSCTL_RCGCTIMER_R;
	TIMER3_CTL_R = 0; 				// Disable timer before configuration
	TIMER3_CFG_R = 0x00; 					// Choose 32 bit timer mode
	TIMER3_TAMR_R = 0x02; 				// Periodic mode and count down timer
	TIMER3_TAILR_R = 160000-1;//clk is 16MHz no PLL used
	TIMER3_TAPR_R=0;
	//enable_interrupt
	TIMER3_ICR_R=0x00000001;
	TIMER3_IMR_R=0x00000001;
	NVIC_PRI8_R=(NVIC_PRI8_R & 0x00FFFFFF)|0x80000000;
	NVIC_EN1_R=1<<(35-32);
	TIMER3_CTL_R |= 0x01; 				// Enable timer after configuration
}

void TIMER3A_Handler(void)
{
	TIMER3_ICR_R=0x00000001;
	count++;
}


