#include "transfer.h"

extern int low ;
extern int high ; //��������ȫ�ֱ���

void transfer(int a){

low = a & 0xff;    //�Ͱ�λ
	
high = (a>>8)&0XFF;     //�߰�λ
	
}

