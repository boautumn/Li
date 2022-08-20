
#include "include.h"

u8 state=0;
int kl;

u16 a1=0;
u16 a11=0;
u16 a2=0;
u16 a22=0;
uint8 ps_ok;
uint16 ServoPwmDutySet1[][8]={
															{1500,800,1500,2000,2200,1900,1500,1500},//��λ
															{1500,800,1500,2000,2300,1900,1800,1500},//��ת
	                            {1500,800,1500,1550,2300,1000,1800,1500},//����
                              {1500,1900,1500,1550,2300,1000,1800,1500},//�н�
															{1500,1900,1500,1550,2100,1000,1800,1500},//̧��
															{1500,1900,1500,1550,2100,1000,1500,1500},//��ת����
															{1500,1900,1500,1550,2200,1000,1500,1500},//�½�
															{1500,1900,1500,1550,2265,1000,1500,1500},//�½��ȴ�
															{1500,800,1500,1550,2265,1000,1500,1500},//����
															{1500,800,1500,2000,2200,1900,1500,1500},//��λ

                               };//1�Ŷ��1800����ץ����
//                                    1    2  (3    4     5)    6
uint16 ServoPwmDutySet2[][8]={
															 {1500,800,1500,2000,2200,1900,1500,1500},//��λ
															 {1500,800,1500,2000,2300,1900,1200,1500},//��ת
                               {1500,800,1500,1550,2300,1000,1200,1500},//����
                               {1500,1900,1500,1550,2300,1000,1200,1500},//�н�
														 	 {1500,1900,1500,1750,2100,1100,1200,1500},//̧��
															 {1500,1900,1500,1750,2100,1100,1500,1500},//��ת����
															 {1500,1900,1500,1750,2165,1100,1500,1500},//�½�
															 {1500,800,1500,1750,2165,1100,1500,1500},//����
															 {1500,800,1500,2000,2200,1900,1500,1500},//��λ---------�ڶ�����
															 {1500,800,1500,2000,2300,1900,1800,1500},//��ת
	                             {1500,800,1500,1550,2300,1000,1800,1500},//����
                               {1500,1900,1500,1550,2300,1000,1800,1500},//�н�
															 {1500,1900,1500,1710,2100,1200,1800,1500},//̧��
															 {1500,1900,1500,1710,2100,1200,1500,1500},//��ת����
															 {1500,1900,1500,1710,2170,1200,1500,1500},//�½�
															 {1500,1900,1500,1710,2200,1200,1500,1500},//�½��ȴ�
															 {1500,800,1500,1710,2200,1200,1500,1500},//����
															 {1500,800,1500,2000,2200,1900,1500,1500},//��λ
                               };
int main(void)
{
	a1=0;
  a11=0;
  a2=0;
  a22=0;
	ps_ok = 1;
	SystemInit(); 			 //ϵͳʱ�ӳ�ʼ��Ϊ72M	  SYSCLK_FREQ_72MHz
	InitDelay(72);	     //��ʱ��ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	InitPWM();
	InitTimer2();//���ڲ���100us�Ķ�ʱ�ж�
	InitUart1();//������PC�˽���ͨ��
	uart3_init();
	InitADC();
	InitLED();
	InitKey();
	InitBuzzer();
//	ps_ok = InitPS2();//PS2��Ϸ�ֱ���������ʼ��
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
		if(state==1)//ץȡһ��Ŀ�ֹ꣬ͣ����
		{
				USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);//�����ж�
				a2=0;
				a22=0;
				ServoPwmDutySet[1]=ServoPwmDutySet1[a1][1];	
				ServoPwmDutySet[2]=ServoPwmDutySet1[a1][2];
				ServoPwmDutySet[3]=ServoPwmDutySet1[a1][3];
				ServoPwmDutySet[4]=ServoPwmDutySet1[a1][4];
				ServoPwmDutySet[5]=ServoPwmDutySet1[a1][5];
				ServoPwmDutySet[6]=ServoPwmDutySet1[a1][6];
				ServoPwmDutyHaveChange=TRUE;
				if(systimes%10==0)//ÿ20ms��һ�αȽϣ����ݾ����������
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
//					printf("��ɶ���1\r\n");
				}
			
		}
		
		else if(state==2)//ץȡ����Ŀ�꣬��������
		{
			USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);//�����ж�
			a1=0;
			a11=0;
			ServoPwmDutySet[1]=ServoPwmDutySet2[a2][1];	
			ServoPwmDutySet[2]=ServoPwmDutySet2[a2][2];
			ServoPwmDutySet[3]=ServoPwmDutySet2[a2][3];
			ServoPwmDutySet[4]=ServoPwmDutySet2[a2][4];
			ServoPwmDutySet[5]=ServoPwmDutySet2[a2][5];
			ServoPwmDutySet[6]=ServoPwmDutySet2[a2][6];
			ServoPwmDutyHaveChange=TRUE;
			if(systimes%10==0)//ÿ20ms��һ�αȽϣ����ݾ����������
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
//				printf("��ɶ���2\r\n");
			}		
			
		}
		
		else if(state==3)//�л���п��ƣ�50��
		{
			a1=0;
			a11=0;
			a2=0;
			a22=0;
			USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�����ж�
//			TaskRun(ps_ok);
			TaskPCMsgHandle();
			if(systimes%2==0)//ÿ20ms��һ�αȽϣ����ݾ����������
			{
				ServoPwmDutyCompare();	
			}
		}	
		
		else if(state==0)
		{
			USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);//�����ж�
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
			if(systimes%20==0)//ÿ20ms��һ�αȽϣ����ݾ����������
			{
				ServoTime=10000;
				ServoPwmDutyCompare();	
				CheckBatteryVoltage();
			}

		}
	}
}


