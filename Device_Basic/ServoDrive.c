/****************************************************************************
* 文件名称 : ServoDrive.h
* 文件功能 : 驱动舵机的相关 函数
* 注意事项 :
*****************************************************************************/

#include "ServoDrive.h"

int16_Pro Servo_Default_Angle = 860;
int16_Pro Servo_Angle_Now = 0;
int16_Pro Servo_MaxTurn_Angle = 450;
Float_Pro Servo_FeedBack_DirDeviation = 0;				/* !< 摄像头中 的 摄像头方向误差			 		*/

/****************************************************************************
* 函数名称: 舵机初始化函数
* 函数功能 : 将舵机所需要的 PWM波 IO 口 等相关初始化 并将 舵机角度调整为 默认角度
* 输入 :void
* 输出 :void
* 作者 :
*****************************************************************************/
void Servo_Init(void)
{
// #if SERVO_FUNCTION == 1
// 	#warning "函数未重写"		/*!< 重写后请删除此段 */
// #endif
	ftm_pwm_init(FTM1,FTM_CH0,50,0);
	Servo_Set_Angle(Servo_Default_Angle);
}


/****************************************************************************
* 函数名称: 舵机设置角度 为 ang
* 函数功能 : 舵机设置角度 为 ang
* 输入 :角度 数值 ang (45~135)
* 输出 :void
* 作者 :
*****************************************************************************/
void Servo_Set_Angle(int16_Pro ang)
{
// #if SERVO_FUNCTION == 1
// 	#warning "函数未重写"		/*!< 重写后请删除此段 */
// #endif


//使用下面的算法进行优化
#if 0
	ang &= 0X7FFF;
	if(ang < (Servo_Default_Angle-Servo_MaxTurn_Angle))
	{/* !< 小于最小转角 */
		ang = Servo_Default_Angle-Servo_MaxTurn_Angle;		
	}else if(ang > (Servo_Default_Angle+Servo_MaxTurn_Angle))
		{/* !< 大于最大转角 */
			ang = Servo_Default_Angle+Servo_MaxTurn_Angle;
		}
#else
	ang = (ang < (Servo_Default_Angle-Servo_MaxTurn_Angle))?/* !< 小于最小转角 */
			( Servo_Default_Angle-Servo_MaxTurn_Angle):
			((ang > (Servo_Default_Angle+Servo_MaxTurn_Angle))?/* !< 大于最大转角 */
			(ang = Servo_Default_Angle+Servo_MaxTurn_Angle):
			(ang)
			);
#endif
	Servo_Angle_Now = ang;
	u16_Pro temp = (ang + 450) * 1000/1800;			/*!< 根据角度求算真正的占空比  */
	/*!< u16_Pro temp = (ang + 45) * 10000/1800; */
	ftm_pwm_duty(FTM1,FTM_CH0,temp);
}


/****************************************************************************
* 函数名称: 舵机左转函数
* 函数功能 : 将舵机 向左打角度 left 度
* 输入 :左转角度 (0~(默认偏角+最大偏角-当前偏角))
* 输出 :void
* 作者 :
*****************************************************************************/
void Servo_SetLeft_Angle(int16_Pro left)
{
// #if SERVO_FUNCTION == 1
// 	#warning "函数未重写"		/*!< 重写后请删除此段 */
// #endif
	/* !< 减法 */
	if(left < Servo_Angle_Now)
	{
		
		Servo_Set_Angle(Servo_Angle_Now - left);
	}else
	{
		Servo_Set_Angle(0);
	}
}


/****************************************************************************
* 函数名称: 舵机右转函数
* 函数功能 : 将舵机 向左打角度 left 度
* 输入 :右转角度 (0~(当前偏角-(默认偏角-最大偏角)))
* 输出 :void
* 作者 :
*****************************************************************************/
void Servo_SetRight_Angle(int16_Pro right)
{
// #if SERVO_FUNCTION == 1
// 	#warning "函数未重写"		/*!< 重写后请删除此段 */
// #endif
	if((right+Servo_Angle_Now)< 180)
	{
		Servo_Set_Angle(right + Servo_Angle_Now);		
	}
	else
	{
		Servo_Set_Angle(180);
	}
}



