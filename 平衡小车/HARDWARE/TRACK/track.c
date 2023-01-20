#include "track.h"


void Track_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//¿ªÆôÊ±ÖÓ
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);	
	
}



uint8_t Track_R(void)
{
	return GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4);
}

uint8_t Track_L(void)
{
	return GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8);
}


