#include "control.h"

#define SPEED_Y 20 //����(ǰ��)����趨�ٶ�
#define SPEED_Z 100//ƫ��(����)����趨�ٶ� 


float Med_Angle=0;	//��е��ֵ��---�������޸���Ļ�е��ֵ���ɡ�
float Target_Speed=0;	//�����ٶȡ�---���ο����ӿڣ����ڿ���С��ǰ�����˼����ٶȡ�
float Turn_Speed=0;

float 
	Vertical_Kp=138,//ֱ����KP��KD	+
	Vertical_Kd=-0.72;				//-
float 
	Velocity_Kp=-75,//�ٶȻ�KP��KI -
	Velocity_Ki=-0.325;			//-
float 
	Turn_Kp=20,
	Turn_Kd=-0.6;      //-

int Vertical_out,Velocity_out,Turn_out;//ֱ����&�ٶȻ�&ת�� ���������
int Vertical(float Med,float Angle,float gyro_Y);//��������
int Velocity(int Target,int encoder_left,int encoder_right);
int Turn(int gyro_Z,int RC); 


uint8_t TRACK_R1;
uint8_t TRACK_L1;
uint8_t TRACK_R2;
uint8_t TRACK_L2;

void EXTI9_5_IRQHandler(void)
{
	int PWM_out;
	if(EXTI_GetITStatus(EXTI_Line5)!=0)//һ���ж�
	{
		if(PBin(5)==0)//�����ж�
		{
			EXTI_ClearITPendingBit(EXTI_Line5);//����жϱ�־λ
			
			//1.�ɼ�����������&MPU6050�Ƕ���Ϣ��
			Encoder_Left=-Read_Speed(2);//�������԰�װ���պ����180�ȣ�Ϊ�˱������������һ�£�����Ҫ������һ��ȡ����
			Encoder_Right=Read_Speed(3);
			
			Angle_Calcu();			//�Ƕ�
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//������
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//���ٶ�
			//2.������ѹ��ջ������У�����������������
			if(Mode==1)
			{
				TRACK_R1 = Track_R1();TRACK_R2 = Track_R2();TRACK_L1 = Track_L1();TRACK_L2 = Track_L2();
				//С��ת
				if((TRACK_L1 == 0 && TRACK_L2 == 0 && TRACK_R1 == 1 && TRACK_R2 == 0))
				{
					Turn_Kd=0;Turn_Speed=-25;Target_Speed++;
				}
				//����ת
				else if((TRACK_L1 == 0 && TRACK_L2 == 0 && TRACK_R1 == 0 && TRACK_R2 == 1))
				{
					Turn_Kd=0;Turn_Speed=-50;
				}
				//С��ת
				else if((TRACK_L1 == 1 && TRACK_L2 == 0 && TRACK_R1 == 0 && TRACK_R2 == 0))
				{
					Turn_Kd=0;Turn_Speed=25;Target_Speed++;
				}
				//����ת
				else if((TRACK_L1 == 0 && TRACK_L2 == 1 && TRACK_R1 == 0 && TRACK_R2 == 0))
				{ 
					Turn_Kd=0;Turn_Speed=50;
				}
				//ֱ��
				else if((TRACK_L1 == 0 && TRACK_L2 == 0 && TRACK_R1 == 0 && TRACK_R2 == 0))
				{ 
					Turn_Kd=-0.6;Turn_Speed=0;Target_Speed++;
				}
				else
				{
					Turn_Kd=-0.6;Turn_Speed=0;Target_Speed=0;
				}
		
			}
			
			if(Mode==2)
			{
				/*ǰ��*/
				if((Fore==0)&&(Back==0))Target_Speed=0;//δ���ܵ�ǰ������ָ��-->�ٶ����㣬����ԭ��
				if(Fore==1)Target_Speed--;//ǰ��1��־λ����-->��Ҫǰ��
				if(Back==1)Target_Speed++;//
				
				/*����*/
				if((Left==0)&&(Right==0))Turn_Speed=0;
				if(Left==1)Turn_Speed=-20;	//��ת
				if(Right==1)Turn_Speed=20;	//��ת

				
				/*ת��Լ��*/
				if((Left==0)&&(Right==0))Turn_Kd=0.3;//��������ת��ָ�����ת��Լ��
				else if((Left==1)||(Right==1))Turn_Kd=0;//������ת��ָ����յ�����ȥ��ת��Լ��
				/*********************************************************************************************/
					
			}
			
			Target_Speed=Target_Speed>SPEED_Y?SPEED_Y:(Target_Speed<-SPEED_Y?(-SPEED_Y):Target_Speed);//�޷�	
			Turn_Speed=Turn_Speed>SPEED_Z?SPEED_Z:(Turn_Speed<-SPEED_Z?(-SPEED_Z):Turn_Speed);//�޷�( (20*100) * 100   )
			Velocity_out=Velocity(Target_Speed,Encoder_Left,Encoder_Right);	//�ٶȻ�
			Vertical_out=Vertical(Med_Angle,Pitch,gyroy);			//ֱ����
			Turn_out=Turn(gyroz,Turn_Speed);																						//ת��
			PWM_out = Vertical_out - Velocity_out;
			//3.�ѿ�����������ص�����ϣ�������յĵĿ��ơ�
			MOTO1=PWM_out-Turn_out;//����
			MOTO2=PWM_out+Turn_out;//�ҵ��
			Limit(&MOTO1,&MOTO2);	 //PWM�޷�			
			Load(MOTO1,MOTO2);		 //���ص�����ϡ�
			
			Stop(&Med_Angle,&Pitch);//��ȫ���
			
		}
	}
}




/*********************
ֱ����PD��������Kp*Ek+Kd*Ek_D

��ڣ������Ƕȡ���ʵ�Ƕȡ���ʵ���ٶ�
���ڣ�ֱ�������
*********************/
int Vertical(float Med,float Angle,float gyro_Y)
{
	int PWM_out;
	
	PWM_out=Vertical_Kp*(Angle-Med)+Vertical_Kd*(gyro_Y-0);
	return PWM_out;
}


/*********************
�ٶȻ�PI��Kp*Ek+Ki*Ek_S
*********************/
int Velocity(int Target,int encoder_left,int encoder_right)
{
	static int Encoder_S,EnC_Err_Lowout_last,PWM_out,Encoder_Err,EnC_Err_Lowout;
	float a=0.7;
	
	//1.�����ٶ�ƫ��
	Encoder_Err=((encoder_left+encoder_right)-Target);//��ȥ���--�ҵ���⣺�ܹ����ٶ�Ϊ"0"�ĽǶȣ����ǻ�е��ֵ��
	//2.���ٶ�ƫ����е�ͨ�˲�
	//low_out=(1-a)*Ek+a*low_out_last;
	EnC_Err_Lowout=(1-a)*Encoder_Err+a*EnC_Err_Lowout_last;//ʹ�ò��θ���ƽ�����˳���Ƶ���ţ���ֹ�ٶ�ͻ�䡣
	EnC_Err_Lowout_last=EnC_Err_Lowout;//��ֹ�ٶȹ����Ӱ��ֱ����������������
	//3.���ٶ�ƫ����֣����ֳ�λ��
	Encoder_S+=EnC_Err_Lowout;
	//4.�����޷�
	Encoder_S=Encoder_S>10000?10000:(Encoder_S<(-10000)?(-10000):Encoder_S);
	
	if(stop==1)Encoder_S=0,stop=0;//���������
	
	//5.�ٶȻ������������
	PWM_out=Velocity_Kp*EnC_Err_Lowout+Velocity_Ki*Encoder_S;
	return PWM_out;
}



/*********************
ת�򻷣�ϵ��*Z����ٶ�+ϵ��*ң������
*********************/
int Turn(int gyro_Z,int RC)
{
	int PWM_out;
	
	PWM_out=Turn_Kd*gyro_Z + Turn_Kp*RC;

	return PWM_out;
}

