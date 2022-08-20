#include "servo2.h"

//02 舵机控制  对应参数 st9 st10
//控制参数；pitch

//映射关系  ：  角度   
//映射关系  ：  对应

//线性拟合函数 y= kx+b    :
//拟合函数计算出的Y值 取整  即为角度位置参数


extern void transfer(int a);
extern int st9 ;  
extern int st10 ;


extern int low ;
extern int high ;



void Contorl_two_Init(float x)
{
	int state = 0;
	int angle = 0 ;//定义舵机位置状态参数

	state = x * 10 ;
	state =state/10;//陀螺仪状态参数（已经取整）
	
	
	if (state >= -50&& state <= 50) {
	

	angle = -12.625*state;//将陀螺仪状态参数转化为舵机位置参数     
	angle =angle +1501.263;
	
	
	angle = angle * 10 ;
	angle =angle/10;   //转化后的参数取整
	

	transfer(angle); //转化赋值
	st9= low;  //赋值
	st10= high; //赋值
	
	}
	
	

	}
