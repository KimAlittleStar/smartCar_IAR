/****************************************************************************
* 文件名称 : KeyScan.c
* 文件功能 : 初始化按键 相关函数 扫描判断按键
* 注意事项 :
*****************************************************************************/
#include "KeyScan.h"

u8_Pro key_up1 = 0;
u8_Pro key_up2 = 0;
u8_Pro key_up3 = 0;
u8_Pro key_up4 = 0;

/****************************************************************************
* 函数名称: 按键初始化函数 初始化按键
* 函数功能 : 配置 IO 口 使其为 按键使用
* 输入 : void
* 输出 : void
* 作者 :
*****************************************************************************/
void KeyScan_Init(void)
{
	gpio_init(Key_Cancel,GPI,0);
	gpio_init(Key_Confirm,GPI,0);
	gpio_init(Key_Up,GPI,0);
	gpio_init(Key_Down,GPI,0);
	port_init_NoALT(Key_Cancel,PULLUP);
	port_init_NoALT(Key_Confirm,PULLUP);
	port_init_NoALT(Key_Up,PULLUP);
	port_init_NoALT(Key_Down,PULLUP);
	// #if KEY_FUNCTION ==1
	// #warning "函数未重编辑"			/* !< 编辑后请删除此行 */
	// #endif
}


/****************************************************************************
* 函数名称: 确认键 按下状态确定函数
* 函数功能 : 判断 确认键是否按下。
* 输入 :按键扫描模式: KEY_MODE_Once  or  KEY_MODE_Continuous
* 输出 :按键状态 wei按下 为零 == 0  or 按下  非零 != 0
* 作者 :
*****************************************************************************/
u8_Pro Key_Of_Confirm(u8_Pro KEY_MODE)
{
	if(KEY_MODE)key_up1=1;  //支持连按		  
	if(key_up1 &&  Key_Confirm_IN == 0)
	{
		DELAY_MS(10);
		key_up1=0;
		if( Key_Confirm_IN== 0)
			return 1;
	}else if( Key_Confirm_IN==1)
			key_up1=1; 	   
 	return 0;// 无按键按下
	// u8_Pro key_precess = 0;
	// #if KEY_FUNCTION ==1
	// #warning "函数未重编辑"			/* !< 编辑后请删除此行 */
	// #endif
	
}


/****************************************************************************
* 函数名称: 取消键 按下状态确定函数
* 函数功能 : 判断 取消键是否已经按下
* 输入 :按键扫描模式: KEY_MODE_Once  or  KEY_MODE_Continuous
* 输出 :按键状态 已按下 为零 == 0  or 按下  非零 != 0
* 作者 :
*****************************************************************************/
u8_Pro Key_Of_Cancel(u8_Pro KEY_MODE)
{
	if(KEY_MODE)key_up2=1;  //支持连按		  
	if(key_up2 &&  Key_Cancel_IN == 0)
	{
		DELAY_MS(10);
		key_up2=0;
		if( Key_Cancel_IN== 0)
			return 1;
	}else if( Key_Cancel_IN==1)
			key_up2=1; 	   
 	return 0;// 无按键按下

	// u8_Pro key_precess = 0;
	// #if KEY_FUNCTION ==1
	// #warning "函数未重编辑"			/* !< 编辑后请删除此行 */
	// #endif
	// return key_precess;
}


/****************************************************************************
* 函数名称: 向上按键状态确定函数
* 函数功能 : 判断向上按键是否按下
* 输入 :按键扫描模式: KEY_MODE_Once  or  KEY_MODE_Continuous
* 输出 :按键状态 已按下 为零 == 0  or 按下  非零 != 0
* 作者 :
*****************************************************************************/
u8_Pro Key_Of_Up(u8_Pro KEY_MODE)
{
	if(KEY_MODE)key_up3=1;  //支持连按
	if(key_up3 &&  Key_Up_IN == 0)
	{
		DELAY_MS(10);
		key_up3=0;
		if( Key_Up_IN== 0)
			return 1;
	}else if( Key_Up_IN==1)
			key_up3=1; 	   
 	return 0;// 无按键按下

	// u8_Pro key_precess = 0;
	// #if KEY_FUNCTION ==1
	// #warning "函数未重编辑"			/* !< 编辑后请删除此行 */
	// #endif
	// return key_precess;
	
}


/****************************************************************************
* 函数名称: 向下按键状态确定函数
* 函数功能 : 判断向下按键是否按下
* 输入 :按键扫描模式: KEY_MODE_Once  or  KEY_MODE_Continuous
* 输出 :按键状态 已按下 为零 == 0  or 按下  非零 != 0
* 作者 :
*****************************************************************************/
u8_Pro Key_Of_Down(u8_Pro KEY_MODE)
{
	if(KEY_MODE)key_up4=1;  //支持连按
	if(key_up4 &&  Key_Down_IN == 0)
	{
		DELAY_MS(10);
		key_up4=0;
		if( Key_Down_IN== 0)
			return 1;
	}else if( Key_Down_IN==1)
			key_up4=1; 	   
 	return 0;// 无按键按下

	// u8_Pro key_precess = 0;
	// #if KEY_FUNCTION ==1
	// #warning "函数未重编辑"			/* !< 编辑后请删除此行 */
	// #endif
	// return key_precess;
}

