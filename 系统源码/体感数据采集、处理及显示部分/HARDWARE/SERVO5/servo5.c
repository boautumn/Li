#include "servo5.h"

//05 �������  ��Ӧ���� roll  st11 st12

//ӳ���ϵ  

//������Ϻ��� y= kx+b    ��Ϻ����������Yֵ ȡ��  ��Ϊ�Ƕ�λ�ò���


extern void transfer(int a);
extern int st11 ;  
extern int st12 ;


extern int low ;
extern int high ;

void Contorl_five_Init(float x)
{
	int state = 0;
	int angle = 0 ;//������λ��״̬����

	state = x * 10 ;
	state =state/10;//������״̬�������Ѿ�ȡ����
	
if (state >= 0 && state <= 90) {
	angle =-9.741*state;//��������״̬����ת��Ϊ���λ�ò���
	angle =angle +1785.226;
	angle = angle * 10 ;
	angle =angle/10;   //ת����Ĳ���ȡ��
	

	transfer(angle); //ת����ֵ
	st11= low;  //��ֵ
	st12= high; //��ֵ

	}

	else if (state >= -55 && state < 0){
	

	angle =-2.241*state;//��������״̬����ת��Ϊ���λ�ò���
	angle =angle +1786.224;
	angle = angle * 10 ;
	angle =angle/10;   //ת����Ĳ���ȡ��
	

	transfer(angle); //ת����ֵ
	st11= low;  //��ֵ
	st12= high; //��ֵ		
		
		
		
	}
	
	
	else {}//�����
	
}

