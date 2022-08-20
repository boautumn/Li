#include "servo5.h"

//05 舵机控制  对应参数 roll  st11 st12

//映射关系  

//线性拟合函数 y= kx+b    拟合函数计算出的Y值 取整  即为角度位置参数


extern void transfer(int a);
extern int st11 ;  
extern int st12 ;


extern int low ;
extern int high ;

void Contorl_five_Init(float x)
{
	int state = 0;
	int angle = 0 ;//定义舵机位置状态参数

	state = x * 10 ;
	state =state/10;//陀螺仪状态参数（已经取整）
	
if (state >= 0 && state <= 90) {
	angle =-9.741*state;//将陀螺仪状态参数转化为舵机位置参数
	angle =angle +1785.226;
	angle = angle * 10 ;
	angle =angle/10;   //转化后的参数取整
	

	transfer(angle); //转化赋值
	st11= low;  //赋值
	st12= high; //赋值

	}

	else if (state >= -55 && state < 0){
	

	angle =-2.241*state;//将陀螺仪状态参数转化为舵机位置参数
	angle =angle +1786.224;
	angle = angle * 10 ;
	angle =angle/10;   //转化后的参数取整
	

	transfer(angle); //转化赋值
	st11= low;  //赋值
	st12= high; //赋值		
		
		
		
	}
	
	
	else {}//空语句
	
}

