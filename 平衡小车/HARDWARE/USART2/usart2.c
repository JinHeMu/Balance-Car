#include "usart2.h"	  





void uart2_init(u32 bound)
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	USART_DeInit(USART2);
	//USART2_TX   PA.2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//USART2_RX	  PA.3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);  
	//USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
	
	
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�����ж�
	USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ��� 

	
}

/*
0x00:stop
0x01:run
0x05:back
0x03:turn left
0x07:turn right
*/

u8 Fore,Back,Left,Right,Mode;

void USART2_IRQHandler(void)                	//����2�жϷ������
	{
		int Bluetooth_data;

		
		if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //�����ж�
		{
		Bluetooth_data = USART_ReceiveData(USART2);//(USART2->DR);	//��ȡ���յ�������
		
		if(Bluetooth_data==0x00)Fore=0,Back=0,Left=0,Right=0;//ɲ
		else if(Bluetooth_data==0x05)Fore=1,Back=0,Left=0,Right=0;//ǰ
		else if(Bluetooth_data==0x01)Fore=0,Back=1,Left=0,Right=0;//��
		else if(Bluetooth_data==0x07)Fore=0,Back=0,Left=1,Right=0;//��
		else if(Bluetooth_data==0x03)Fore=0,Back=0,Left=0,Right=1;//��
		else if(Bluetooth_data==0x09)Mode=1;
		else if(Bluetooth_data==0x011)Mode=2;
		else if(Bluetooth_data==0x013)Mode=0;
		else						Fore=0,Back=0,Left=0,Right=0;//ɲ
		}

} 

void USART2_Send_Data(char data)
{
	USART_SendData(USART2,data);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
}

//һ��
void USART2_Send_String(char *String)
{
	u16 len,j;
	
	len=strlen(String);
	for(j=0;j<len;j++)
	{
		USART2_Send_Data(*String++);
	}
}
