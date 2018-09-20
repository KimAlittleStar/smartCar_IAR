
#ifndef _PID_PRPCESS_H_
#define _PID_PRPCESS_H_

#include "Global_Var.h"

/*------------------------�궨��  ��----------------------*/

#define PID_Motor_PROCESSTYPE 2   /*!< PID�������� 0 ��P���� 1 PI ���� 2 PID ���� */
#define PID_Servo_PROCESSTYPE 1 


struct PID_DIV
{
    u32_Pro setPoint;
    u32_Pro realPoint;
    u32_Pro const_P;         /*!< ��������  ���λ ΪС��λ������ʱӦ���ư�λ */
    u32_Pro const_I;         /*!< ͬ�� */
    u32_Pro const_D;         /*!< ͬ�� */
    int32_Pro error_s[3];      /*!< ����һ����������һ����� */
    int32_Pro error_sum;
};


/*------------------------����������----------------------*/

void PID_NVIC_init(void);

void PID_Servo_AngleSet(void);

void PID_Motor_SpeedSet(void);

void PID_SET_begin(void);

void PID_SET_stop(void);

void PID_HandleProcess(void);


#endif


