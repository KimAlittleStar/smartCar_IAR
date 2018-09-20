#ifndef _CELLDRIVE_H_
#define _CELLDRIVE_H_

#include "Global_Var.h"

/*------------------------宏定义  区----------------------*/
#define Cell_BEEP_ON {PTE24_OUT=1;}
#define Cell_BEEP_OFF {PTE24_OUT=0;}

/*------------------------函数声明区----------------------*/

void Cell_Init(void);

u16_Pro Cell_GetBatteryPer(void);

void Cell_Protection(void);

void Cell_Warning(String_Pro warning);

void Cell_PIT2_Protect_NVIC(void);


#endif


