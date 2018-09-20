#ifndef _OLEDDRIVE_H_
#define _OLEDDRIVE_H_


#include "Global_Var.h"
/*------------------------�궨��  ��----------------------*/

#define OLED_SPI_MODE 	1 									/* !< OLED ��Ļ����ģʽ  1Ϊ���ģ�� 0 ΪӲ��Ӧ�� */
#define OLED_DC_CMD 	0									/* !< OLED ��Ļд�Ĵ�������					    */
#define OLED_DC_DATA 	1									/* !< OLED ��Ļд��������						*/

#define POINT_STATU_SET 	1								/* !< OLED ��Ļ���״̬��־�� ���				*/
#define POINT_STATU_RESET 	0								/* !< OLED ��Ļ���״̬��־�� ȥ��				*/

#define charSize12	12										/* !< �ַ��Ĵ�С���� OLED_FONT.h  ���� 			*/
#define charSize16	16										/* !< �ַ��Ĵ�С���� OLED_FONT.h  ���� 			*/
#define charSize24	24										/* !< �ַ��Ĵ�С���� OLED_FONT.h  ���� 			*/

#define OLED_Port_SPI_RST PTA14
#define  OLED_PIN_SPI_RST PTA14_OUT

#define OLED_Port_SPI_DC PTA13
#define  OLED_PIN_SPI_DC PTA13_OUT

//ʱ��
#define OLED_Port_SPI_SCLK PTA15
#define  OLED_PIN_SPI_SCLK PTA15_OUT

//����
#define OLED_Port_SPI_SDIN PTA16
#define  OLED_PIN_SPI_SDIN PTA16_OUT

//Ƭѡ
#define OLED_Port_SPI_CS PTA17
#define  OLED_PIN_SPI_CS PTA17_OUT




//�궨�����ùܽ�
#define OLED_SPI_SETRST(bit) (OLED_PIN_SPI_RST = bit)

#define OLED_SPI_SETDC(bit) (OLED_PIN_SPI_DC = bit)
#define OLED_SPI_SETCS_High (OLED_PIN_SPI_CS = 1)
#define OLED_SPI_SETCS_Low (OLED_PIN_SPI_CS = 0)


#if OLED_SPI_MODE == 1

//ʹ�����SPI
//#define OLED_SPI_SETSCLK(bit) (bit == 0 ? (Port_SPI_SCLK->BSRRH=PIN_SPI_SCLK):(Port_SPI_SCLK->BSRRL=PIN_SPI_SCLK))
#define OLED_SPI_SETSCLK_High (OLED_PIN_SPI_SCLK = 1)
#define OLED_SPI_SETSCLK_Low (OLED_PIN_SPI_SCLK = 0)

//#define OLED_SPI_SETSDIN(bit) (bit == 0 ? (Port_SPI_SDIN->BSRRH=PIN_SPI_SDIN):(Port_SPI_SDIN->BSRRL=PIN_SPI_SDIN))
#define OLED_SPI_SETSDIN_High (OLED_PIN_SPI_SDIN = 1)
#define OLED_SPI_SETSDIN_Low (OLED_PIN_SPI_SDIN = 0)

#endif

extern u8_Pro* BBmp;

/*------------------------����������----------------------*/

/* !< Oled ��Ļ ���ܺ��� */

void OLED_Init(void);

void OLED_ResetGRAM(void);

void OLED_Clear(void);

void OLED_Show_Point(u8_Pro Index_X,u8_Pro Index_Y,u8_Pro POINT_STATU);

void OLED_Refrech_Gram(void);

void OLED_Show_Char(u8_Pro Index_X,u8_Pro Index_Y,u8_Pro chr,u8_Pro charSize,u8_Pro POINT_STATU);

void OLED_Show_Num(u8_Pro Index_X,u8_Pro Index_Y,u32_Pro NUMBER,u8_Pro charSize);

void OLED_Show_String(u8_Pro Index_X,u8_Pro Index_Y,const String_Pro string,u8_Pro charSize);
void OLED_Set_Byte(u8_Pro Index_X,u8_Pro Index_Y,u8_Pro dat,u8_Pro POINT_STATU);
void OLED_Inverting_Rectangle(u8_Pro Index_X,u8_Pro Index_Line,u8_Pro longer);


/* !< Oled ��Ļ�������� */

void OLED_WR_Byte(u8_Pro dat,u8_Pro OLED_DC);




#endif
