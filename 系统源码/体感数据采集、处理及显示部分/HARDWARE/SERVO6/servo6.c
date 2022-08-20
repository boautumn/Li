#include "servo6.h"

//06 舵机控制  对应参数 st3 st4
//控制参数；yaw

//映射关系  ：  角度   
//映射关系  ：  对应

//线性拟合函数 y= kx+b    :
//拟合函数计算出的Y值 取整  即为角度位置参数


extern void transfer(int a);
extern int st3 ;  
extern int st4 ;


extern int low ;
extern int high ;



void Contorl_six_Init(float x)
{
	int state = 0;
	int angle = 0 ;//定义舵机位置状态参数

	state = x * 10 ;
	state =state/10;//陀螺仪状态参数（已经取整）
	

	angle = 11.07*state;//将陀螺仪状态参数转化为舵机位置参数     ////未设置参数
	angle =angle +1491.52;
	
	
	angle = angle * 10 ;
	angle =angle/10;   //转化后的参数取整
	

	transfer(angle); //转化赋值
	st3= low;  //赋值
	st4= high; //赋值
	


	}
