#include "stm32f10x.h"
#include "servo1.h"
#include "infrared.h"

//01 �������  ��Ӧ���� st7 st8
//���Ʋ�����pitch

//ӳ���ϵ  ��  �Ƕ�   
//ӳ���ϵ  ��  ��Ӧ

//������Ϻ��� y= kx+b    :
//��Ϻ����������Yֵ ȡ��  ��Ϊ�Ƕ�λ�ò���


extern void transfer(int a);
extern int st7 ;  
extern int st8 ;


extern int low ;
extern int high ;




//void Contorl_one_Init(float x)
//{
//	
// if(INFRARED_STATE()==0)  
//  {
//    st7 = 0x78;
//		st8 = 0x05;
//		
//		
//  }
//  else                  //?????,???????
//  {

//		st7 = 0x67;
//		st8 = 0x02;
//		
//  }
//			
//}
//			
		
void Contorl_one_Init(float x)
{
	

	int state = 0;
	int angle = 0 ;//������λ��״̬����


	state = x * 10 ;
	state =state/10;//������״̬�������Ѿ�ȡ����
	
   if (state <= 90) 
{    
//	angle = 9.44*state;//��������״̬����ת��Ϊ���λ�ò���   
	angle =state; 
	angle =800.000;
	
	
	angle = angle * 10 ;
	angle =angle/10;   //ת����Ĳ���ȡ��
	

	transfer(angle); //ת����ֵ
	st7= low;  //��ֵ
	st8= high; //��ֵ
	
}
	else if (state >= 90){
	
	angle =state;
	angle =2500;
	
	
	angle = angle * 10 ;
	angle =angle/10;   //ת����Ĳ���ȡ��
	

	transfer(angle); //ת����ֵ
	st7= low;  //��ֵ
	st8= high; //��ֵ	
		
		
		
	}
	
	
	else {}//�����

}

