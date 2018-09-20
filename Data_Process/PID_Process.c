/****************************************************************************
* �ļ����� : PID_Process.c
* �ļ����� : �� ��ǰ���߷��򣬶���Ƕȡ�����ٶ� ��·��� ���ϴ���
*			ʹ��PID����ʵ�� ��ǰ ת�� ת���ٶ� ���ȵȡ�
* ע������ :
*****************************************************************************/

#include "PID_Process.h"
#include "MotorDrive.h"
#include "ServoDrive.h"
#include "EncordDrive.h"
#include "CellDrive.h"

int32_Pro Motor_Default_Setpoint;
struct PID_DIV Motor_div;
struct PID_DIV Servo_div;
void PID_HandleProcess(void)
{
	PIT_Flag_Clear(PIT1);
	PID_Motor_SpeedSet();
}

/**********************************************************************************
* ��������: PID_NVIC_init
* ��������: ��ʼ����ʱ�� pit1 ʹ��PID����Ϊ�����Ե���
* ��������: void input:
* �������: void output:
* ���� :author:@Kim_alittle_star
**********************************************************************************/
void PID_NVIC_init(void)
{
#if PID_FUNCTION == 1
	#warning "����δ��д"		/*!< ��д����ɾ���˶� */
#endif
	pit_init_ms(PIT1,20);
	disable_irq(PIT1_IRQn);
	set_vector_handler(PIT1_VECTORn,PID_HandleProcess);
}

/****************************************************************************
* ��������: ����Ƕ����ú���
* �������� : ��������ڵĽǶ� �� ���ݴ���ĽǶ� �����ٶ� �������
* ���� :���ڵ���ٶ�
* ��� :void
* ���� :
*****************************************************************************/
int32_Pro Servo_dError = 0;
int32_Pro Servo_result = 0;
void PID_Servo_AngleSet(void)
{

//	Servo_div.realPoint = Encord_GetMotorTime();
//	Servo_div.realPoint = 800;
	/*!< ƫ�� */
	Servo_div.error_s[0] = (int32_Pro)Servo_div.setPoint-(int32_Pro)Servo_div.realPoint;
	Servo_div.error_s[0] = 	(Servo_div.error_s[0]<5)
						?	(
								(Servo_div.error_s[0]<(-5))
							?	(Servo_div.error_s[0])
							:	(0)
							)
						:	(Servo_div.error_s[0]);
	/*!< ���� */
	Servo_div.error_sum += Servo_div.error_s[0];
	/*!< ΢�� */
	Servo_dError = Servo_div.error_s[1]-Servo_div.error_s[2];

	Servo_div.error_s[2] = Servo_div.error_s[1];
	Servo_div.error_s[1] = Servo_div.error_s[0];
#if PID_Servo_PROCESSTYPE==0 
/*!< ��Pģʽ�µĵ��� */
	Servo_result = Servo_div.error_s[0] * Servo_div.const_P;
#elif PID_Servo_PROCESSTYPE==1
/*!< PI ģʽ�µĵ��� */
	Servo_result =  Servo_div.error_s[0] * Servo_div.const_P
				+	Servo_div.error_sum * Servo_div.const_I;
#elif PID_Servo_PROCESSTYPE==2
/*!< PIDģʽ�µĵ��� */
#endif
	Servo_div.realPoint = Servo_div.setPoint;
	if(Servo_result < 0)
	{
	  Servo_result = 0-Servo_result;
	  Servo_result>>=8;
//	  Servo_result = 0-Servo_result;
	}else
	{
	  Servo_result>>=8;
	  Servo_result = 0-Servo_result;
	}
	Servo_Set_Angle(Servo_Angle_Now+Servo_result);
#if PID_FUNCTION == 1
	#warning "����δ��д"		/*!< ��д����ɾ���˶� */
#endif
}


/****************************************************************************
* ��������: ����ٶȵ��� ����
* �������� : ���ݵ����ǰ�ٶȡ���·���ͽ����ٶȵ���
* ���� :
* ��� :
* ���� :
*****************************************************************************/
int32_Pro Motor_dError = 0;
int32_Pro Motor_result = 0;
void PID_Motor_SpeedSet(void)
{
	Motor_div.realPoint = Encord_GetMotorTime();
//	Motor_div.realPoint = 800;
	/*!< ƫ�� */
	Motor_div.error_s[0] = (int32_Pro)Motor_div.setPoint-(int32_Pro)Motor_div.realPoint;
	Motor_div.error_s[0] = 	(Motor_div.error_s[0]<5)
						?	(
								(Motor_div.error_s[0]<(-5))
							?	(Motor_div.error_s[0])
							:	(0)
							)
						:	(Motor_div.error_s[0]);
	/*!< ���� */
	Motor_div.error_sum += Motor_div.error_s[0];
	/*!< ΢�� */
	Motor_dError = Motor_div.error_s[1]-Motor_div.error_s[2];

	Motor_div.error_s[2] = Motor_div.error_s[1];
	Motor_div.error_s[1] = Motor_div.error_s[0];

#if	PID_Motor_PROCESSTYPE==0
/*!< ��Pģʽ�µĵ��� */
	Motor_result = Motor_div.error_s[0] * Motor_div.const_P;
#elif PID_Motor_PROCESSTYPE==1
/*!< PI ģʽ�µĵ��� */
	Motor_result =  Motor_div.error_s[0] * Motor_div.const_P
				+	motor_div.error_sum * Motor_div.const_I;

#elif PID_Motor_PROCESSTYPE==2
/*!< PIDģʽ�µĵ��� */
	Motor_result =  Motor_div.error_s[0] * Motor_div.const_P
				+	Motor_div.error_sum * Motor_div.const_I
				+	Motor_div.const_D * Motor_dError;
 
#endif
	if(Motor_result < 0)
	{
	  Motor_result = 0-Motor_result;
	  Motor_result>>=8;
	  Motor_result = 0-Motor_result;
	}else
	{
	  Motor_result>>=8;
	}
//	 = (int32_Pro)((Motor_result&0x80000000) | ((Motor_result&0x7FFFFFFF)>>8));
	Motor_Set_Speed(((Motor_Speed_Now + Motor_result)>0)?(Motor_Speed_Now + Motor_result):(MOTOR_CH1_DUTY));


#if PID_FUNCTION == 1
	#warning "����δ��д"		/*!< ��д����ɾ���˶� */
#endif
}

void PID_SET_begin(void)
{
	enable_irq(PIT1_IRQn);
}

void PID_SET_stop(void)
{
	disable_irq(PIT1_IRQn);
}


