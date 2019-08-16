#ifndef __UART_H__
#define	__UART_H__

#include "stdint.h"

void UART0_Init(void);

uint8_t UART0_InChar_Read(void);

void UART0_OutChar_Write(uint8_t data);

#endif // __UART_H__
