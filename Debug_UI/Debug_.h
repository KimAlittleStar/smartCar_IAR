#ifndef _DEBUG__H_
#define _DEBUG__H_

#include "Global_Var.h"
/*------------------------�궨��  ��----------------------*/


/*------------------------����������----------------------*/

void Debug_OLED_ShowIcon(u8_Pro index_X,u8_Pro index_Y,u8_Pro* icon);
void senddata(u16_Pro count,u32_Pro* Value_Array);
void Debug_ModifyPar(u8_Pro DEBUG_set_Flag,u8_Pro Up_Down);


void Debug_SetParam(void);
void Debug_RcMode(void);
void Debug_ImgAcq(void);
void Debug_Set(void);


/* !< OLED ��Ļ ������				 */

//��һҳ����ʾ ���������
void Debug_OLED_FristPage(void);
void Debug_OLED_FristPage_Up_Press(void);
void Debug_OLED_FristPage_Down_Press(void);
void Debug_OLED_FristPage_Cancel_Press(void);
void Debug_OLED_FristPage_Confirm_Press(void);

//�ڶ�ҳ
void Debug_OLED_SecondPage(void);
void Debug_OLED_SecondPage_Up_Press(void);
void Debug_OLED_SecondPage_Down_Press(void);
void Debug_OLED_SecondPage_Cancel_Press(void);
void Debug_OLED_SecondPage_Confirm_Press(void);

//����ҳ
void Debug_OLED_ThirdPage(void);
void Debug_OLED_ThirdPage_Up_Press(void);
void Debug_OLED_ThirdPage_Down_Press(void);
void Debug_OLED_ThirdPage_Cancel_Press(void);
void Debug_OLED_ThirdPage_Confirm_Press(void);

//����ҳ
void Debug_OLED_FourthPage(void);
void Debug_OLED_FourthPage_Up_Press(void);
void Debug_OLED_FourthPage_Down_Press(void);
void Debug_OLED_FourthPage_Cancel_Press(void);
void Debug_OLED_FourthPage_Confirm_Press(void);

void Debug_OLED_Main(void);



/* !< IO ���� ������					 */

void Debug_IOTest_Init(void);
void Debug_IOTest_Star(void);
void Debug_IOTest_Over(void);

u32_Pro Debug_getNumFormStr(const String_Pro str,const String_Pro key,u32_Pro oldValue );



/* !< �ӿ� ��������					 */

void Debug_Interface(void);

#endif

