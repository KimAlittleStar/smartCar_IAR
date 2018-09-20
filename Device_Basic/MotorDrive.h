
#ifndef _MOTORDRIVE_H_
#define _MOTORDRIVE_H_

#include "Global_Var.h"
/*------------------------宏定义  区----------------------*/

#define MOTOR_Default_Hz	10000	/* !< 默认电机PWM输出频率 10K Hz */
#define MOTOR_CH1_DUTY 250


/*------------------------函数声明区----------------------*/
void Motor_Init(void);

void Motor_Set_Speed(int16_Pro Motor_Speed);

void Motor_Increase_Speed(int16_Pro Motor_IncValue);

void Motor_Reduce_Speed(int16_Pro Motor_RedValue);

int16_Pro Motor_GetSpeed_Of_Now(void);

#endif

