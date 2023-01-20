#include "usart2.h"	  





void uart2_init(u32 bound)
{
	//GPIO端口设置
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
	//USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
	
	
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启中断
	USART_Cmd(USART2, ENABLE);                    //使能串口 

	
}

/*
0x00:stop
0x01:run
0x05:back
0x03:turn left
0x07:turn right
*/

u8 Fore,Back,Left,Right,Mode;

void USART2_IRQHandler(void)                	//串口2中断服务程序
	{
		int Bluetooth_data;

		
		if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断
		{
		Bluetooth_data = USART_ReceiveData(USART2);//(USART2->DR);	//读取接收到的数据
		
		if(Bluetooth_data==0x00)Fore=0,Back=0,Left=0,Right=0;//刹
		else if(Bluetooth_data==0x05)Fore=1,Back=0,Left=0,Right=0;//前
		else if(Bluetooth_data==0x01)Fore=0,Back=1,Left=0,Right=0;//后
		else if(Bluetooth_data==0x07)Fore=0,Back=0,Left=1,Right=0;//左
		else if(Bluetooth_data==0x03)Fore=0,Back=0,Left=0,Right=1;//右
		else if(Bluetooth_data==0x09)Mode=1;
		else if(Bluetooth_data==0x011)Mode=2;
		else if(Bluetooth_data==0x013)Mode=0;
		else						Fore=0,Back=0,Left=0,Right=0;//刹
		}

} 

void USART2_Send_Data(char data)
{
	USART_SendData(USART2,data);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
}

//一串
void USART2_Send_String(char *String)
{
	u16 len,j;
	
	len=strlen(String);
	for(j=0;j<len;j++)
	{
		USART2_Send_Data(*String++);
	}
}
