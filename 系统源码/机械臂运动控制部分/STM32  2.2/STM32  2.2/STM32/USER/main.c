
#include "include.h"

u8 state=0;
int kl;

u16 a1=0;
u16 a11=0;
u16 a2=0;
u16 a22=0;
uint8 ps_ok;
uint16 ServoPwmDutySet1[][8]={
															{1500,800,1500,2000,2200,1900,1500,1500},//复位
															{1500,800,1500,2000,2300,1900,1800,1500},//左转
	                            {1500,800,1500,1550,2300,1000,1800,1500},//伸手
                              {1500,1900,1500,1550,2300,1000,1800,1500},//夹紧
															{1500,1900,1500,1550,2100,1000,1800,1500},//抬手
															{1500,1900,1500,1550,2100,1000,1500,1500},//旋转回中
															{1500,1900,1500,1550,2200,1000,1500,1500},//下降
															{1500,1900,1500,1550,2265,1000,1500,1500},//下降等待
															{1500,800,1500,1550,2265,1000,1500,1500},//松手
															{1500,800,1500,2000,2200,1900,1500,1500},//复位

                               };//1号舵机1800部分抓不紧
//                                    1    2  (3    4     5)    6
uint16 ServoPwmDutySet2[][8]={
															 {1500,800,1500,2000,2200,1900,1500,1500},//复位
															 {1500,800,1500,2000,2300,1900,1200,1500},//右转
                               {1500,800,1500,1550,2300,1000,1200,1500},//伸手
                               {1500,1900,1500,1550,2300,1000,1200,1500},//夹紧
														 	 {1500,1900,1500,1750,2100,1100,1200,1500},//抬手
															 {1500,1900,1500,1750,2100,1100,1500,1500},//旋转回中
															 {1500,1900,1500,1750,2165,1100,1500,1500},//下降
															 {1500,800,1500,1750,2165,1100,1500,1500},//松手
															 {1500,800,1500,2000,2200,1900,1500,1500},//复位---------第二动作
															 {1500,800,1500,2000,2300,1900,1800,1500},//左转
	                             {1500,800,1500,1550,2300,1000,1800,1500},//伸手
                               {1500,1900,1500,1550,2300,1000,1800,1500},//夹紧
															 {1500,1900,1500,1710,2100,1200,1800,1500},//抬手
															 {1500,1900,1500,1710,2100,1200,1500,1500},//旋转回中
															 {1500,1900,1500,1710,2170,1200,1500,1500},//下降
															 {1500,1900,1500,1710,2200,1200,1500,1500},//下降等待
															 {1500,800,1500,1710,2200,1200,1500,1500},//松手
															 {1500,800,1500,2000,2200,1900,1500,1500},//复位
                               };
int main(void)
{
	a1=0;
  a11=0;
  a2=0;
  a22=0;
	ps_ok = 1;
	SystemInit(); 			 //系统时钟初始化为72M	  SYSCLK_FREQ_72MHz
	InitDelay(72);	     //延时初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	InitPWM();
	InitTimer2();//用于产生100us的定时中断
	InitUart1();//用于与PC端进行通信
	uart3_init();
	InitADC();
	InitLED();
	InitKey();
	InitBuzzer();
//	ps_ok = InitPS2();//PS2游戏手柄接收器初始化
	InitFlash();
	InitMemory();
	InitBusServoCtrl();
	LED = LED_ON;
	BusServoCtrl(1,SERVO_MOVE_TIME_WRITE,500,1000);
	BusServoCtrl(2,SERVO_MOVE_TIME_WRITE,500,1000);
	BusServoCtrl(3,SERVO_MOVE_TIME_WRITE,500,1000);
	BusServoCtrl(4,SERVO_MOVE_TIME_WRITE,500,1000);
	BusServoCtrl(5,SERVO_MOVE_TIME_WRITE,500,1000);
	BusServoCtrl(6,SERVO_MOVE_TIME_WRITE,500,1000);
	while(1)
	{
//		TaskRun(ps_ok);
		if(arr1[2]==0x2C)state=arr1[1];
		if(state==1)//抓取一号目标，停止加热
		{
				USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);//开启中断
				a2=0;
				a22=0;
				ServoPwmDutySet[1]=ServoPwmDutySet1[a1][1];	
				ServoPwmDutySet[2]=ServoPwmDutySet1[a1][2];
				ServoPwmDutySet[3]=ServoPwmDutySet1[a1][3];
				ServoPwmDutySet[4]=ServoPwmDutySet1[a1][4];
				ServoPwmDutySet[5]=ServoPwmDutySet1[a1][5];
				ServoPwmDutySet[6]=ServoPwmDutySet1[a1][6];
				ServoPwmDutyHaveChange=TRUE;
				if(systimes%10==0)//每20ms进一次比较，根据具体情况调整
				{
					ServoTime=4000;
					ServoPwmDutyCompare();	
					a11=a11+1;
				}
				if(a11%15000==0)
				{
					a11=a11+1;
					if(a1<9)
						a1=a1+1;
//					printf("完成动作1\r\n");
				}
			
		}
		
		else if(state==2)//抓取二号目标，继续加热
		{
			USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);//开启中断
			a1=0;
			a11=0;
			ServoPwmDutySet[1]=ServoPwmDutySet2[a2][1];	
			ServoPwmDutySet[2]=ServoPwmDutySet2[a2][2];
			ServoPwmDutySet[3]=ServoPwmDutySet2[a2][3];
			ServoPwmDutySet[4]=ServoPwmDutySet2[a2][4];
			ServoPwmDutySet[5]=ServoPwmDutySet2[a2][5];
			ServoPwmDutySet[6]=ServoPwmDutySet2[a2][6];
			ServoPwmDutyHaveChange=TRUE;
			if(systimes%10==0)//每20ms进一次比较，根据具体情况调整
			{
				ServoTime=4000;
				ServoPwmDutyCompare();	
				a22=a22+1;
			}
			if(a22%15000==0)
			{
				a22=a22+1;
				if(a2<17)
					a2=a2+1;
//				printf("完成动作2\r\n");
			}		
			
		}
		
		else if(state==3)//切换体感控制，50度
		{
			a1=0;
			a11=0;
			a2=0;
			a22=0;
			USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断
//			TaskRun(ps_ok);
			TaskPCMsgHandle();
			if(systimes%2==0)//每20ms进一次比较，根据具体情况调整
			{
				ServoPwmDutyCompare();	
			}
		}	
		
		else if(state==0)
		{
			USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);//开启中断
			a1=0;
			a11=0;
			a2=0;
			a22=0;
			ServoPwmDutySet[1]=800;
			ServoPwmDutySet[2]=1500;
			ServoPwmDutySet[3]=2000;
			ServoPwmDutySet[4]=2200;
			ServoPwmDutySet[5]=1900;
			ServoPwmDutySet[6]=1500;
			ServoPwmDutyHaveChange=TRUE;
			if(systimes%20==0)//每20ms进一次比较，根据具体情况调整
			{
				ServoTime=10000;
				ServoPwmDutyCompare();	
				CheckBatteryVoltage();
			}

		}
	}
}


