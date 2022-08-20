
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
������л�е��  

//OLED����

//              GND    ��Դ��
//              VCC  ��5V��3.3v��Դ
//              D0   ��PG12��SCL��
//              D1   ��PD5��SDA��
//              RES  ��PD4
//              DC   ��PD15
//              CS   ��PD1 

  
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



int low = 0; //ʮ�����ƵͰ�λ
int high = 0;	//ʮ�����Ƹ߰�λ

unsigned char  buf[64];
unsigned char  buf1[64];

int flexADC;
float flexV=0.0;
float flexR=0.0;
float angle_1=0.0;
const float VCC =3.3; // ģ�鹩���ѹ��ADC�ο���ѹΪV
const float R_DIV =100000.0; // ��ѹ����Ϊ100K��
const float STRAIGHT_RESISTANCE =29089.0; // ƽֱʱ�ĵ���ֵ  ���޸�
const float BEND_RESISTANCE =40063.0; // 90������ʱ�ĵ���ֵ  ���޸�
long map(long x, long in_min, long in_max, long out_min, long out_max);

float adcx10;
float yawpidset;

	float cube[8][3]={{-20,-20,-20},{-20,20,-20},{20,20,-20},{20,-20,-20},{-20,-20,20},{-20,20,20},{20,20,20},{20,-20,20}};
	int lineid[]={1,2,2,3,3,4,4,1,5,6,6,7,7,8,8,5,8,4,7,3,6,2,5,1,};//
	

	/////////////////////////  

//����1����1���ַ� 
//c:Ҫ���͵��ַ�
void usart1_send_char(u8 c)
{   	
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET); //ѭ������,ֱ���������   
	USART_SendData(USART1,c);  
} 
//�������ݸ�����������λ�����(V2.6�汾)
//fun:������. 0XA0~0XAF
//data:���ݻ�����,���28�ֽ�!!
//len:data����Ч���ݸ���



void Contorl_date(void)  //���ڷ��Ϳ�������
	{
		
		int i =0;
		char ByteSend[25]={0};
		
		ByteSend[0]= 0x55;
		ByteSend[1]= 0x55;
		ByteSend[2]= 0x23;//���ݳ���=3*���ƶ������+5      23
		ByteSend[3]= 0x03;//ָ���� ����Ҫ����  ���ƶ������˶�
		ByteSend[4]= 0x06;//Ҫ���ƵĶ������
		ByteSend[5]= 0xC8;//ʱ��Ͱ�λ
		ByteSend[6]= 0x00;//ʱ��߰�λ
		
		ByteSend[7]= 0x03;//���Ŷ��
		ByteSend[8]= st1;//λ�õͰ�λ��03��
		ByteSend[9]= st2;//λ�ø߰�λ��03��
		
		ByteSend[10]= 0x06;//���Ŷ��
		ByteSend[11]= st3;//λ�õͰ�λ��06��
		ByteSend[12]= st4;//λ�ø߰�λ��06��
		
		ByteSend[13]= 0x04;//�ĺŶ��
		ByteSend[14]= st5;//λ�õͰ�λ��04��
		ByteSend[15]= st6;//λ�ø߰�λ��04��
		
		
		ByteSend[16]= 0x01;//һ�Ŷ��
		ByteSend[17]= st7;//λ�õͰ�λ��01��
		ByteSend[18]= st8;//λ�ø߰�λ��01��


		ByteSend[19]= 0x02;//���Ŷ��
		ByteSend[20]= st9;//λ�õͰ�λ��02��
		ByteSend[21]= st10;//λ�ø߰�λ��02��

		ByteSend[22]= 0x05;//��Ŷ��
		ByteSend[23]= st11;//λ�õͰ�λ��05��
		ByteSend[24]= st12;//λ�ø߰�λ��05��
	
		
		
		for (i=0;i<25;i++)
		{
			
			usart1_send_char(ByteSend[i]);
			
		}
		
		
	}
	


///////////////////////////////////////////////////////




//ͶӰ����ά�ռ�
float *matconv(float *a, float b[3][3]){
    float res[3];
    int i;
    for(i=0; i<3; ++i)
        res[i] = b[i][0] * a[0] + b[i][1] * a[1] + b[i][2] * a[2];
    for(i=0; i<3; ++i)
        a[i] = res[i];
    return a;
}


//��������ת
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
	u8 t=0;			//Ĭ�Ͽ����ϱ�
	u8 key;
	float pitch,roll,yaw; 		//ŷ����
	short aacx,aacy,aacz;		//���ٶȴ�����ԭʼ����
	short gyrox,gyroy,gyroz;	//������ԭʼ����

	 
	 infrared_INIT();//�����ʼ��///////////////////////////////////////////////////////////////////////////////
	 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(500000);	 	//���ڳ�ʼ��Ϊ500000   
  uart3_init(115200);	 
	Adc_Init();
	Dac1_Init();
	delay_init();	//��ʱ��ʼ�� 
	usmart_dev.init(72);		//��ʼ��USMART
	LED_Init();		  			//��ʼ����LED���ӵ�Ӳ���ӿ�
	KEY_Init();					//��ʼ������
	LCD_Init();			   		//��ʼ��LCD  
	MPU_Init();					//��ʼ��MPU6050
	OLED_Init();         //��ʼ��oled
 	POINT_COLOR=RED;			//��������Ϊ��ɫ 
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
		if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)    //����һ����ȡ��������
		{ 
	
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//�õ����ٶȴ���������
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//�õ�����������
		
			flexADC = Get_Adc(1);	//10��ƽ��ֵ
			flexV=(flexADC*VCC)/4096;
			flexR= R_DIV * (VCC / flexV-1.0);
			angle_1 = map(flexR, STRAIGHT_RESISTANCE, BEND_RESISTANCE,0, 90.0);	
			
			DAC_SetChannel1Data(DAC_Align_12b_R, yaw+180);//����DACֵ
			
			adcx10 = Get_Adc(10);	//10��ƽ��ֵ
			if(100<adcx10<120)adcx10=110;
			yawpidset=adcx10+yaw-110;
			
			sprintf((char *)buf,"page1.t5.txt=\"%.1f\"",pitch); //ǿ������ת����ת��Ϊ�ַ�
			HMISends((char *)buf); //����yaw�����ݸ�page0ҳ���t3�ı��ؼ�
			HMISendb(0xff);//������
		
			sprintf((char *)buf,"page1.t6.txt=\"%.1f\"",roll); //ǿ������ת����ת��Ϊ�ַ���
			HMISends((char *)buf); //����pitch�����ݸ�page0ҳ���t3�ı��ؼ�
			HMISendb(0xff);//������
		
			sprintf((char *)buf,"page1.t7.txt=\"%.1f\"",yawpidset); //ǿ������ת����ת��Ϊ�ַ���
			HMISends((char *)buf); //����roll�����ݸ�page0ҳ���t3�ı��ؼ�
			HMISendb(0xff);//������
	////////////////////////////////////////////////////////////////////////////////////////////////	
			//�������ǽ������ŷ���ǽ��ж��λ���������
			
			Contorl_one_Init(angle_1);//һ�Ŷ��״̬�����
			Contorl_two_Init(pitch);//���Ŷ��״̬���
			Contorl_three_Init(roll);//���Ŷ��״̬���
			Contorl_four_Init(roll);//�ĺŶ��״̬���
			Contorl_five_Init(roll);//��Ŷ��״̬���
			Contorl_six_Init(yawpidset);//���Ŷ��״̬���
			
			Contorl_date();//�������ָ��
			
	

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
