/****************************************************************************
* 文件名称 : PID_Process.c
* 文件功能 : 将 当前中线方向，舵机角度。电机速度 道路情况 整合处理。
*			使用PID调节实现 提前 转向 转向速度 均匀等。
* 注意事项 :
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
* 函数名称: PID_NVIC_init
* 函数功能: 初始化定时器 pit1 使得PID调节为周期性调节
* 函数输入: void input:
* 函数输出: void output:
* 作者 :author:@Kim_alittle_star
**********************************************************************************/
void PID_NVIC_init(void)
{
#if PID_FUNCTION == 1
	#warning "函数未重写"		/*!< 重写后请删除此段 */
#endif
	pit_init_ms(PIT1,20);
	disable_irq(PIT1_IRQn);
	set_vector_handler(PIT1_VECTORn,PID_HandleProcess);
}

/****************************************************************************
* 函数名称: 舵机角度设置函数
* 函数功能 : 将舵机现在的角度 和 数据处理的角度 根据速度 进项调整
* 输入 :现在电机速度
* 输出 :void
* 作者 :
*****************************************************************************/
int32_Pro Servo_dError = 0;
int32_Pro Servo_result = 0;
void PID_Servo_AngleSet(void)
{

//	Servo_div.realPoint = Encord_GetMotorTime();
//	Servo_div.realPoint = 800;
	/*!< 偏差 */
	Servo_div.error_s[0] = (int32_Pro)Servo_div.setPoint-(int32_Pro)Servo_div.realPoint;
	Servo_div.error_s[0] = 	(Servo_div.error_s[0]<5)
						?	(
								(Servo_div.error_s[0]<(-5))
							?	(Servo_div.error_s[0])
							:	(0)
							)
						:	(Servo_div.error_s[0]);
	/*!< 积分 */
	Servo_div.error_sum += Servo_div.error_s[0];
	/*!< 微分 */
	Servo_dError = Servo_div.error_s[1]-Servo_div.error_s[2];

	Servo_div.error_s[2] = Servo_div.error_s[1];
	Servo_div.error_s[1] = Servo_div.error_s[0];
#if PID_Servo_PROCESSTYPE==0 
/*!< 单P模式下的调节 */
	Servo_result = Servo_div.error_s[0] * Servo_div.const_P;
#elif PID_Servo_PROCESSTYPE==1
/*!< PI 模式下的调节 */
	Servo_result =  Servo_div.error_s[0] * Servo_div.const_P
				+	Servo_div.error_sum * Servo_div.const_I;
#elif PID_Servo_PROCESSTYPE==2
/*!< PID模式下的调节 */
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
	#warning "函数未重写"		/*!< 重写后请删除此段 */
#endif
}


/****************************************************************************
* 函数名称: 电机速度调节 函数
* 函数功能 : 根据电机当前速度。道路类型进行速度调节
* 输入 :
* 输出 :
* 作者 :
*****************************************************************************/
int32_Pro Motor_dError = 0;
int32_Pro Motor_result = 0;
void PID_Motor_SpeedSet(void)
{
	Motor_div.realPoint = Encord_GetMotorTime();
//	Motor_div.realPoint = 800;
	/*!< 偏差 */
	Motor_div.error_s[0] = (int32_Pro)Motor_div.setPoint-(int32_Pro)Motor_div.realPoint;
	Motor_div.error_s[0] = 	(Motor_div.error_s[0]<5)
						?	(
								(Motor_div.error_s[0]<(-5))
							?	(Motor_div.error_s[0])
							:	(0)
							)
						:	(Motor_div.error_s[0]);
	/*!< 积分 */
	Motor_div.error_sum += Motor_div.error_s[0];
	/*!< 微分 */
	Motor_dError = Motor_div.error_s[1]-Motor_div.error_s[2];

	Motor_div.error_s[2] = Motor_div.error_s[1];
	Motor_div.error_s[1] = Motor_div.error_s[0];

#if	PID_Motor_PROCESSTYPE==0
/*!< 单P模式下的调节 */
	Motor_result = Motor_div.error_s[0] * Motor_div.const_P;
#elif PID_Motor_PROCESSTYPE==1
/*!< PI 模式下的调节 */
	Motor_result =  Motor_div.error_s[0] * Motor_div.const_P
				+	motor_div.error_sum * Motor_div.const_I;

#elif PID_Motor_PROCESSTYPE==2
/*!< PID模式下的调节 */
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
	#warning "函数未重写"		/*!< 重写后请删除此段 */
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


