#include "servo6.h"

//06 �������  ��Ӧ���� st3 st4
//���Ʋ�����yaw

//ӳ���ϵ  ��  �Ƕ�   
//ӳ���ϵ  ��  ��Ӧ

//������Ϻ��� y= kx+b    :
//��Ϻ����������Yֵ ȡ��  ��Ϊ�Ƕ�λ�ò���


extern void transfer(int a);
extern int st3 ;  
extern int st4 ;


extern int low ;
extern int high ;



void Contorl_six_Init(float x)
{
	int state = 0;
	int angle = 0 ;//������λ��״̬����

	state = x * 10 ;
	state =state/10;//������״̬�������Ѿ�ȡ����
	

	angle = 11.07*state;//��������״̬����ת��Ϊ���λ�ò���     ////δ���ò���
	angle =angle +1491.52;
	
	
	angle = angle * 10 ;
	angle =angle/10;   //ת����Ĳ���ȡ��
	

	transfer(angle); //ת����ֵ
	st3= low;  //��ֵ
	st4= high; //��ֵ
	


	}
