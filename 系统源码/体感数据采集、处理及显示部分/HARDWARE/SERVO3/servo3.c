#include "servo3.h"

//03 舵机控制  对应参数 st1 st2
//控制参数；roll

//映射关系  ：  角度   
//映射关系  ：  对应

//线性拟合函数 y= kx+b    :
//拟合函数计算出的Y值 取整  即为角度位置参数


extern void transfer(int a);
extern int st1 ;  
extern int st2 ;


extern int low ;
extern int high ;



void Contorl_three_Init(float x)
{
	int state = 0;
	int angle = 0 ;//定义舵机位置状态参数

	state = x * 10 ;
	state =state/10;//陀螺仪状态参数（已经取整）
	
if (state >= -50&& state <= 90) {
	
	
		angle =-8.732*state;//将陀螺仪状态参数转化为舵机位置参数     ////未设置参数
	angle =angle + 2051.732;
	
	
	angle = angle * 10 ;
	angle =angle/10;   //转化后的参数取整
	

	transfer(angle); //转化赋值
	st1= low;  //赋值
	st2= high; //赋值
	
	
	
           
     }

	


	}
