#include "sys.h"

/*NCHU第十六届三小项目 体感机械臂-上位机*/

/*
  结合多方资料和现有资源完成，也算是个杂交成果【doge】
  感谢B站UP主：@无枫Raclette、@jo良jo影x、@Cyber_Mice
  项目灵感来自@无枫Raclette 下位机部分初始代码来自@jo良jo影x 
  项目改进思路可参考@Cyber_Mice 
*/

//u8 t=0;	
float adcx;
float adcx10;

int flexADC;
float flexV=0.0;
float flexR=0.0;
float angle_1=0.0;
const float VCC =3.3; // 模块供电电压，ADC参考电压为V
const float R_DIV =100000.0; // 分压电阻为100KΩ
const float STRAIGHT_RESISTANCE =29089.0; // 平直时的电阻值  需修改
const float BEND_RESISTANCE =48063.0; // 90度弯曲时的电阻值  需修改
long map(long x, long in_min, long in_max, long out_min, long out_max);
//short aacx,aacy,aacz;		//加速度传感器原始数据
//short gyrox,gyroy,gyroz;	//陀螺仪原始数据
float angle=0;

//传送数据给匿名四轴上位机软件(V2.6版本)
//fun:功能字. 0XA0~0XAF
//data:数据缓存区,最多28字节!!
//len:data区有效数据个数
void usart1_niming_report(u8 fun,u8*data,u8 len)
{
	u8 send_buf[32];
	u8 i;
	if(len>28)return;	//最多28字节数据 
	send_buf[len+3]=0;	//校验数置零
	send_buf[0]=0X88;	//帧头
	send_buf[1]=fun;	//功能字
	send_buf[2]=len;	//数据长度
	for(i=0;i<len;i++)send_buf[3+i]=data[i];			//复制数据
	for(i=0;i<len+3;i++)send_buf[len+3]+=send_buf[i];	//计算校验和	
	for(i=0;i<len+4;i++)usart1_send_char(send_buf[i]);	//发送数据到串口1 
}
//data:数据缓存区,最多28字节!!
//len:data区有效数据个数
void usart1_report(u8*data,u8 len)
{
	u8 send_buf[32];
	u8 i;
	if(len>28)return;	//最多28字节数据 
//	send_buf[len+3]=0;	//校验数置零
//	send_buf[0]=0X88;	//帧头
	send_buf[len]=len;//数据长度
	send_buf[len+1]=0x0d;//帧尾1
	send_buf[len+2]=0x0a;//帧尾2
	
	for(i=0;i<len;i++)send_buf[i]=data[i];			//复制数据
	
//	for(i=0;i<len+3;i++)send_buf[len+3]+=send_buf[i];	//计算校验和		
	for(i=0;i<len+3;i++)usart1_send_char(send_buf[i]);	//发送数据到串口1 
}
//发送加速度传感器数据和陀螺仪数据
//aacx,aacy,aacz:x,y,z三个方向上面的加速度值
//gyrox,gyroy,gyroz:x,y,z三个方向上面的陀螺仪值
void mpu6050_send_data(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz)
{
	u8 tbuf[12]; 
	tbuf[0]=(aacx>>8)&0XFF;
	tbuf[1]=aacx&0XFF;
	tbuf[2]=(aacy>>8)&0XFF;
	tbuf[3]=aacy&0XFF;
	tbuf[4]=(aacz>>8)&0XFF;
	tbuf[5]=aacz&0XFF; 
	tbuf[6]=(gyrox>>8)&0XFF;
	tbuf[7]=gyrox&0XFF;
	tbuf[8]=(gyroy>>8)&0XFF;
	tbuf[9]=gyroy&0XFF;
	tbuf[10]=(gyroz>>8)&0XFF;
	tbuf[11]=gyroz&0XFF;
//	usart1_niming_report(0XA1,tbuf,12);//自定义帧,0XA1
	usart1_report(tbuf,12);
}	
//通过串口1上报结算后的姿态数据给电脑
//aacx,aacy,aacz:x,y,z三个方向上面的加速度值
//gyrox,gyroy,gyroz:x,y,z三个方向上面的陀螺仪值
//roll:横滚角.单位0.01度。 -18000 -> 18000 对应 -180.00  ->  180.00度
//pitch:俯仰角.单位 0.01度。-9000 - 9000 对应 -90.00 -> 90.00 度
//yaw:航向角.单位为0.1度 0 -> 3600  对应 0 -> 360.0度
void usart1_report_imu(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz,short roll,short pitch,short yaw)
{
	u8 tbuf[28]; 
	u8 i;
	for(i=0;i<28;i++)tbuf[i]=0;//清0
	tbuf[0]=(aacx>>8)&0XFF;
	tbuf[1]=aacx&0XFF;
	tbuf[2]=(aacy>>8)&0XFF;
	tbuf[3]=aacy&0XFF;
	tbuf[4]=(aacz>>8)&0XFF;
	tbuf[5]=aacz&0XFF; 
	tbuf[6]=(gyrox>>8)&0XFF;
	tbuf[7]=gyrox&0XFF;
	tbuf[8]=(gyroy>>8)&0XFF;
	tbuf[9]=gyroy&0XFF;
	tbuf[10]=(gyroz>>8)&0XFF;
	tbuf[11]=gyroz&0XFF;	
	tbuf[18]=(roll>>8)&0XFF;
	tbuf[19]=roll&0XFF;
	tbuf[20]=(pitch>>8)&0XFF;
	tbuf[21]=pitch&0XFF;
	tbuf[22]=(yaw>>8)&0XFF;
	tbuf[23]=yaw&0XFF;
	usart1_niming_report(0XAF,tbuf,28);//飞控显示帧,0XAF
}   

void show_init(void)
{
	OLED_ShowString(5,0, " ADC :    ",12,1);
	OLED_ShowString(5,10," Temp:    ",12,1);	
	OLED_ShowString(5,20," Pitch:   ",12,1);	
	OLED_ShowString(5,30," Roll:    ",12,1);	 
	OLED_ShowString(5,40," Yaw :    ",12,1);		
	OLED_Refresh();

}
void show(void)
{
		adcx=Get_Adc_Average(ADC_Channel_1,3);//读取灰度传感器ad值
		adcx=(float)adcx/4096*33;
	  printf("dddddd\r\n");
	  printf("%d\r\n",adcx);
		data[1]=adcx;//数据包1位 
		OLED_ShowNum(5+48+8,0,adcx,2,12,1);	
//		if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
//		{ 	
				temp=MPU_Get_Temperature();//得到温度值
				if(temp<0)
				{
					OLED_ShowString(5+48,10,"-",12,1);		//显示负号
					temp=-temp;		//转为正数
				}
				else OLED_ShowString(5+48,10," ",12,1);		//去掉负号 
				OLED_ShowNum(5+48+8,10,temp/100,2,12,1);		//显示整数部分	    
				OLED_ShowString(5+48+24,10,".",12,1);
				OLED_ShowNum(5+48+30,10,temp%10,1,12,1);		//显示小数部分 
				
				temp=pitch*10;
				if(temp<0)
				{
					OLED_ShowString(5+48,20,"-",12,1);		//显示负号
					temp=-temp;		//转为正数
				}else OLED_ShowString(5+48,20," ",12,1);		//去掉负号 
				OLED_ShowNum(5+48+8,20,temp/10,2,12,1);		//显示整数部分	    
				OLED_ShowString(5+48+24,20,".",12,1);
				OLED_ShowNum(5+48+30,20,temp%10,1,12,1);		//显示小数部分 
				
				temp=roll*10;
				if(temp<0)
				{
					OLED_ShowString(5+48,30,"-",12,1);		//显示负号
					temp=-temp;		//转为正数
				}else OLED_ShowString(5+48,30," ",12,1);		//去掉负号 
				OLED_ShowNum(5+48+8,30,temp/10,2,12,1);		//显示整数部分	    
				OLED_ShowString(5+48+24,30,".",12,1);
				OLED_ShowNum(5+48+30,30,temp%10,1,12,1);		//显示小数部分 
									
				temp=yaw*10;
				if(temp<0)
				{
					OLED_ShowString(5+48,40,"-",12,1);		//显示负号
					temp=-temp;		//转为正数
				}else OLED_ShowString(5+48,40," ",12,1);		//去掉负号 
				OLED_ShowNum(5+48+8,40,temp/10,2,12,1);		//显示整数部分	 
				OLED_ShowString(5+48+24,40,".",12,1);				
				OLED_ShowNum(5+48+30,40,temp%10,1,12,1);		//显示小数部分  
				LED=!LED;//LED闪烁
			OLED_Refresh();	
//	}
}
/*
up本来想在上位机用两个mpu6050模块传角度数据，
一个在手背上，一个在小臂上，就不用pitch同时控制3、4号舵机
奈何官方库只能针对那1个IIC地址的模块进行配置，
想在同一个IIC总线上再加一个不同地址的mpu6050就相当与
重新写一遍mpuiic、inv_mpu、inv_mpu_dmp_motion文件，
现阶段本人水平有限，尝试过后实在行不通T.T。
tb搜 GY-25模块 结合@Cyber_Mice的方案，
可以实现整个手臂的姿态控制，感兴趣可以参考。
关于项目的任何问题欢迎在原视频下讨论，向神通广大的B友们求助^.^。
*/
int main(void)
 {     
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init();	    	 	//延时函数初始化	  
	uart_init(9600);	 		//串口初始化为9600
	LED_Init();		  			//初始化与LED连接的硬件接口
	Adc_Init();
	Dac1_Init();
//	KEY_Init();					//初始化按键
	MPU_Init();					//初始化MPU6050
	OLED_Init();
	OLED_ColorTurn(0);//0正常显示，1 反色显示
	OLED_DisplayTurn(0);//0正常显示 1 屏幕翻转显示
	delay_ms(100);//等待模块初始化完成
	OLED_DisPlay_On();
	while(mpu_dmp_init())
 	{
		OLED_ShowString(8,16,"MPU6050 Error",16,1);
		OLED_Refresh();
 		delay_ms(500);
		OLED_Clear();
	} 
	show_init();
	delay_ms(100);	
	TIM4_Int_Init(3999,719);//40ms
 	while(1)
	{	
//		adcx=Get_Adc_Average(ADC_Channel_1,3);//读取灰度传感器ad值
//		adcx=(float)adcx/4096*33;
//	  printf("dddddd\r\n");
//	  printf("ggggg%f\r\n",adcx);
//		data[1]=adcx;//数据包1位	
		flexADC = Get_Adc_Average(1,25);	//10次平均值
		flexV=(flexADC*VCC)/4096;
	  flexR= R_DIV * (VCC / flexV-1.0);
		angle_1 = map(flexR, STRAIGHT_RESISTANCE, BEND_RESISTANCE,0, 90.0);	
		printf("Bend:%fdegrees\r\n",angle_1);
		data[1]=angle_1;//数据包1?
		DAC_SetChannel1Data(DAC_Align_12b_R, yaw+180);//设置DAC值
		printf("设置:%f\r\n",yaw);

		adcx10 = Get_Adc_Average(10,25);	//10次平均值
		printf("adcx10:%f\r\n",adcx10);

//    printf("Yaw:%f degrees\r\n",yaw);

		
	}
	
}
 long map(long x, long in_min, long in_max, long out_min, long out_max) {
 return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
