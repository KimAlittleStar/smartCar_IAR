
#ifndef _PID_PRPCESS_H_
#define _PID_PRPCESS_H_

#include "Global_Var.h"

/*------------------------宏定义  区----------------------*/

#define PID_Motor_PROCESSTYPE 2   /*!< PID调节类型 0 单P调节 1 PI 调节 2 PID 调节 */
#define PID_Servo_PROCESSTYPE 1 


struct PID_DIV
{
    u32_Pro setPoint;
    u32_Pro realPoint;
    u32_Pro const_P;         /*!< 参数整定  后八位 为小数位，运算时应左移八位 */
    u32_Pro const_I;         /*!< 同上 */
    u32_Pro const_D;         /*!< 同上 */
    int32_Pro error_s[3];      /*!< 误差，上一个的误差，在上一个误差 */
    int32_Pro error_sum;
};


/*------------------------函数声明区----------------------*/

void PID_NVIC_init(void);

void PID_Servo_AngleSet(void);

void PID_Motor_SpeedSet(void);

void PID_SET_begin(void);

void PID_SET_stop(void);

void PID_HandleProcess(void);


#endif


