#include "servo2.h"

//02 �������  ��Ӧ���� st9 st10
//���Ʋ�����pitch

//ӳ���ϵ  ��  �Ƕ�   
//ӳ���ϵ  ��  ��Ӧ

//������Ϻ��� y= kx+b    :
//��Ϻ����������Yֵ ȡ��  ��Ϊ�Ƕ�λ�ò���


extern void transfer(int a);
extern int st9 ;  
extern int st10 ;


extern int low ;
extern int high ;



void Contorl_two_Init(float x)
{
	int state = 0;
	int angle = 0 ;//������λ��״̬����

	state = x * 10 ;
	state =state/10;//������״̬�������Ѿ�ȡ����
	
	
	if (state >= -50&& state <= 50) {
	

	angle = -12.625*state;//��������״̬����ת��Ϊ���λ�ò���     
	angle =angle +1501.263;
	
	
	angle = angle * 10 ;
	angle =angle/10;   //ת����Ĳ���ȡ��
	

	transfer(angle); //ת����ֵ
	st9= low;  //��ֵ
	st10= high; //��ֵ
	
	}
	
	

	}
