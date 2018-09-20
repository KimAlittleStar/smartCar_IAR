#ifndef __INCLUDE_H__
#define __INCLUDE_H__

/* !< include �? */
#include "Global_Var.h"
#include "OledDrive.h"			/* !< OLED 屏幕驱动	 */
#include "CellDrive.h"			/* !< 电池电压驱动	 */	
#include "EncordDrive.h"		/* !< 编码器驱�?		 */
#include "KeyScan.h"			/* !< �?盘扫描驱�?	 */
#include "MotorDrive.h"			/* !< 直流电机驱动	 */
#include "OV7725Drive.h"		/* !< 摄像�?7725驱动	 */
#include "ServoDrive.h"			/* !< 舵机驱动		 */
#include "WIFIDrive.h"			/* !< WiFi 驱动		 */
#include "Debug_.h"
#include "IMG_Process.h"
#include "MidLine_Process.h"
#include "PID_Process.h"


/* !< 函数声明�? */

void Project_Init(void);

void Project_StarUp(void);

#if RC_MODE==1

void Project_RC(void);
void RC_Motor_go(void);

#elif RC_MODE==0
void senddata(u16_Pro count,u32_Pro* Value_Array);

#endif


#endif  //__INCLUDE_H__
