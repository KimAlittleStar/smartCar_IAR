/****************************************************************************
* 文件名称 : EncordDrive.h
* 文件功能 : 编码器的基本控制功能
* 注意事项 :
*****************************************************************************/

#include "EncordDrive.h"

u32_Pro Encord_IRQ_COUNT = 0;
u32_Pro Encord_Hz = 0;
u16_Pro Encord_NVIC_Time_Weith = 0;
/****************************************************************************
* 函数名称 : 编码器驱动初始化
* 函数功能 : 初始化编码器 扫描频率1M 赫兹 不滤波
*			 接收时间使用中断服务函数处理
* 输入 :void
* 输出 :void
* 作者 :
*****************************************************************************/
void Encord_Init(void)
{
// 	#if ENCORD_FUNCTION ==1
// 	#warning "函数未重编辑"			/* !< 编辑后请删除此行 */
// #endif
	ftm_input_init(FTM2,FTM_CH0,FTM_Rising,FTM_PS_1);	/* 定义 TFM2 为 输入捕获，上升沿 开始，不滤波 */
	set_vector_handler(FTM2_VECTORn, NVIC_Encord_FTM_IRQHander);
	set_vector_handler(PIT0_VECTORn,NVIC_Encord_PIT_IRQHander);
//	enable_irq(FTM2_IRQn);
	pit_init_ms(PIT0,20);			//定时中断  1 秒一次;
	Encord_NVIC_Time_Weith = (1000/20);		/* 周期计算函数 ，即 一秒钟内有几次 中断 */
//	enable_irq(PIT0_IRQn);
}

/****************************************************************************
* 函数名称: 
* 函数功能 : 
* 输入 :
* 输出 :
* 作者 :
*****************************************************************************/
u32_Pro Encord_GetMotorTime(void)
{
//	Motor_FeedBack_SpeedTime =0;
	
	// #if ENCORD_FUNCTION ==1
	// #warning "函数未重编辑"			/* !< 编辑后请删除此行 */
	// #endif
	return Encord_Hz;
}


/****************************************************************************
* 函数名称: 编码器中断等级等初始化函数
* 函数功能 : 初始化 编码器相关中断 结构体函数
* 输入 : void
* 输出 : void
* 作者 :
*****************************************************************************/
void Encord_NVIC_Init(void)
{
	#if ENCORD_FUNCTION ==1
	#warning "函数未重编辑"			/* !< 编辑后请删除此行 */
	#endif
}


void NVIC_Encord_FTM_IRQHander(void)
{
 //   u8_Pro s = FTM2_STATUS;          //读取捕捉和比较状态  All CHnF bits can be checked using only one read of STATUS.
 //   u8_Pro CHn;


 //   CHn = 0;						/* 检测通道0  中断状态 */
    if( FTM2_STATUS & 0x01 )		/* ftm2_status & 1<<CHn */
    {	     
  /*     用户任务       */			
       Encord_IRQ_COUNT++;
		FTM2_STATUS = 0x00;				/* 清空中断状态		 */
		
        /*********************/
    }
}
void NVIC_Encord_PIT_IRQHander(void)
{
	PIT_Flag_Clear(PIT0);		 /* 清除 定时中断 */
	/* 如果将 PIT_Flag_Clear 放入里面 那么 中断将会连续进行两次 */
	// DisableInterrupts;		/* 不能将所有中断都 屏蔽 只能屏蔽 定时器捕获中断 */
	disable_irq(FTM2_IRQn);
	Encord_Hz = Encord_IRQ_COUNT;
	Encord_IRQ_COUNT = 0;
	enable_irq(FTM2_IRQn);

	
	// EnableInterrupts;
}
/**********************************************************************************
* 函数名称: Encord_GetSpeed_Prepare
* 函数功能: 开启编码器采集开关
* 函数输入: void input:
* 函数输出: void output:
* 作者 :author:@Kim_alittle_star
**********************************************************************************/
void Encord_GetSpeed_Prepare(void)
{
	enable_irq(FTM2_IRQn);
	enable_irq(PIT0_IRQn);
}

/**********************************************************************************
* 函数名称: Encord_GetSpeed_Close
* 函数功能: 关闭编码器采集开关
* 函数输入: void input:
* 函数输出: void output:
* 作者 :author:@Kim_alittle_star
**********************************************************************************/
void Encord_GetSpeed_Close(void)
{
	disable_irq(FTM2_IRQn);
	disable_irq(PIT0_IRQn);
}
