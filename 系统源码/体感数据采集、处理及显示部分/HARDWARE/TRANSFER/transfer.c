#include "transfer.h"

extern int low ;
extern int high ; //声明调用全局变量

void transfer(int a){

low = a & 0xff;    //低八位
	
high = (a>>8)&0XFF;     //高八位
	
}

