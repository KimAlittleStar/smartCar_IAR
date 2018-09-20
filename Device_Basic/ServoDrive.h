
#ifndef _SERVODRIVE_H_
#define _SERVODRIVE_H_

#include "Global_Var.h"

/*------------------------宏定义  区----------------------*/



/*------------------------函数声明区----------------------*/

void Servo_Init(void);

void Servo_Set_Angle(int16_Pro ang);

void Servo_SetLeft_Angle(int16_Pro left);

void Servo_SetRight_Angle(int16_Pro right);

#endif

