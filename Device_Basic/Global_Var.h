#ifndef _GLOBAL_VAR_H_
#define _GLOBAL_VAR_H_
/****************************************************************************
* �ļ����� : Global_Var.h
* �ļ����� : ����Ŀ�����еĹ��������������������
* ע������ : ���������ȫ�����ͽ������ﶨ��
*****************************************************************************/
#include  "common.h"

/*
 * Include �û��Զ����ͷ�ļ�
 */
#include  "MK60_wdog.h"
#include  "MK60_gpio.h"     //IO�ڲ���
#include  "MK60_uart.h"     //����
#include  "MK60_SysTick.h"
#include  "MK60_lptmr.h"    //�͹��Ķ�ʱ��(��ʱ)
#include  "MK60_pit.h"      //PIT
#include  "MK60_FLASH.h"    //FLASH
#include  "MK60_FTM.h"
#include "MK60_adc.h"
#include "MK60_pit.h"
#include "MK60_uart.h"
#include  "VCAN_computer.h"     //�๦�ܵ�������




/* !< �궨���� */

#define u8_Pro unsigned char
#define u16_Pro unsigned short
#define u32_Pro unsigned int
#define String_Pro unsigned char*
#define Float_Pro float
#define int8_Pro signed char
#define int32_Pro signed int
#define int16_Pro signed short


#define OLED_FUNCTION	1									/* !< OLED �ļ��������� 1Ϊ����ģʽ 0Ϊ����ģʽ	    */
#define CELL_FUNCTION	1									/* !< ��ؼ����Ժ�����־ 1Ϊ����0Ϊ���� 		    */
#define ENCORD_FUNCTION	1									/* !< �������������α�־ 1������0���� 			  */
#define KEY_FUNCTION	1									/* !< �����������α�־ 1Ϊ����0Ϊ���� 			  */
#define MOTOR_FUNCTION	1									/* !< ����������α�־ 1Ϊ����0Ϊ���� 			  */
#define OV_FUNCTION		0									/* !< OV����ͷ���Ա�־ 1Ϊ���� 0Ϊ���� 			   */
#define SERVO_FUNCTION	1									/* !< ������Ա�־ 1Ϊ���� 0Ϊ���� 				    */
#define WIFI_FUNCTION	1 									/* !< WIFI���Ա�־ 1Ϊ���� 0Ϊ����				  */
#define IMG_FUNCTION	0									/* !< ͼ�����ݴ�����Ա�־ 1Ϊ���� 0Ϊ����		     */
#define PID_FUNCTION	0
#define MIDLINE_FUNCTION	0
#define Debug_OLED_Function				0
#define Debug_IO_Function				1
#define Debug_Interface_Function		0

#define Rx_BUF_MAX_SIZE 100		                            /*!< wifi ���ڽ������ ����                         */          							

#define RC_MODE 1                                           /*!< ң��ģʽ��0Ϊ�ر� 1Ϊ���� */

#if RC_MODE==1
#define GOHADE "g"
#define BACK "b"
#define RIGHT "r"
#define LEFT "l"
#endif

/* !< ȫ�ֱ��������� */

extern u8_Pro OLED_GRAM[128][8];							/* !< OLED ��Ļ��ʾ ����							*/
extern u32_Pro Motor_FeedBack_SpeedTime;					/* !< ��������ȡ�ĵ���ٶȷ���				 	*/
extern Float_Pro Servo_FeedBack_DirDeviation;				/* !< ����ͷ�� �� ����ͷ�������			 		*/
extern u16_Pro OLED_Refrech_FPS;

extern int16_Pro Servo_Default_Angle;							/* !< ���Ĭ�ϽǶ�		 						*/
extern int16_Pro Servo_Angle_Now;								/* !< ������нǶ� 								*/		
extern int16_Pro Servo_MaxTurn_Angle;							/* !< ������ƫ�� 								*/

extern u16_Pro OV_Refresh_FPS;								/* !< ����ͷ���ݽ���ˢ����(ÿ���� 0)  			*/
extern u8_Pro OV_ImgOVER_Flag;								/* !< ͼ�������ɱ�־							*/
// extern u8_Pro OV_Img[60][80];								/* !< �����ݴ洢����		 						*/
// extern u8_Pro OV_7725Rx_Img[240][40];						/* !< ����ͷ������������  						*/

extern u16_Pro Motor_Speed_Now;							/* !< �����ǰ�����ٶ� 0~������ֵ				*/

extern u8_Pro ROAD_IMG_FLAG;
extern u8_Pro MidLine_Array[60];
extern int16_Pro MDL_Slope;






extern Float_Pro MidLine_Dir_Near;							/* !< ����ضε� ���� б��						*/
extern Float_Pro MidLine_Dir_Mid;							/* !< �в��ضε� ���� б��						*/
extern Float_Pro MidLine_Dir_Far;							/* !< Զ���ضε� ���� б��						*/

extern u8_Pro Debug_OLED_PageNum;							/* !< ȷ����ǰ��ʾ���ǵڼ�ҳ 						*/
extern u8_Pro Debug_OLED_PageCmd;							/* !< ȷ����ǰ��ʹ���޸����� ����ѡ������ 		*/

extern u8_Pro WIFI_Tx_BUF[4800];                            /* WiFi ���� ��������                          */
extern u8_Pro WIFI_Rx_BUF[Rx_BUF_MAX_SIZE];                  /*<! wifi ���� ���� ���� */
extern u8_Pro WIFI_RX_STA;		                                /*<! Ҫ�� MAX SIZE С�ڵ��� 63 */

extern struct PID_DIV Motor_div;
extern struct PID_DIV Servo_div;

extern int32_Pro Motor_Default_Setpoint;

#endif
