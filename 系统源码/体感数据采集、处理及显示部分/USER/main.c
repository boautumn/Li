
#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"
#include "mpu6050.h"
#include "usmart.h"   
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include "transfer.h"
#include "infrared.h"
#include "servo4.h"
#include "servo1.h"
#include "servo2.h"
#include "servo3.h"
#include "servo5.h"
#include "servo6.h"
#include "math.h"
#include "adc.h"
#include "dac.h"
#include "usart3.h"
#define PI 3.141592


 
/************************************************  
六轴体感机械臂  

//OLED接线

//              GND    电源地
//              VCC  接5V或3.3v电源
//              D0   接PG12（SCL）
//              D1   接PD5（SDA）
//              RES  接PD4
//              DC   接PD15
//              CS   接PD1 

  
************************************************/

int st1 = 0;  //03
int st2 = 0;

int st3 = 0;  //06
int st4 = 0;

int st5 = 0;  //04
int st6 = 0;

int st7 = 0;   //01
int st8 = 0;

int st9 = 0;  //02
int st10 = 0;

int st11 = 0;  //05
int st12 = 0;



int low = 0; //十六进制低八位
int high = 0;	//十六进制高八位

unsigned char  buf[64];
unsigned char  buf1[64];

int flexADC;
float flexV=0.0;
float flexR=0.0;
float angle_1=0.0;
const float VCC =3.3; // 模块供电电压，ADC参考电压为V
const float R_DIV =100000.0; // 分压电阻为100KΩ
const float STRAIGHT_RESISTANCE =29089.0; // 平直时的电阻值  需修改
const float BEND_RESISTANCE =40063.0; // 90度弯曲时的电阻值  需修改
long map(long x, long in_min, long in_max, long out_min, long out_max);

float adcx10;
float yawpidset;

	float cube[8][3]={{-20,-20,-20},{-20,20,-20},{20,20,-20},{20,-20,-20},{-20,-20,20},{-20,20,20},{20,20,20},{20,-20,20}};
	int lineid[]={1,2,2,3,3,4,4,1,5,6,6,7,7,8,8,5,8,4,7,3,6,2,5,1,};//
	

	/////////////////////////  

//串口1发送1个字符 
//c:要发送的字符
void usart1_send_char(u8 c)
{   	
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET); //循环发送,直到发送完毕   
	USART_SendData(USART1,c);  
} 
//传送数据给匿名四轴上位机软件(V2.6版本)
//fun:功能字. 0XA0~0XAF
//data:数据缓存区,最多28字节!!
//len:data区有效数据个数



void Contorl_date(void)  //串口发送控制数据
	{
		
		int i =0;
		char ByteSend[25]={0};
		
		ByteSend[0]= 0x55;
		ByteSend[1]= 0x55;
		ByteSend[2]= 0x23;//数据长度=3*控制舵机个数+5      23
		ByteSend[3]= 0x03;//指令名 不需要更改  控制多个舵机运动
		ByteSend[4]= 0x06;//要控制的舵机个数
		ByteSend[5]= 0xC8;//时间低八位
		ByteSend[6]= 0x00;//时间高八位
		
		ByteSend[7]= 0x03;//三号舵机
		ByteSend[8]= st1;//位置低八位（03）
		ByteSend[9]= st2;//位置高八位（03）
		
		ByteSend[10]= 0x06;//六号舵机
		ByteSend[11]= st3;//位置低八位（06）
		ByteSend[12]= st4;//位置高八位（06）
		
		ByteSend[13]= 0x04;//四号舵机
		ByteSend[14]= st5;//位置低八位（04）
		ByteSend[15]= st6;//位置高八位（04）
		
		
		ByteSend[16]= 0x01;//一号舵机
		ByteSend[17]= st7;//位置低八位（01）
		ByteSend[18]= st8;//位置高八位（01）


		ByteSend[19]= 0x02;//二号舵机
		ByteSend[20]= st9;//位置低八位（02）
		ByteSend[21]= st10;//位置高八位（02）

		ByteSend[22]= 0x05;//五号舵机
		ByteSend[23]= st11;//位置低八位（05）
		ByteSend[24]= st12;//位置高八位（05）
	
		
		
		for (i=0;i<25;i++)
		{
			
			usart1_send_char(ByteSend[i]);
			
		}
		
		
	}
	


///////////////////////////////////////////////////////




//投影到二维空间
float *matconv(float *a, float b[3][3]){
    float res[3];
    int i;
    for(i=0; i<3; ++i)
        res[i] = b[i][0] * a[0] + b[i][1] * a[1] + b[i][2] * a[2];
    for(i=0; i<3; ++i)
        a[i] = res[i];
    return a;
}


//立方体旋转
void rotate(float* obj, float x, float y, float z){


float rz[3][3] ={0};
float ry[3][3] ={0};
float rx[3][3] ={0};

			
		rz[0][0]= cos(z/PI);rz[0][1]=-sin(z/PI);rz[0][2]=0;
		rz[1][0]= sin(z/PI);rz[1][1]=cos(z/PI);rz[1][2]=0;
		rz[2][0]= 0;rz[2][1]=0;rz[2][2]=1;

    ry[0][0]= 1;ry[0][1]=0;ry[0][2]=0;
		ry[1][0]= 0;ry[1][1]=cos(y/PI);ry[1][2]=-sin(y/PI);
		ry[2][0]= 0;ry[2][1]=sin(y/PI);ry[2][2]=cos(y/PI);

		rx[0][0]= cos(x/PI);rx[0][1]=0;rx[0][2]=sin(x/PI);
		rx[1][0]= 0;rx[1][1]=1;rx[1][2]=0;
		rx[2][0]= -sin(x/PI);rx[2][1]=0;rx[2][2]=cos(x/PI);		
    matconv(matconv(matconv(obj,rz), ry), rx);



}	
	

	

	

/////////////////////////////////////////////////////////
 	
 int main(void)
 {	 
	u8 t=0;			//默认开启上报
	u8 key;
	float pitch,roll,yaw; 		//欧拉角
	short aacx,aacy,aacz;		//加速度传感器原始数据
	short gyrox,gyroy,gyroz;	//陀螺仪原始数据

	 
	 infrared_INIT();//红外初始化///////////////////////////////////////////////////////////////////////////////
	 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(500000);	 	//串口初始化为500000   
  uart3_init(115200);	 
	Adc_Init();
	Dac1_Init();
	delay_init();	//延时初始化 
	usmart_dev.init(72);		//初始化USMART
	LED_Init();		  			//初始化与LED连接的硬件接口
	KEY_Init();					//初始化按键
	LCD_Init();			   		//初始化LCD  
	MPU_Init();					//初始化MPU6050
	OLED_Init();         //初始化oled
 	POINT_COLOR=RED;			//设置字体为红色 
	while(mpu_dmp_init())
 	{
//		LCD_ShowString(30,130,200,16,16,"MPU6050 Error");
		delay_ms(200);
		LCD_Fill(30,130,239,130+16,WHITE);
 		delay_ms(200);
	}  
 	while(1)
	{  
		
		key=KEY_Scan(0);
		if(key==KEY0_PRES)
		{
//			report=!report;
//			if(report)LCD_ShowString(30,170,200,16,16,"UPLOAD ON ");
//			else LCD_ShowString(30,170,200,16,16,"UPLOAD OFF");
		}
		if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)    //在这一步读取解算数据
		{ 
	
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//得到加速度传感器数据
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据
		
			flexADC = Get_Adc(1);	//10次平均值
			flexV=(flexADC*VCC)/4096;
			flexR= R_DIV * (VCC / flexV-1.0);
			angle_1 = map(flexR, STRAIGHT_RESISTANCE, BEND_RESISTANCE,0, 90.0);	
			
			DAC_SetChannel1Data(DAC_Align_12b_R, yaw+180);//设置DAC值
			
			adcx10 = Get_Adc(10);	//10次平均值
			if(100<adcx10<120)adcx10=110;
			yawpidset=adcx10+yaw-110;
			
			sprintf((char *)buf,"page1.t5.txt=\"%.1f\"",pitch); //强制类型转化，转化为字符
			HMISends((char *)buf); //发送yaw的数据给page0页面的t3文本控件
			HMISendb(0xff);//结束符
		
			sprintf((char *)buf,"page1.t6.txt=\"%.1f\"",roll); //强制类型转化，转化为字符串
			HMISends((char *)buf); //发送pitch的数据给page0页面的t3文本控件
			HMISendb(0xff);//结束符
		
			sprintf((char *)buf,"page1.t7.txt=\"%.1f\"",yawpidset); //强制类型转化，转化为字符串
			HMISends((char *)buf); //发送roll的数据给page0页面的t3文本控件
			HMISendb(0xff);//结束符
	////////////////////////////////////////////////////////////////////////////////////////////////	
			//对陀螺仪解算出的欧拉角进行舵机位置线性拟合
			
			Contorl_one_Init(angle_1);//一号舵机状态读拟合
			Contorl_two_Init(pitch);//二号舵机状态拟合
			Contorl_three_Init(roll);//三号舵机状态拟合
			Contorl_four_Init(roll);//四号舵机状态拟合
			Contorl_five_Init(roll);//五号舵机状态拟合
			Contorl_six_Init(yawpidset);//六号舵机状态拟合
			
			Contorl_date();//传输控制指令
			
	

		}
		t++; 
			//////////////////////////////////////////////////////////////////////////////////////////////	
      POINT_COLOR=WHITE;
			BACK_COLOR= BLACK;

		
	} 	
}
	
 long map(long x, long in_min, long in_max, long out_min, long out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
 }
