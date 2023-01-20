#include "stm32f10x.h"
#include "sys.h" 

float Pitch,Roll,Yaw;						//角度
short gyrox,gyroy,gyroz;				//陀螺仪--角速度
short aacx,aacy,aacz;						//加速度
int Encoder_Left,Encoder_Right;	//编码器数据（速度）

int PWM_MAX=7200,PWM_MIN=-7200;	//PWM限幅变量
int MOTO1,MOTO2;								//电机装载变量

extern int Vertical_out,Velocity_out,Turn_out;
int main(void)	
{
	int angle;
	delay_init();
//	Track_Init();
	
	NVIC_Config();
	
//	uart2_init(9600);//先9600
	
	OLED_Init();
	OLED_Clear();
	
	MPU_Init();
	mpu_dmp_init();
	MPU6050_EXTI_Init();
	
//	Encoder_TIM2_Init();
//	Encoder_TIM3_Init();
//	Motor_Init();
//	PWM_Init_TIM1(0,7199);
  while(1)	
	{
		angle+=gyroz;
		OLED_Float(0,0,Yaw,5);
		OLED_Float(2,0,Vertical_out,1);
		OLED_Float(4,0,Velocity_out,1);
		OLED_Float(6,0,MOTO1,1);
		OLED_Float(6,50,MOTO2,1);

	
	} 	
}



