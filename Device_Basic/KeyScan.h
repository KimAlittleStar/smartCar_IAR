#ifndef _KEYSCAN_H_
#define _KEYSCAN_H_

#include "Global_Var.h"
/*------------------------宏定义  区----------------------*/


#define KEY_MODE_Once			0
#define KEY_MODE_Continuous		1

#define Key_Cancel  PTB3
#define Key_Cancel_IN  PTB3_IN

#define Key_Confirm PTB1
#define Key_Confirm_IN PTB1_IN

#define Key_Down PTE26
#define Key_Down_IN PTE26_IN

#define Key_Up PTE25
#define Key_Up_IN PTE25_IN


/*------------------------函数声明区----------------------*/

void KeyScan_Init(void);

u8_Pro Key_Of_Confirm(u8_Pro KEY_MODE);

u8_Pro Key_Of_Cancel(u8_Pro KEY_MODE);

u8_Pro Key_Of_Up(u8_Pro KEY_MODE);

u8_Pro Key_Of_Down(u8_Pro KEY_MODE);

#endif

