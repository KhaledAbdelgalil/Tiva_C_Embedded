#include "stdint.h"
#include "tm4c123gh6pm.h"
#include "PLL.h"
uint32_t cnt1ms =0;
uint8_t flag_led=0x00;
void EnableInterrupts(void);
void WaitForInterrupts(void);
void DisableInterrupts(void);
void RGB_LED_Init(void);
void SysTick_Init(void);
void SysTick_Handler(void);


void SystemInit(void)
{
	PLL_Init();
	RGB_LED_Init();
	SysTick_Init();
	EnableInterrupts();
	DisableInterrupts();
	__enable_irq();//intrinsic fn known by compiler

}
int main(void)
{
	
	while(1){
		WaitForInterrupts();//to save power it says to CPU sleep and don't do anything and I promise I will wake you when interrupt arrives it's written in assembly :WFI, BX LR
		GPIO_PORTF_DATA_R=flag_led;
		if(cnt1ms==100)
		{
			switch(flag_led)
			{
				case 0: flag_led=0x02;break;
				case 0x02: flag_led=0x04; break;
				case 0x04: flag_led=0x08;break;
				case 0x08: flag_led=0x0E; break;
				case 0x0E: flag_led=0x00;break;
			}
		cnt1ms=0;
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




void SysTick_Init(void)
{
	NVIC_ST_CTRL_R = 0;         // disable SysTick during setup
	NVIC_ST_RELOAD_R = 800000 - 1;// reload value it will count for 10ms
	NVIC_ST_CURRENT_R = 0;      // any write to current clears it
	NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x20000000; // priority 1      
	NVIC_ST_CTRL_R = 0x0007;  // enable SysTick with core clock and interrupts
}

void SysTick_Handler(void)
{
	cnt1ms++;//to count sec it must count 100
}