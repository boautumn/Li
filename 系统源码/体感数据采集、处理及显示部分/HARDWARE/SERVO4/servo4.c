#include "servo4.h"

//04 舵机控制  对应参数 roll  映射关系   0-60  1120-1500     -60 到0 950-1120   总结 角度-60到60  对应950-1500 角度
//线性拟合函数 y= kx+b  （-60，950）  （60，1150）  拟合函数计算出的Y值 取整  即为角度位置参数   已经重新拟合位置


extern void transfer(int a);
extern int st5 ;  
extern int st6 ;


extern int low ;
extern int high ;

void Contorl_four_Init(float x)
{
	int state = 0;
	int angle = 0 ;//定义舵机位置状态参数

	state = x * 10 ;
	state =state/10;//陀螺仪状态参数（已经取整）
	
if (state >= -50 && state <= 60) {
	angle =4.882*state;//将陀螺仪状态参数转化为舵机位置参数
	angle =angle +2169.882;
	angle = angle * 10 ;
	angle =angle/10;   //转化后的参数取整
	

	transfer(angle); //转化赋值
	st5= low;  //赋值
	st6= high; //赋值
	
}

	}
	
	

