#include "stm32f10x.h"
#include "servo1.h"
#include "infrared.h"

//01 舵机控制  对应参数 st7 st8
//控制参数；pitch

//映射关系  ：  角度   
//映射关系  ：  对应

//线性拟合函数 y= kx+b    :
//拟合函数计算出的Y值 取整  即为角度位置参数


extern void transfer(int a);
extern int st7 ;  
extern int st8 ;


extern int low ;
extern int high ;




//void Contorl_one_Init(float x)
//{
//	
// if(INFRARED_STATE()==0)  
//  {
//    st7 = 0x78;
//		st8 = 0x05;
//		
//		
//  }
//  else                  //?????,???????
//  {

//		st7 = 0x67;
//		st8 = 0x02;
//		
//  }
//			
//}
//			
		
void Contorl_one_Init(float x)
{
	

	int state = 0;
	int angle = 0 ;//定义舵机位置状态参数


	state = x * 10 ;
	state =state/10;//陀螺仪状态参数（已经取整）
	
   if (state <= 90) 
{    
//	angle = 9.44*state;//将陀螺仪状态参数转化为舵机位置参数   
	angle =state; 
	angle =800.000;
	
	
	angle = angle * 10 ;
	angle =angle/10;   //转化后的参数取整
	

	transfer(angle); //转化赋值
	st7= low;  //赋值
	st8= high; //赋值
	
}
	else if (state >= 90){
	
	angle =state;
	angle =2500;
	
	
	angle = angle * 10 ;
	angle =angle/10;   //转化后的参数取整
	

	transfer(angle); //转化赋值
	st7= low;  //赋值
	st8= high; //赋值	
		
		
		
	}
	
	
	else {}//空语句

}

