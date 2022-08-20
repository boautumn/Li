#ifndef _BLUETOOTH_H_
#define _BLUETOOTH_H_

typedef unsigned char	u8, U8, uint8, UINT8, BYTE;

extern u8 arr1[7];
void USART3_IRQHandler(void);     	//串口3中断服务程序
void uart3_init(void);

void InitUart3(void);
void TaskBLEMsgHandle(void);

#endif

