/****************************************************************************
* 文件名称 : 直流电机驱动相关文件 函数
* 文件功能 : 实现 电机驱动的所有功能
* 注意事项 :
*****************************************************************************/
#include "MotorDrive.h"

u16_Pro Motor_Speed_Now = 0;		/* !< 电机当前速度 0~最大计数值 */
u32_Pro Motor_FeedBack_SpeedTime;	/* !< 编码器获取的电机速度反馈  */


/****************************************************************************
* 函数名称: 直流电机驱动初始化函数
* 函数功能 : 将直流电机先关定时器 IO口初始化
* 输入 :void
* 输出 :void
* 作者 :
*****************************************************************************/
void Motor_Init(void)
{
	ftm_pwm_init(FTM0,FTM_CH2,10000,MOTOR_CH1_DUTY);
	
	ftm_pwm_init(FTM0,FTM_CH1,10000,MOTOR_CH1_DUTY);
	
	gpio_init(PTD15,GPO,HIGH);
	gpio_init(PTE27,GPO,HIGH);

	// #if MOTOR_FUNCTION ==1
	// #warning "函数未重编辑"			/* !< 编辑后请删除此行 */
	// #endif
	
}




/****************************************************************************
* 函数名称: 直流电机设置 绝对速度函数
* 函数功能 : 将直流电机 绝对速度设置 占空比为 ：Motor_Speed/最大计数值
*			函数先判断是否 参数在合理范围内 如在：修改电机现在速度值
*										如不在：处理完成之后在修改电机现在速度值
* 输入 :Motor_Speed (0~最大计数值)
* 输出 :void
* 作者 :
*****************************************************************************/
void Motor_Set_Speed(int16_Pro Motor_Speed)
{
	Motor_Speed = (Motor_Speed>1000)?(1000):(Motor_Speed);
	Motor_Speed_Now = Motor_Speed;
	ftm_pwm_duty(FTM0,FTM_CH2,Motor_Speed);
//	ftm_pwm_duty(FTM0,FTM_CH1,Motor_Speed);

	// Motor_Speed_Now = Motor_Speed;
	// #if MOTOR_FUNCTION ==1
	// #warning "函数未重编辑"			/* !< 编辑后请删除此行 */
	// #endif
}


/****************************************************************************
* 函数名称: 直流电机增加 相对的速度
* 函数功能 : 使电机加速 加速相对值为：Motor_IncValue/最大计数值
* 输入 :Motor_Value (0~(最大计数值-Motor_Speed_Now))
* 输出 :void
* 作者 :
*****************************************************************************/
void Motor_Increase_Speed(int16_Pro Motor_IncValue)
{
	Motor_Speed_Now += Motor_IncValue;
	Motor_Set_Speed(Motor_Speed_Now);
//	#if MOTOR_FUNCTION ==1
//	#warning "函数未重编辑"			/* !< 编辑后请删除此行 */
//	#endif
}

/****************************************************************************
* 函数名称: 直流电机减少 相对的速度
* 函数功能 : 使电机减速 减速相对值为：Motor_RedValue/最大计数值
* 输入 :Motor_Value (0~Motor_Speed_Now)
* 输出 :void
* 作者 :
*****************************************************************************/
void Motor_Reduce_Speed(int16_Pro Motor_RedValue)
{
	if(Motor_RedValue < Motor_Speed_Now)
	{
		Motor_Speed_Now -= Motor_RedValue;
		Motor_Set_Speed(Motor_Speed_Now);
	}
	
//	#if MOTOR_FUNCTION ==1
//	#warning "函数未重编辑"			/* !< 编辑后请删除此行 */
//	#endif
//
}


/****************************************************************************
* 函数名称: 获取当前电机的转速(推荐只用此函数 直接 读取 Motor_Speed_Now 即可)
* 函数功能 : 获取当前电机的转速
* 输入 :void
* 输出 :当前电机转速 Motor_Speed_Now
* 作者 :
*****************************************************************************/
int16_Pro Motor_GetSpeed_Of_Now(void)
{
	return Motor_Speed_Now;
}

