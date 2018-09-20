#ifndef _ENCORDDRIVE_H_
#define _ENCORDDRIVE_H_

#include "Global_Var.h"

/*------------------------宏定义  区----------------------*/

extern u32_Pro Encord_Time_Value;

/*------------------------函数声明区----------------------*/

void Encord_Init(void);

u32_Pro Encord_GetMotorTime(void);

void Encord_NVIC_Init(void);

void NVIC_Encord_FTM_IRQHander(void);

void NVIC_Encord_PIT_IRQHander(void);

void Encord_GetSpeed_Close(void);

void Encord_GetSpeed_Prepare(void);


#endif

