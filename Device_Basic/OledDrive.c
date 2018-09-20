/****************************************************************************
* �ļ����� : OledDrive.c
* �ļ����� : ��ʼ�� OLED��Ļ ʵ�ֻ����Ļ�����ʾ����
* ע������ :
*****************************************************************************/

#include "OledDrive.h"
#include "OledFont.h"

u8_Pro OLED_GRAM[128][8] = {0};
 u8_Pro *BBmp = (u8_Pro*)bmp;
u16_Pro OLED_Refrech_FPS = 0;


#if OLED_SPI_MODE == 1		/* !< ##################���ģ��ģʽ�µĺ���################## */



/****************************************************************************
* ��������: OLED ��ʼ������(���ģ��ģʽ)
* �������� : ������ OLED��Ļ��ʼ��
* ���� :
* ��� :
* ���� :
*****************************************************************************/
void OLED_Init(void)
{
	gpio_init(OLED_Port_SPI_CS,GPO,0);
	gpio_init(OLED_Port_SPI_DC,GPO,0);
	gpio_init(OLED_Port_SPI_SDIN,GPO,0);
	gpio_init(OLED_Port_SPI_SCLK,GPO,0);
	gpio_init(OLED_Port_SPI_RST,GPO,0);
	
	OLED_SPI_SETCS_Low;
	OLED_SPI_SETDC(1);
	DELAY_MS(1);
	OLED_SPI_SETRST(0);
	DELAY_MS(1);
	OLED_SPI_SETRST(1);
	
	OLED_WR_Byte(0xAE,OLED_DC_CMD);				/*!< �ر���ʾ													 			*/ 
	OLED_WR_Byte(0xD5,OLED_DC_CMD);				/*!< ����ʱ�ӷ�Ƶ����,��Ƶ�� 												*/ 
	OLED_WR_Byte(80,OLED_DC_CMD); 					/*!< [3:0],��Ƶ����;[7:4],��Ƶ�� 											*/   
	OLED_WR_Byte(0xA8,OLED_DC_CMD);				/*!< ��������·�� 															*/  
	OLED_WR_Byte(0X3F,OLED_DC_CMD);				/*!< Ĭ��0X3F(1/64)  														*/  
	OLED_WR_Byte(0xD3,OLED_DC_CMD);				/*!< ������ʾƫ�� 															*/  
	OLED_WR_Byte(0X00,OLED_DC_CMD);				/*!< Ĭ��Ϊ0 																*/  

	OLED_WR_Byte(0x40,OLED_DC_CMD);				/*!< ������ʾ��ʼ�� [5:0],����. 											*/  
													    
	OLED_WR_Byte(0x8D,OLED_DC_CMD);				/*!< ��ɱ����� 															*/  
	OLED_WR_Byte(0x14,OLED_DC_CMD);				/*!< bit2������/�ر� 														*/  
	OLED_WR_Byte(0x20,OLED_DC_CMD);				/*!< �����ڴ��ַģʽ 														*/  
	OLED_WR_Byte(0x02,OLED_DC_CMD);				/*!< [1:0],00���е�ַģʽ;01���е�ַģʽ;10,ҳ��ַģʽ;Ĭ��10;				*/  
	OLED_WR_Byte(0xA1,OLED_DC_CMD);				/*!< ���ض�������,bit0:0,0->0;1,0->127;										*/  
	OLED_WR_Byte(0xC0,OLED_DC_CMD);				/*!< ����COMɨ�跽��;bit3:0,��ͨģʽ;1,�ض���ģʽ COM[N-1]->COM0;N:����·�� */  
	OLED_WR_Byte(0xDA,OLED_DC_CMD);				/*!< ����COMӲ���������� 													*/  
	OLED_WR_Byte(0x12,OLED_DC_CMD);				/*!< [5:4]���� 																*/  
		 
	OLED_WR_Byte(0x81,OLED_DC_CMD); 				/*!< �Աȶ�����	 															*/	
	OLED_WR_Byte(0xEF,OLED_DC_CMD); 				/*!< 1~255;Ĭ��0X7F (��������,Խ��Խ��) 									*/ 
	OLED_WR_Byte(0xD9,OLED_DC_CMD); 				/*!< ����Ԥ������� 														*/
	OLED_WR_Byte(0xf1,OLED_DC_CMD); 				/*!< [3:0],PHASE 1;[7:4],PHASE 2; 											*/ 
	OLED_WR_Byte(0xDB,OLED_DC_CMD); 				/*!< ����VCOMH ��ѹ���� 													*/ 
	OLED_WR_Byte(0x30,OLED_DC_CMD); 				/*!< [6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc; 							*/ 

	OLED_WR_Byte(0xA4,OLED_DC_CMD); 				/*!< ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����)  							*/ 
	OLED_WR_Byte(0xA6,OLED_DC_CMD); 				/*!< ������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ	 							*/  						   
	OLED_WR_Byte(0xAF,OLED_DC_CMD); 				/*!< ������ʾ 																*/ 
	OLED_Clear(); 								/*!< ���� 																	*/
	
		
// #if OLED_FUNCTION == 1
// 	#warning "����δ�ر༭"			/* !< �༭����ɾ������ */
// #endif	
}


/****************************************************************************
* ��������: OLED д���ݺ���(���ģ��ģʽ)
* �������� : ��ĳ��ֵ ������д�� OLED��Ļ ��
* ���� :dat ֵ 0x00-0xFF ��DC д������ or ����:	OLED_DC_CMD or OLED_DC_DATA
* ��� :void
* ���� :
*****************************************************************************/
void OLED_WR_Byte(u8_Pro dat,u8_Pro OLED_DC)
{
u8_Pro i;
	
	OLED_SPI_SETDC(OLED_DC);
//	OLED_SPI_SETCS_Low;
	for(i = 0;i < 8;i++)
	{
		OLED_SPI_SETSCLK_Low;
		if(dat & 0x80)
		{
			OLED_SPI_SETSDIN_High;
		}else
		{
			OLED_SPI_SETSDIN_Low;
		}
		dat <<= 1;
		OLED_SPI_SETSCLK_High;	
	}
//	OLED_SPI_SETCS_High;
	OLED_SPI_SETDC(1);
	
	
//#if OLED_FUNCTION == 1
//	#warning "����δ�ر༭"			/* !< �༭����ɾ������ */
//#endif		
}


#else	/* !< ##################Ӳ��֧��ģʽ�µĺ���################## */


/****************************************************************************
* ��������: OLED ��ʼ������(���ģ��ģʽ)
* �������� : ������ OLED��Ļ��ʼ��
* ���� :
* ��� :
* ���� :
*****************************************************************************/
void OLED_Init(void)
{
#if OLED_FUNCTION == 1
	#warning "����δ�ر༭"			/* !< �༭����ɾ������ */
#endif		
}


/****************************************************************************
* ��������: OLED д���ݺ���(���ģ��ģʽ)
* �������� : ��ĳ��ֵ ������д�� OLED��Ļ ��
* ���� :dat ֵ 0x00-0xFF ��DC д������ or ����:	OLED_DC_CMD or OLED_DC_DATA
* ��� :void
* ���� :
*****************************************************************************/
void OLED_WR_Byte(u8_Pro dat,u8_Pro OLED_DC)
{
#if OLED_FUNCTION == 1
	#warning "����δ�ر༭"			/* !< �༭����ɾ������ */
#endif		
}

#endif


/****************************************************************************
* ��������: OLED ˢ����Ļ���� ����������ֻд ���ݸı䣬���е���ʾ���ڴ˺����б�ˢ��
* �������� : ȫˢ��һ�� OLED��Ļ
* ���� :void
* ��� :void
* ���� :
*****************************************************************************/
void OLED_Refrech_Gram(void)
{
	
	u16_Pro i,j;
	for(i = 0;i < 8 ; i++)
	{
		OLED_WR_Byte (0xb0+i,OLED_DC_CMD);   			/*!< ����ҳ��ַ��0~7�� 						*/ 
		OLED_WR_Byte (0x00,OLED_DC_CMD);    			/*!< ������ʾλ�á��е͵�ַ 					*/  
		OLED_WR_Byte (0x10,OLED_DC_CMD);    			/*!< ������ʾλ�á��иߵ�ַ 					*/   
		for(j = 0; j<128 ;j++)
		OLED_WR_Byte(OLED_GRAM[j][i],OLED_DC_DATA);
	}
//#if OLED_FUNCTION == 1
//	#warning "����δ�ر༭"			/* !< �༭����ɾ������ */
//#endif		
}


/****************************************************************************
* ��������: OLED��Ļ����
* �������� : �� OLED ��Ļ����
* ���� :void
* ��� :void
* ���� :
*****************************************************************************/
void OLED_Clear(void)
{
	u16_Pro i, j;
//	OLED_ResetGRAM();
	for(i = 0;i < 8;i++)
	{
		OLED_WR_Byte (0xb0+i,OLED_DC_CMD);   			/*!< ����ҳ��ַ��0~7�� 						*/ 
		OLED_WR_Byte (0x00,OLED_DC_CMD);    			/*!< ������ʾλ�á��е͵�ַ 					*/  
		OLED_WR_Byte (0x10,OLED_DC_CMD);    			/*!< ������ʾλ�á��иߵ�ַ 					*/   
		for(j = 0;j<128 ;j++)
		{
			OLED_GRAM[j][i] = 0x00;
			OLED_WR_Byte(0,OLED_DC_DATA);
		}
		
	}
//#if OLED_FUNCTION == 1
//	#warning "����δ�ر༭"			/* !< �༭����ɾ������ */
//#endif		
}


/****************************************************************************
* ��������: OLED ���㺯��
* �������� : OLED�������� �����к�����ʾ�������������
* ���� :������� X(0-128) Y(0-63) ���״̬:POINT_STATU_RESET or POINT_STATU_SET
*		POINT_STATU_RESET: ��������� (ʵ���к����õ������Բ�����???)
*		POINT_STATU_SET:   ���������
* ��� :void
* ���� :
*****************************************************************************/
void OLED_Show_Point(u8_Pro Index_X,u8_Pro Index_Y,u8_Pro POINT_STATU)
{

	if(Index_X < 128 && Index_Y <64)
	{
		OLED_GRAM[Index_X][7-(Index_Y>>3)] |= POINT_STATU << (7-(Index_Y & 0x07));
	}

//	#if OLED_FUNCTION == 1
//	#warning "����δ�ر༭"			/* !< �༭����ɾ������ */
//	#endif		
	
}


/****************************************************************************
* ��������: OLED ��ʾ�ַ�����(��Ҫ�õ�OLED_FONT.h�ļ�)
* �������� : ��һ���ַ���ʾ��OLED��Ļ��
* ���� :���� X(0~(127-charSize/2)) Y(0~(63-charSize))
*		�ַ� ���õ�Ӣ���ַ������š�����
*		�ַ���С charSize ��OLED_FONT.h �ļ�����
*		���״̬
* ��� :void
* ���� :
*****************************************************************************/
void OLED_Show_Char(u8_Pro Index_X,u8_Pro Index_Y,u8_Pro chr,u8_Pro charSize,u8_Pro POINT_STATU)
{
// #if OLED_FUNCTION == 1
// 	#warning "����δ�ر༭"			/* !< �༭����ɾ������ */
// #endif		
	u8_Pro temp,t;
	u8_Pro chline = charSize/8 + ((charSize%8) ? 1:0);
	u8_Pro csize=chline * (charSize / 2);									/*!< �õ�����һ���ַ���Ӧ������ռ���ֽ��� 		*/	
	chr=chr-' ';														/*!< �õ�ƫ�ƺ��ֵ									*/			 
    for(t=0;t<csize;t++)
	{   
		if(charSize==12)temp=asc2_1206[chr][t]; 						/*!< ����1206����									*/	
		else if(charSize==16)temp=asc2_1608[chr][t]; 					/*!< ����1608����									*/	
		else if(charSize==24)temp=asc2_2412[chr][t]; 					/*!< ����2412���� 									*/	
		else return;													/*!< û�е��ֿ� 									*/	
		
		OLED_Set_Byte(Index_X+(t/chline),Index_Y+((t % chline)<<3),temp,POINT_STATU);
																		/*!< ������� �ַ��� n �� 8Ϊ�ֽڴ洢			    */
																		/*!< ��ĸΪ size * (size/2)/8 ���ֽ�			    */
																		/*!< ���з�ʽΪ �����ں�						    */
																		/*!< ���� 8 �ı����� ������Ҫ��һ �磺12 Ϊ 2��6��  */
																		/*!< x���� ÿִ�� ���� �� ��һ					    */
																		/*!< y ���� �� ÿ�еĵ�һ������ ���ر䶯		    */
    } 
	

}


/****************************************************************************
* ��������: OLED ��ʾ���ֺ���
* �������� : ��ʾ NUMBER ��ֵ����Ļ�� ������Ҫ��ʾ����������������ʾ Ϊ������ʾ '0'
* ���� :���� X(0~(127-charSize/2)) Y(0~(63-charSize))
*		Ҫ��ʵ����ֵ NUMBER(0~4294967295)
*		�ַ���С charSize ��OLED_FONT.h �ļ�����
* ��� :void
* ���� :
*****************************************************************************/
void OLED_Show_Num(u8_Pro Index_X,u8_Pro Index_Y,u32_Pro NUMBER,u8_Pro charSize)
{
//#if OLED_FUNCTION == 1
//	#warning "����δ�ر༭"			/* !< �༭����ɾ������ */
//#endif		
	u8_Pro numLen = 0;
	u32_Pro temp = 1;
	u8_Pro wei = 0;
	if(NUMBER)
	{
			while(NUMBER/temp != 0)
		{
			temp *= 10; 
			numLen++;		
		}
		temp /= 10;
		for(wei = 0;wei < numLen;wei++)
		{
			//ΪʲôcharSize Ҫ���Զ��� ��Ϊ��ĸ ���� �ֱ��� 12*12  ��ʵ ����Ϊ 6*12 ������ͬ��
			OLED_Show_Char(Index_X + (charSize/2) * wei ,Index_Y,(NUMBER/temp)%10+'0',charSize,POINT_STATU_SET);
			temp /= 10;
		}

	}else
	{
		OLED_Show_Char(Index_X,Index_Y,'0',charSize,POINT_STATU_SET);
	}

	}

/****************************************************************************
* ��������: OLED ��ʾ �ַ�������
* �������� : �� string �е��ַ�����ʾ����Ļ��
*			һ����ʾ���������У�һҳ��ʾ������ҳ
* ���� :���� X(0~(127-charSize/2)) Y(0~(63-charSize))
*		�ַ��� string ��const ������
*		�ַ���С charSize ��OLED_FONT.h �ļ�����
* ��� :void
* ���� :
*****************************************************************************/
void OLED_Show_String(u8_Pro Index_X,u8_Pro Index_Y,const String_Pro string,u8_Pro charSize)
{
	
//#if OLED_FUNCTION == 1
//	#warning "����δ�ر༭"			/* !< �༭����ɾ������ */
//#endif		
	while((*string) <= '~' && (*string) >= ' ')
	{
		if(Index_X > (128-(charSize / 2)))
		{
			Index_X=0;Index_Y += charSize;
		}
        if(Index_Y > (64 - charSize))
		{ 
			Index_Y = Index_X = 0;
			OLED_Clear();
		}
		
        OLED_Show_Char(Index_X,Index_Y,*string,charSize,POINT_STATU_SET);	 
        
			/*!< ΪʲôcharSize Ҫ���Զ��� ��Ϊ��ĸ ���� �ֱ��� 12*12  ��ʵ ����Ϊ 6*12 ������ͬ�� */	
		Index_X += charSize/2;
        
		string++;
	}

	
}
/**************************************************************************
* �������ܣ��� �Դ������� һ���ֽڵ�����
*	���ļ��� ����Ҫ�Ļ��� ���ݺ����������� �Դ� ��д��һb ����
* ���룺�ֽ�λ�� XY ���� ������ֵ �����״̬
* �����void
* ����:@Kim_alittle_star
***************************************************************************/
void OLED_Set_Byte(u8_Pro Index_X,u8_Pro Index_Y,u8_Pro dat,u8_Pro POINT_STATU)
{
	u8_Pro line = (63-Index_Y) >> 3;				/*!< ȷ�����ڵڼ���							 */
	u8_Pro move = (Index_Y & 0x07);				/*!< ȷ���ǵ���ĳ�еĵڼ���λ��				 */
	if(POINT_STATU == POINT_STATU_SET)
		;
	else if(POINT_STATU == POINT_STATU_RESET)
		dat = ~dat;
	if(Index_X<128 && Index_Y<64)
	{
		if(move == 0)
		{
			OLED_GRAM[Index_X][line] |= dat;
		}else
		{
			OLED_GRAM[Index_X][line] |= (dat>>move);
			if(line != 0)
				OLED_GRAM[Index_X][line-1] |= dat << (8-move);
		}
	}
}


/**************************************************************************
* �������ܣ���Ļ���ຯ��
*	���ƶ��� ��ָ���п�ʼ ָ������ �ڽ��䷴��
* ���룺��ʼ���� & ĳһ�� & ���� 
* �����void
***************************************************************************/
void OLED_Inverting_Rectangle(u8_Pro Index_X,u8_Pro Index_Line,u8_Pro longer)
{
	//�������� ��ʹ�øú���ʹ�� �÷������������
	u8_Pro i;
	OLED_WR_Byte (0xb0+7-Index_Line,OLED_DC_CMD);   			/*!< ����ҳ��ַ��0~7�� 											*/ 
	OLED_WR_Byte (0x00 | (Index_X & 0x0f),OLED_DC_CMD);    	/*!< ������ʾλ�á��е͵�ַ 										*/ 
	OLED_WR_Byte (0x10 | (Index_X >> 4),OLED_DC_CMD);    		/*!< ������ʾλ�á��иߵ�ַ 										*/
	for(i = 0;i < longer ;i++ )
	{
		OLED_GRAM[Index_X+i][7-Index_Line] = ~OLED_GRAM[Index_X+i][7-Index_Line];
		OLED_WR_Byte(OLED_GRAM[Index_X+i][7-Index_Line],OLED_DC_DATA);
	}
	OLED_Refrech_Gram();
}

void OLED_ResetGRAM(void)
{
	u8_Pro* p = OLED_GRAM[0];
	u16_Pro sum = 8*128;
	for(;sum>0;sum--)
	{
		*p = 0x00;
		p++;
	}
}

