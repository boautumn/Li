#include "servo4.h"

//04 �������  ��Ӧ���� roll  ӳ���ϵ   0-60  1120-1500     -60 ��0 950-1120   �ܽ� �Ƕ�-60��60  ��Ӧ950-1500 �Ƕ�
//������Ϻ��� y= kx+b  ��-60��950��  ��60��1150��  ��Ϻ����������Yֵ ȡ��  ��Ϊ�Ƕ�λ�ò���   �Ѿ��������λ��


extern void transfer(int a);
extern int st5 ;  
extern int st6 ;


extern int low ;
extern int high ;

void Contorl_four_Init(float x)
{
	int state = 0;
	int angle = 0 ;//������λ��״̬����

	state = x * 10 ;
	state =state/10;//������״̬�������Ѿ�ȡ����
	
if (state >= -50 && state <= 60) {
	angle =4.882*state;//��������״̬����ת��Ϊ���λ�ò���
	angle =angle +2169.882;
	angle = angle * 10 ;
	angle =angle/10;   //ת����Ĳ���ȡ��
	

	transfer(angle); //ת����ֵ
	st5= low;  //��ֵ
	st6= high; //��ֵ
	
}

	}
	
	

