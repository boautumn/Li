#ifndef __INFRARED_H
#define __INFRARED_H	 
#include "sys.h"

#define INFRARED_STATE() GPIO_ReadInputDataBit(infrared_PORT,infrared_PIN)//??????

#define infrared_PORT GPIOA
#define infrared_PIN GPIO_Pin_8
#define infrared_RCC RCC_APB2Periph_GPIOA

#define INFRARED_STATE() GPIO_ReadInputDataBit(infrared_PORT,infrared_PIN)//??????


void infrared_INIT(void);
		 				    
#endif


//PA8
