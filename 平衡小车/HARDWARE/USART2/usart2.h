#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 


void uart2_init(u32 bound);					
void USART2_IRQHandler(void);   
void USATR2_Send_Data(char data);
void USATR2_Send_String(char *String);

#endif


