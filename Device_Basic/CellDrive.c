/***************************************************************************
* 文件名称 : CellDrive.c
* 文件功能 : 检测电池 点亮。低于多少电量时 主板报警
* 注意事项 :
*****************************************************************************/
#include "CellDrive.h"
#include "EncordDrive.h"
#include "MotorDrive.h"
#include "OledDrive.h"
#include "KeyScan.h"
u16_Pro Cell_Vsum = 0;
/****************************************************************************
* 函数名称: 电池检测初始化函数
* 函数功能 : 将检测电池的 ADC模块初始化
* 输入 :void
* 输出 :void
* 作者 :
*****************************************************************************/
void Cell_Init(void)
{
	// #if CELL_FUNCTION ==1
	// #warning "函数未重编辑"			/* !< 编辑后请删除此行 */
	// #endif
	u8_Pro i;
	adc_init(ADC0_SE16);	      // ADC0_SE16
	gpio_init(PTE24,GPO,LOW);
	Cell_BEEP_ON;
	DELAY_MS(200);
	Cell_BEEP_OFF;
	i = Key_Of_Cancel(KEY_MODE_Once);
	DELAY_MS(500);
	i = Key_Of_Cancel(KEY_MODE_Continuous);
	if(i == 0)
	{
		pit_init_ms(PIT2,200);
		/*!< 重定向中断函数 */
		set_vector_handler(PIT2_VECTORn,Cell_PIT2_Protect_NVIC);
		enable_irq(PIT2_IRQn);
		/*!< 使能中断函数 */
	}else
	{
	  	Cell_BEEP_ON;
		DELAY_MS(200);
		Cell_BEEP_OFF;
	}


}


/****************************************************************************
* 函数名称: 得到电池的电压值
* 函数功能 : 从ADC 模块中得到 电池当前电压
*	得到的 确切值  3300 实际值为  3.300 V
* 输入 :void
* 输出 :当前电池电压 压值3300 实际值为  3.300 V
* 作者 :
*****************************************************************************/
u16_Pro Cell_GetBatteryPer(void)
{
	u16_Pro CELL_A = 0;
// 	#if CELL_FUNCTION ==1
// 	#warning "函数未重编辑"			/* !< 编辑后请删除此行 */
// #endif
	CELL_A =  adc_once(ADC0_SE16,ADC_16bit);			/* 去ADC0 模块的 SE8 通道 模拟量 数据大小为 8位 */
	
	return 3300*CELL_A/0xFFFF;
}

/**********************************************************************************
* 函数名称: void Cell_Protection(void);
* 函数功能: 电池电源保护功能，防止短接电流或这 过放电池
* 函数输入: void input:
* 函数输出: void output:
* 作者 :author:@Kim_alittle_star
**********************************************************************************/
void Cell_Protection(void)
{
  static u8_Pro Protect[2] = {0,0};
	if(Protect[0] > 3)
	{
		Protect[0] = 0;
		if((Cell_Vsum>>2)>2065 && Protect[1]<=2)
		{
		  Protect[1] = 0;
		  Cell_Vsum = 0;
		}else
		{
			if((Cell_Vsum>>2)<=2265)
		  		Cell_Warning("Voltage");
			else
			{
				Cell_Warning("speed");
			}
		}

	}else
	{
		Protect[0]++;
		Cell_Vsum += Cell_GetBatteryPer();
		if(Motor_Speed_Now > 850 && Encord_GetMotorTime()<50)
		{
			Protect[1]++;
		}
	}

}

void Cell_Warning(String_Pro warning)
{
	u8_Pro* B = (u8_Pro* )BBmp;
	u8_Pro i,j;
	Motor_Set_Speed(MOTOR_CH1_DUTY);
	Encord_GetSpeed_Close();
	DisableInterrupts;	/*!< 屏蔽中断 报警模式 */
	while(1)
	{
		for(i = 0;i<8;i++)
		{
			for(j = 0;j<128;j++)
			{
				OLED_Set_Byte(j,i*8,*B,POINT_STATU_SET);
				B++;
			}
		}
		OLED_Refrech_Gram();
		PTE24_OUT = 0;
		DELAY_MS(750);
		OLED_ResetGRAM();
		OLED_Show_String(0,0,warning,charSize16);
		OLED_Refrech_Gram();
		PTE24_OUT = 1;
		DELAY_MS(250);
		OLED_ResetGRAM();
		B =(u8_Pro* ) BBmp;
	}
}

void Cell_PIT2_Protect_NVIC(void)
{
	static u8_Pro i = 0;
	PIT_Flag_Clear(PIT2);		 /* 清除 定时中断 */
	/* 如果将 PIT_Flag_Clear 放入里面 那么 中断将会连续进行两次 */
	// DisableInterrupts;		/* 不能将所有中断都 屏蔽 只能屏蔽 定时器捕获中断 */

	Cell_Protection();
	i++;
	if(i == 5)
	{
		OLED_Refrech_FPS = OV_Refresh_FPS;
		OV_Refresh_FPS = 0;
		i = 0;
//		printf("%d",OLED_Refrech_FPS);
	}
}