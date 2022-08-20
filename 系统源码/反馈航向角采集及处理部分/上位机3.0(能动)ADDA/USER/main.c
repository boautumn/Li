#include "sys.h"

/*NCHU��ʮ������С��Ŀ ��л�е��-��λ��*/

/*
  ��϶෽���Ϻ�������Դ��ɣ�Ҳ���Ǹ��ӽ��ɹ���doge��
  ��лBվUP����@�޷�Raclette��@jo��joӰx��@Cyber_Mice
  ��Ŀ�������@�޷�Raclette ��λ�����ֳ�ʼ��������@jo��joӰx 
  ��Ŀ�Ľ�˼·�ɲο�@Cyber_Mice 
*/

//u8 t=0;	
float adcx;
float adcx10;

int flexADC;
float flexV=0.0;
float flexR=0.0;
float angle_1=0.0;
const float VCC =3.3; // ģ�鹩���ѹ��ADC�ο���ѹΪV
const float R_DIV =100000.0; // ��ѹ����Ϊ100K��
const float STRAIGHT_RESISTANCE =29089.0; // ƽֱʱ�ĵ���ֵ  ���޸�
const float BEND_RESISTANCE =48063.0; // 90������ʱ�ĵ���ֵ  ���޸�
long map(long x, long in_min, long in_max, long out_min, long out_max);
//short aacx,aacy,aacz;		//���ٶȴ�����ԭʼ����
//short gyrox,gyroy,gyroz;	//������ԭʼ����
float angle=0;

//�������ݸ�����������λ�����(V2.6�汾)
//fun:������. 0XA0~0XAF
//data:���ݻ�����,���28�ֽ�!!
//len:data����Ч���ݸ���
void usart1_niming_report(u8 fun,u8*data,u8 len)
{
	u8 send_buf[32];
	u8 i;
	if(len>28)return;	//���28�ֽ����� 
	send_buf[len+3]=0;	//У��������
	send_buf[0]=0X88;	//֡ͷ
	send_buf[1]=fun;	//������
	send_buf[2]=len;	//���ݳ���
	for(i=0;i<len;i++)send_buf[3+i]=data[i];			//��������
	for(i=0;i<len+3;i++)send_buf[len+3]+=send_buf[i];	//����У���	
	for(i=0;i<len+4;i++)usart1_send_char(send_buf[i]);	//�������ݵ�����1 
}
//data:���ݻ�����,���28�ֽ�!!
//len:data����Ч���ݸ���
void usart1_report(u8*data,u8 len)
{
	u8 send_buf[32];
	u8 i;
	if(len>28)return;	//���28�ֽ����� 
//	send_buf[len+3]=0;	//У��������
//	send_buf[0]=0X88;	//֡ͷ
	send_buf[len]=len;//���ݳ���
	send_buf[len+1]=0x0d;//֡β1
	send_buf[len+2]=0x0a;//֡β2
	
	for(i=0;i<len;i++)send_buf[i]=data[i];			//��������
	
//	for(i=0;i<len+3;i++)send_buf[len+3]+=send_buf[i];	//����У���		
	for(i=0;i<len+3;i++)usart1_send_char(send_buf[i]);	//�������ݵ�����1 
}
//���ͼ��ٶȴ��������ݺ�����������
//aacx,aacy,aacz:x,y,z������������ļ��ٶ�ֵ
//gyrox,gyroy,gyroz:x,y,z�������������������ֵ
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
//	usart1_niming_report(0XA1,tbuf,12);//�Զ���֡,0XA1
	usart1_report(tbuf,12);
}	
//ͨ������1�ϱ���������̬���ݸ�����
//aacx,aacy,aacz:x,y,z������������ļ��ٶ�ֵ
//gyrox,gyroy,gyroz:x,y,z�������������������ֵ
//roll:�����.��λ0.01�ȡ� -18000 -> 18000 ��Ӧ -180.00  ->  180.00��
//pitch:������.��λ 0.01�ȡ�-9000 - 9000 ��Ӧ -90.00 -> 90.00 ��
//yaw:�����.��λΪ0.1�� 0 -> 3600  ��Ӧ 0 -> 360.0��
void usart1_report_imu(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz,short roll,short pitch,short yaw)
{
	u8 tbuf[28]; 
	u8 i;
	for(i=0;i<28;i++)tbuf[i]=0;//��0
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
	usart1_niming_report(0XAF,tbuf,28);//�ɿ���ʾ֡,0XAF
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
		adcx=Get_Adc_Average(ADC_Channel_1,3);//��ȡ�Ҷȴ�����adֵ
		adcx=(float)adcx/4096*33;
	  printf("dddddd\r\n");
	  printf("%d\r\n",adcx);
		data[1]=adcx;//���ݰ�1λ 
		OLED_ShowNum(5+48+8,0,adcx,2,12,1);	
//		if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
//		{ 	
				temp=MPU_Get_Temperature();//�õ��¶�ֵ
				if(temp<0)
				{
					OLED_ShowString(5+48,10,"-",12,1);		//��ʾ����
					temp=-temp;		//תΪ����
				}
				else OLED_ShowString(5+48,10," ",12,1);		//ȥ������ 
				OLED_ShowNum(5+48+8,10,temp/100,2,12,1);		//��ʾ��������	    
				OLED_ShowString(5+48+24,10,".",12,1);
				OLED_ShowNum(5+48+30,10,temp%10,1,12,1);		//��ʾС������ 
				
				temp=pitch*10;
				if(temp<0)
				{
					OLED_ShowString(5+48,20,"-",12,1);		//��ʾ����
					temp=-temp;		//תΪ����
				}else OLED_ShowString(5+48,20," ",12,1);		//ȥ������ 
				OLED_ShowNum(5+48+8,20,temp/10,2,12,1);		//��ʾ��������	    
				OLED_ShowString(5+48+24,20,".",12,1);
				OLED_ShowNum(5+48+30,20,temp%10,1,12,1);		//��ʾС������ 
				
				temp=roll*10;
				if(temp<0)
				{
					OLED_ShowString(5+48,30,"-",12,1);		//��ʾ����
					temp=-temp;		//תΪ����
				}else OLED_ShowString(5+48,30," ",12,1);		//ȥ������ 
				OLED_ShowNum(5+48+8,30,temp/10,2,12,1);		//��ʾ��������	    
				OLED_ShowString(5+48+24,30,".",12,1);
				OLED_ShowNum(5+48+30,30,temp%10,1,12,1);		//��ʾС������ 
									
				temp=yaw*10;
				if(temp<0)
				{
					OLED_ShowString(5+48,40,"-",12,1);		//��ʾ����
					temp=-temp;		//תΪ����
				}else OLED_ShowString(5+48,40," ",12,1);		//ȥ������ 
				OLED_ShowNum(5+48+8,40,temp/10,2,12,1);		//��ʾ��������	 
				OLED_ShowString(5+48+24,40,".",12,1);				
				OLED_ShowNum(5+48+30,40,temp%10,1,12,1);		//��ʾС������  
				LED=!LED;//LED��˸
			OLED_Refresh();	
//	}
}
/*
up����������λ��������mpu6050ģ�鴫�Ƕ����ݣ�
һ�����ֱ��ϣ�һ����С���ϣ��Ͳ���pitchͬʱ����3��4�Ŷ��
�κιٷ���ֻ�������1��IIC��ַ��ģ��������ã�
����ͬһ��IIC�������ټ�һ����ͬ��ַ��mpu6050���൱��
����дһ��mpuiic��inv_mpu��inv_mpu_dmp_motion�ļ���
�ֽ׶α���ˮƽ���ޣ����Թ���ʵ���в�ͨT.T��
tb�� GY-25ģ�� ���@Cyber_Mice�ķ�����
����ʵ�������ֱ۵���̬���ƣ�����Ȥ���Բο���
������Ŀ���κ����⻶ӭ��ԭ��Ƶ�����ۣ�����ͨ����B��������^.^��
*/
int main(void)
 {     
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init();	    	 	//��ʱ������ʼ��	  
	uart_init(9600);	 		//���ڳ�ʼ��Ϊ9600
	LED_Init();		  			//��ʼ����LED���ӵ�Ӳ���ӿ�
	Adc_Init();
	Dac1_Init();
//	KEY_Init();					//��ʼ������
	MPU_Init();					//��ʼ��MPU6050
	OLED_Init();
	OLED_ColorTurn(0);//0������ʾ��1 ��ɫ��ʾ
	OLED_DisplayTurn(0);//0������ʾ 1 ��Ļ��ת��ʾ
	delay_ms(100);//�ȴ�ģ���ʼ�����
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
//		adcx=Get_Adc_Average(ADC_Channel_1,3);//��ȡ�Ҷȴ�����adֵ
//		adcx=(float)adcx/4096*33;
//	  printf("dddddd\r\n");
//	  printf("ggggg%f\r\n",adcx);
//		data[1]=adcx;//���ݰ�1λ	
		flexADC = Get_Adc_Average(1,25);	//10��ƽ��ֵ
		flexV=(flexADC*VCC)/4096;
	  flexR= R_DIV * (VCC / flexV-1.0);
		angle_1 = map(flexR, STRAIGHT_RESISTANCE, BEND_RESISTANCE,0, 90.0);	
		printf("Bend:%fdegrees\r\n",angle_1);
		data[1]=angle_1;//���ݰ�1�
		DAC_SetChannel1Data(DAC_Align_12b_R, yaw+180);//����DACֵ
		printf("����:%f\r\n",yaw);

		adcx10 = Get_Adc_Average(10,25);	//10��ƽ��ֵ
		printf("adcx10:%f\r\n",adcx10);

//    printf("Yaw:%f degrees\r\n",yaw);

		
	}
	
}
 long map(long x, long in_min, long in_max, long out_min, long out_max) {
 return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
