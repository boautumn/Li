#ifndef __IIC_H
#define __IIC_H
#include "sys.h"

//IO��������	 
#define IIC_SCL    PBout(3) //SCL
#define IIC_SDA    PBout(4) //SDA	 
#define READ_SDA   PBin(4)  //����SDA 

#define Slave_Addr 0x36
#define Write_Bit 0
#define Read_Bit 1
#define Angle_Hight_Register_Addr 0x0C
#define Angle_Low_Register_Addr 0x0D

//IO��������
#define SDA_IN()  {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=(u32)8<<21;}
#define SDA_OUT() {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=(u32)3<<21;}
//IIC���в�������
void IIC_Init(void);                //��ʼ��IIC��IO��				 
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	  

void AS5600_Init(void);
u16 AS5600_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf);
float Get_Angle(void);
#endif


