#ifndef _GLOBAL_VAR_H_
#define _GLOBAL_VAR_H_
/****************************************************************************
* 文件名称 : Global_Var.h
* 文件功能 : 将项目中所有的共享变量集中声明在这里
* 注意事项 : 共享变量、全局类型将在这里定义
*****************************************************************************/
#include  "common.h"

/*
 * Include 用户自定义的头文件
 */
#include  "MK60_wdog.h"
#include  "MK60_gpio.h"     //IO口操作
#include  "MK60_uart.h"     //串口
#include  "MK60_SysTick.h"
#include  "MK60_lptmr.h"    //低功耗定时器(延时)
#include  "MK60_pit.h"      //PIT
#include  "MK60_FLASH.h"    //FLASH
#include  "MK60_FTM.h"
#include "MK60_adc.h"
#include "MK60_pit.h"
#include "MK60_uart.h"
#include  "VCAN_computer.h"     //多功能调试助手




/* !< 宏定义区 */

#define u8_Pro unsigned char
#define u16_Pro unsigned short
#define u32_Pro unsigned int
#define String_Pro unsigned char*
#define Float_Pro float
#define int8_Pro signed char
#define int32_Pro signed int
#define int16_Pro signed short


#define OLED_FUNCTION	1									/* !< OLED 文件函数屏蔽 1为正常模式 0为屏蔽模式	    */
#define CELL_FUNCTION	1									/* !< 电池检测测试函数标志 1为正常0为屏蔽 		    */
#define ENCORD_FUNCTION	1									/* !< 编码器函数屏蔽标志 1正常，0屏蔽 			  */
#define KEY_FUNCTION	1									/* !< 按键函数屏蔽标志 1为正常0为屏蔽 			  */
#define MOTOR_FUNCTION	1									/* !< 电机函数屏蔽标志 1为正常0为屏蔽 			  */
#define OV_FUNCTION		0									/* !< OV摄像头测试标志 1为正常 0为屏蔽 			   */
#define SERVO_FUNCTION	1									/* !< 舵机测试标志 1为正常 0为屏蔽 				    */
#define WIFI_FUNCTION	1 									/* !< WIFI测试标志 1为正常 0为屏蔽				  */
#define IMG_FUNCTION	0									/* !< 图像数据处理测试标志 1为正常 0为屏蔽		     */
#define PID_FUNCTION	0
#define MIDLINE_FUNCTION	0
#define Debug_OLED_Function				0
#define Debug_IO_Function				1
#define Debug_Interface_Function		0

#define Rx_BUF_MAX_SIZE 100		                            /*!< wifi 串口接收最大 长度                         */          							

#define RC_MODE 1                                           /*!< 遥控模式，0为关闭 1为开启 */

#if RC_MODE==1
#define GOHADE "g"
#define BACK "b"
#define RIGHT "r"
#define LEFT "l"
#endif

/* !< 全局变量声明区 */

extern u8_Pro OLED_GRAM[128][8];							/* !< OLED 屏幕显示 缓存							*/
extern u32_Pro Motor_FeedBack_SpeedTime;					/* !< 编码器获取的电机速度反馈				 	*/
extern Float_Pro Servo_FeedBack_DirDeviation;				/* !< 摄像头中 的 摄像头方向误差			 		*/
extern u16_Pro OLED_Refrech_FPS;

extern int16_Pro Servo_Default_Angle;							/* !< 舵机默认角度		 						*/
extern int16_Pro Servo_Angle_Now;								/* !< 舵机现有角度 								*/		
extern int16_Pro Servo_MaxTurn_Angle;							/* !< 舵机最大偏角 								*/

extern u16_Pro OV_Refresh_FPS;								/* !< 摄像头数据接收刷新数(每秒清 0)  			*/
extern u8_Pro OV_ImgOVER_Flag;								/* !< 图像接收完成标志							*/
// extern u8_Pro OV_Img[60][80];								/* !< 真数据存储数组		 						*/
// extern u8_Pro OV_7725Rx_Img[240][40];						/* !< 摄像头接收数据数组  						*/

extern u16_Pro Motor_Speed_Now;							/* !< 电机当前期望速度 0~最大计数值				*/

extern u8_Pro ROAD_IMG_FLAG;
extern u8_Pro MidLine_Array[60];
extern int16_Pro MDL_Slope;






extern Float_Pro MidLine_Dir_Near;							/* !< 最近地段的 中线 斜率						*/
extern Float_Pro MidLine_Dir_Mid;							/* !< 中部地段的 中线 斜率						*/
extern Float_Pro MidLine_Dir_Far;							/* !< 远处地段的 中线 斜率						*/

extern u8_Pro Debug_OLED_PageNum;							/* !< 确定当前显示的是第几页 						*/
extern u8_Pro Debug_OLED_PageCmd;							/* !< 确定当前是使用修改命令 还是选择命令 		*/

extern u8_Pro WIFI_Tx_BUF[4800];                            /* WiFi 串口 调试助手                          */
extern u8_Pro WIFI_Rx_BUF[Rx_BUF_MAX_SIZE];                  /*<! wifi 串口 接收 缓存 */
extern u8_Pro WIFI_RX_STA;		                                /*<! 要求 MAX SIZE 小于等于 63 */

extern struct PID_DIV Motor_div;
extern struct PID_DIV Servo_div;

extern int32_Pro Motor_Default_Setpoint;

#endif
