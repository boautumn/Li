#include "servo3.h"

//03 �������  ��Ӧ���� st1 st2
//���Ʋ�����roll

//ӳ���ϵ  ��  �Ƕ�   
//ӳ���ϵ  ��  ��Ӧ

//������Ϻ��� y= kx+b    :
//��Ϻ����������Yֵ ȡ��  ��Ϊ�Ƕ�λ�ò���


extern void transfer(int a);
extern int st1 ;  
extern int st2 ;


extern int low ;
extern int high ;



void Contorl_three_Init(float x)
{
	int state = 0;
	int angle = 0 ;//������λ��״̬����

	state = x * 10 ;
	state =state/10;//������״̬�������Ѿ�ȡ����
	
if (state >= -50&& state <= 90) {
	
	
		angle =-8.732*state;//��������״̬����ת��Ϊ���λ�ò���     ////δ���ò���
	angle =angle + 2051.732;
	
	
	angle = angle * 10 ;
	angle =angle/10;   //ת����Ĳ���ȡ��
	

	transfer(angle); //ת����ֵ
	st1= low;  //��ֵ
	st2= high; //��ֵ
	
	
	
           
     }

	


	}
