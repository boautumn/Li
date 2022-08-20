#include "sys.h"

short temp;					//�¶�	
float pitch,roll,yaw; 		//ŷ����
u8 data[9]={0xAA,0,0,0,0,0,0,0,0xEE};//���ݰ� ֡ͷ0xaa��֡β0xee
u16 balance_pitch,balance_roll,balance_yaw;
static u8 i=0;

//��ʱ��3�жϷ������	 
void TIM3_IRQHandler(void)
{ 		    		  			    
	if(TIM3->SR&0X0001)//����ж�
	{
				    				   				     	    	
	}				   
	TIM3->SR&=~(1<<0);//����жϱ�־λ 	    
}
//ͨ�ö�ʱ��3�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
void TIM3_Int_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<1;	//TIM3ʱ��ʹ��    
 	TIM3->ARR=arr;  	//�趨�������Զ���װֵ    
	TIM3->PSC=psc;  	//Ԥ��Ƶ��	  
	TIM3->DIER|=1<<0;   //��������ж�	  
	TIM3->CR1|=0x01;    //ʹ�ܶ�ʱ��3
  	MY_NVIC_Init(1,3,TIM3_IRQn,2);//��ռ1�������ȼ�3����2									 
}

void TIM4_Int_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<2;	//TIM4ʱ��ʹ��    
 	TIM4->ARR=arr;  	//�趨�������Զ���װֵ  
	TIM4->PSC=psc;  	//Ԥ��Ƶ��		  
	TIM4->DIER|=1<<0;   //��������ж�	  
	TIM4->CR1|=0x01;    //ʹ�ܶ�ʱ��4
  	MY_NVIC_Init(0,0,TIM4_IRQn,2);//������ȼ�������
}

void TIM4_IRQHandler(void)//��ʱ���ʹ���������
{ 		    		  			    
	if(TIM4->SR&0X0001)//����ж�
	{
		if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
		{
			balance_pitch=pitch+180;//��֤�Ƕ�ֵȫΪ��
			balance_roll=roll+180;	//�����������и�ֵ	
			balance_yaw=yaw+180;	//����ֱ��ͨ�����ڷ���
	
			data[2]=(balance_pitch>>8)&0xff;//��8λ
			data[3]=balance_pitch&0xff;		//��8λ
			data[4]=(balance_roll>>8)&0xff;
			data[5]=balance_roll&0xff;
			data[6]=(balance_yaw>>8)&0xff;
			data[7]=balance_yaw&0xff;
			for(i=0;i<9;i++)	
				usart1_send_char(data[i]);//��������		
		}
		
	}				   
	TIM4->SR&=~(1<<0);//����жϱ�־λ 	    
}









