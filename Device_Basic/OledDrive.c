/****************************************************************************
* 文件名称 : OledDrive.c
* 文件功能 : 初始化 OLED屏幕 实现基本的画点显示功能
* 注意事项 :
*****************************************************************************/

#include "OledDrive.h"
#include "OledFont.h"

u8_Pro OLED_GRAM[128][8] = {0};
 u8_Pro *BBmp = (u8_Pro*)bmp;
u16_Pro OLED_Refrech_FPS = 0;


#if OLED_SPI_MODE == 1		/* !< ##################软件模拟模式下的函数################## */



/****************************************************************************
* 函数名称: OLED 初始化函数(软件模拟模式)
* 函数功能 : 函数将 OLED屏幕初始化
* 输入 :
* 输出 :
* 作者 :
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
	
	OLED_WR_Byte(0xAE,OLED_DC_CMD);				/*!< 关闭显示													 			*/ 
	OLED_WR_Byte(0xD5,OLED_DC_CMD);				/*!< 设置时钟分频因子,震荡频率 												*/ 
	OLED_WR_Byte(80,OLED_DC_CMD); 					/*!< [3:0],分频因子;[7:4],震荡频率 											*/   
	OLED_WR_Byte(0xA8,OLED_DC_CMD);				/*!< 设置驱动路数 															*/  
	OLED_WR_Byte(0X3F,OLED_DC_CMD);				/*!< 默认0X3F(1/64)  														*/  
	OLED_WR_Byte(0xD3,OLED_DC_CMD);				/*!< 设置显示偏移 															*/  
	OLED_WR_Byte(0X00,OLED_DC_CMD);				/*!< 默认为0 																*/  

	OLED_WR_Byte(0x40,OLED_DC_CMD);				/*!< 设置显示开始行 [5:0],行数. 											*/  
													    
	OLED_WR_Byte(0x8D,OLED_DC_CMD);				/*!< 电荷泵设置 															*/  
	OLED_WR_Byte(0x14,OLED_DC_CMD);				/*!< bit2，开启/关闭 														*/  
	OLED_WR_Byte(0x20,OLED_DC_CMD);				/*!< 设置内存地址模式 														*/  
	OLED_WR_Byte(0x02,OLED_DC_CMD);				/*!< [1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;				*/  
	OLED_WR_Byte(0xA1,OLED_DC_CMD);				/*!< 段重定义设置,bit0:0,0->0;1,0->127;										*/  
	OLED_WR_Byte(0xC0,OLED_DC_CMD);				/*!< 设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数 */  
	OLED_WR_Byte(0xDA,OLED_DC_CMD);				/*!< 设置COM硬件引脚配置 													*/  
	OLED_WR_Byte(0x12,OLED_DC_CMD);				/*!< [5:4]配置 																*/  
		 
	OLED_WR_Byte(0x81,OLED_DC_CMD); 				/*!< 对比度设置	 															*/	
	OLED_WR_Byte(0xEF,OLED_DC_CMD); 				/*!< 1~255;默认0X7F (亮度设置,越大越亮) 									*/ 
	OLED_WR_Byte(0xD9,OLED_DC_CMD); 				/*!< 设置预充电周期 														*/
	OLED_WR_Byte(0xf1,OLED_DC_CMD); 				/*!< [3:0],PHASE 1;[7:4],PHASE 2; 											*/ 
	OLED_WR_Byte(0xDB,OLED_DC_CMD); 				/*!< 设置VCOMH 电压倍率 													*/ 
	OLED_WR_Byte(0x30,OLED_DC_CMD); 				/*!< [6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc; 							*/ 

	OLED_WR_Byte(0xA4,OLED_DC_CMD); 				/*!< 全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)  							*/ 
	OLED_WR_Byte(0xA6,OLED_DC_CMD); 				/*!< 设置显示方式;bit0:1,反相显示;0,正常显示	 							*/  						   
	OLED_WR_Byte(0xAF,OLED_DC_CMD); 				/*!< 开启显示 																*/ 
	OLED_Clear(); 								/*!< 清屏 																	*/
	
		
// #if OLED_FUNCTION == 1
// 	#warning "函数未重编辑"			/* !< 编辑后请删除此行 */
// #endif	
}


/****************************************************************************
* 函数名称: OLED 写数据函数(软件模拟模式)
* 函数功能 : 将某个值 或命令写入 OLED屏幕 中
* 输入 :dat 值 0x00-0xFF ，DC 写入命令 or 数据:	OLED_DC_CMD or OLED_DC_DATA
* 输出 :void
* 作者 :
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
//	#warning "函数未重编辑"			/* !< 编辑后请删除此行 */
//#endif		
}


#else	/* !< ##################硬件支持模式下的函数################## */


/****************************************************************************
* 函数名称: OLED 初始化函数(软件模拟模式)
* 函数功能 : 函数将 OLED屏幕初始化
* 输入 :
* 输出 :
* 作者 :
*****************************************************************************/
void OLED_Init(void)
{
#if OLED_FUNCTION == 1
	#warning "函数未重编辑"			/* !< 编辑后请删除此行 */
#endif		
}


/****************************************************************************
* 函数名称: OLED 写数据函数(软件模拟模式)
* 函数功能 : 将某个值 或命令写入 OLED屏幕 中
* 输入 :dat 值 0x00-0xFF ，DC 写入命令 or 数据:	OLED_DC_CMD or OLED_DC_DATA
* 输出 :void
* 作者 :
*****************************************************************************/
void OLED_WR_Byte(u8_Pro dat,u8_Pro OLED_DC)
{
#if OLED_FUNCTION == 1
	#warning "函数未重编辑"			/* !< 编辑后请删除此行 */
#endif		
}

#endif


/****************************************************************************
* 函数名称: OLED 刷新屏幕函数 其他函数将只写 数据改变，所有的显示将在此函数中被刷新
* 函数功能 : 全刷新一次 OLED屏幕
* 输入 :void
* 输出 :void
* 作者 :
*****************************************************************************/
void OLED_Refrech_Gram(void)
{
	
	u16_Pro i,j;
	for(i = 0;i < 8 ; i++)
	{
		OLED_WR_Byte (0xb0+i,OLED_DC_CMD);   			/*!< 设置页地址（0~7） 						*/ 
		OLED_WR_Byte (0x00,OLED_DC_CMD);    			/*!< 设置显示位置—列低地址 					*/  
		OLED_WR_Byte (0x10,OLED_DC_CMD);    			/*!< 设置显示位置—列高地址 					*/   
		for(j = 0; j<128 ;j++)
		OLED_WR_Byte(OLED_GRAM[j][i],OLED_DC_DATA);
	}
//#if OLED_FUNCTION == 1
//	#warning "函数未重编辑"			/* !< 编辑后请删除此行 */
//#endif		
}


/****************************************************************************
* 函数名称: OLED屏幕清屏
* 函数功能 : 将 OLED 屏幕清屏
* 输入 :void
* 输出 :void
* 作者 :
*****************************************************************************/
void OLED_Clear(void)
{
	u16_Pro i, j;
//	OLED_ResetGRAM();
	for(i = 0;i < 8;i++)
	{
		OLED_WR_Byte (0xb0+i,OLED_DC_CMD);   			/*!< 设置页地址（0~7） 						*/ 
		OLED_WR_Byte (0x00,OLED_DC_CMD);    			/*!< 设置显示位置—列低地址 					*/  
		OLED_WR_Byte (0x10,OLED_DC_CMD);    			/*!< 设置显示位置—列高地址 					*/   
		for(j = 0;j<128 ;j++)
		{
			OLED_GRAM[j][i] = 0x00;
			OLED_WR_Byte(0,OLED_DC_DATA);
		}
		
	}
//#if OLED_FUNCTION == 1
//	#warning "函数未重编辑"			/* !< 编辑后请删除此行 */
//#endif		
}


/****************************************************************************
* 函数名称: OLED 画点函数
* 函数功能 : OLED基础函数 ，所有函数显示将调用这个函数
* 输入 :点的坐标 X(0-128) Y(0-63) 打点状态:POINT_STATU_RESET or POINT_STATU_SET
*		POINT_STATU_RESET: 将这个点变黑 (实际中很少用到，可以不考虑???)
*		POINT_STATU_SET:   将这个点变白
* 输出 :void
* 作者 :
*****************************************************************************/
void OLED_Show_Point(u8_Pro Index_X,u8_Pro Index_Y,u8_Pro POINT_STATU)
{

	if(Index_X < 128 && Index_Y <64)
	{
		OLED_GRAM[Index_X][7-(Index_Y>>3)] |= POINT_STATU << (7-(Index_Y & 0x07));
	}

//	#if OLED_FUNCTION == 1
//	#warning "函数未重编辑"			/* !< 编辑后请删除此行 */
//	#endif		
	
}


/****************************************************************************
* 函数名称: OLED 显示字符函数(需要用到OLED_FONT.h文件)
* 函数功能 : 将一个字符显示在OLED屏幕上
* 输入 :坐标 X(0~(127-charSize/2)) Y(0~(63-charSize))
*		字符 常用的英文字符、符号、数字
*		字符大小 charSize 依OLED_FONT.h 文件而定
*		打点状态
* 输出 :void
* 作者 :
*****************************************************************************/
void OLED_Show_Char(u8_Pro Index_X,u8_Pro Index_Y,u8_Pro chr,u8_Pro charSize,u8_Pro POINT_STATU)
{
// #if OLED_FUNCTION == 1
// 	#warning "函数未重编辑"			/* !< 编辑后请删除此行 */
// #endif		
	u8_Pro temp,t;
	u8_Pro chline = charSize/8 + ((charSize%8) ? 1:0);
	u8_Pro csize=chline * (charSize / 2);									/*!< 得到字体一个字符对应点阵集所占的字节数 		*/	
	chr=chr-' ';														/*!< 得到偏移后的值									*/			 
    for(t=0;t<csize;t++)
	{   
		if(charSize==12)temp=asc2_1206[chr][t]; 						/*!< 调用1206字体									*/	
		else if(charSize==16)temp=asc2_1608[chr][t]; 					/*!< 调用1608字体									*/	
		else if(charSize==24)temp=asc2_2412[chr][t]; 					/*!< 调用2412字体 									*/	
		else return;													/*!< 没有的字库 									*/	
		
		OLED_Set_Byte(Index_X+(t/chline),Index_Y+((t % chline)<<3),temp,POINT_STATU);
																		/*!< 在码表中 字符由 n 个 8为字节存储			    */
																		/*!< 字母为 size * (size/2)/8 个字节			    */
																		/*!< 排列方式为 先竖在横						    */
																		/*!< 不满 8 的倍数的 字体需要加一 如：12 为 2行6列  */
																		/*!< x坐标 每执行 行数 次 加一					    */
																		/*!< y 坐标 在 每行的第一个坐标 来回变动		    */
    } 
	

}


/****************************************************************************
* 函数名称: OLED 显示数字函数
* 函数功能 : 显示 NUMBER 的值在屏幕上 数字需要显示的区域先清零在显示 为零则显示 '0'
* 输入 :坐标 X(0~(127-charSize/2)) Y(0~(63-charSize))
*		要现实的数值 NUMBER(0~4294967295)
*		字符大小 charSize 依OLED_FONT.h 文件而定
* 输出 :void
* 作者 :
*****************************************************************************/
void OLED_Show_Num(u8_Pro Index_X,u8_Pro Index_Y,u32_Pro NUMBER,u8_Pro charSize)
{
//#if OLED_FUNCTION == 1
//	#warning "函数未重编辑"			/* !< 编辑后请删除此行 */
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
			//为什么charSize 要除以二？ 因为字母 点阵 分辨率 12*12  真实 点阵为 6*12 ，以下同理
			OLED_Show_Char(Index_X + (charSize/2) * wei ,Index_Y,(NUMBER/temp)%10+'0',charSize,POINT_STATU_SET);
			temp /= 10;
		}

	}else
	{
		OLED_Show_Char(Index_X,Index_Y,'0',charSize,POINT_STATU_SET);
	}

	}

/****************************************************************************
* 函数名称: OLED 显示 字符串函数
* 函数功能 : 将 string 中的字符串显示在屏幕上
*			一行显示不不下则换行，一页显示不下则换页
* 输入 :坐标 X(0~(127-charSize/2)) Y(0~(63-charSize))
*		字符串 string （const 常量）
*		字符大小 charSize 依OLED_FONT.h 文件而定
* 输出 :void
* 作者 :
*****************************************************************************/
void OLED_Show_String(u8_Pro Index_X,u8_Pro Index_Y,const String_Pro string,u8_Pro charSize)
{
	
//#if OLED_FUNCTION == 1
//	#warning "函数未重编辑"			/* !< 编辑后请删除此行 */
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
        
			/*!< 为什么charSize 要除以二？ 因为字母 点阵 分辨率 12*12  真实 点阵为 6*12 ，以下同理 */	
		Index_X += charSize/2;
        
		string++;
	}

	
}
/**************************************************************************
* 函数功能：向 显存中输入 一个字节的数据
*	该文件是 最重要的基础 操纵函数，用于向 显存 中写入一b 数据
* 输入：字节位置 XY 坐标 ，数据值 ，点的状态
* 输出：void
* 作者:@Kim_alittle_star
***************************************************************************/
void OLED_Set_Byte(u8_Pro Index_X,u8_Pro Index_Y,u8_Pro dat,u8_Pro POINT_STATU)
{
	u8_Pro line = (63-Index_Y) >> 3;				/*!< 确定是在第几行							 */
	u8_Pro move = (Index_Y & 0x07);				/*!< 确定角点在某行的第几个位置				 */
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
* 函数功能：屏幕反相函数
*	在制定行 从指定列开始 指定长度 内将其反相
* 输入：开始列数 & 某一行 & 长度 
* 输出：void
***************************************************************************/
void OLED_Inverting_Rectangle(u8_Pro Index_X,u8_Pro Index_Line,u8_Pro longer)
{
	//函数期望 ：使用该函数使得 该方框区域反相输出
	u8_Pro i;
	OLED_WR_Byte (0xb0+7-Index_Line,OLED_DC_CMD);   			/*!< 设置页地址（0~7） 											*/ 
	OLED_WR_Byte (0x00 | (Index_X & 0x0f),OLED_DC_CMD);    	/*!< 设置显示位置—列低地址 										*/ 
	OLED_WR_Byte (0x10 | (Index_X >> 4),OLED_DC_CMD);    		/*!< 设置显示位置—列高地址 										*/
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

