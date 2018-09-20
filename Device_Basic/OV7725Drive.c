/****************************************************************************
* 文件名称 : OV7725 摄像头驱动文件
* 文件功能 : OV7725 相关驱动文件在这里被实现
* 注意事项 :
*****************************************************************************/

#include "OV7725Drive.h"

u16_Pro OV_Refresh_FPS = 0;				/* !< 摄像头数据接收刷新数(每秒清 0) */
u8_Pro OV_ImgOVER_Flag = 0;				/* !< 图像接收完成标志	 */
// u8_Pro OV_Img[60][80] = {0};			/* !< 真数据存储数组		 */
// u8_Pro OV_7725Rx_Img[240][40] = {0};	/* !< 摄像头接收数据数组  */

u8_Pro OV_7725Rx_Img[2][CAMERA_SIZE];                             //定义存储接收图像的数组
u8_Pro OV_Img[CAMERA_H][CAMERA_W];                           //由于鹰眼摄像头是一字节8个像素，因而需要解压为 1字节1个像素，方便处理
u8_Pro* OV_Rx_ImgOne;

/****************************************************************************
* 函数名称: OV 摄像头初始化函数
* 函数功能 : 初始化 摄像头所需要的 IO 口 初始化设置摄像头的寄存器
* 输入 :void
* 输出 :void
* 作者 :
*****************************************************************************/
u8_Pro OV_Init(void)
{
	u8_Pro init_success_flag = 0;
	init_success_flag = camera_init(OV_7725Rx_Img[0]);
	set_vector_handler(PORTC_VECTORn ,PORTC_IRQHandler);    //设置LPTMR的中断复位函数为 PORTA_IRQHandler
    set_vector_handler(DMA1_VECTORn ,DMA1_IRQHandler);      //设置LPTMR的中断复位函数为 DMA0_IRQHandler

// #if OV_FUNCTION == 1
// 	#warning "函数未重写"		/*!< 重写后请删除此段 */
// #endif
	return init_success_flag;
}

void IMG_externd(void)
{
	u16_Pro i;
	u8_Pro* Ov_ImgP = *OV_Img;
	for(i = 0;i<600;i++)
	{
		*(Ov_ImgP	++) =  (OV_Rx_ImgOne[i] &0x80)?(0x00):(0xFF);
		*(Ov_ImgP++)  =  (OV_Rx_ImgOne[i] &0x40)?(0x00):(0xFF);
		*(Ov_ImgP++)  =  (OV_Rx_ImgOne[i] &0x20)?(0x00):(0xFF);
		*(Ov_ImgP++)  =  (OV_Rx_ImgOne[i] &0x10)?(0x00):(0xFF);
		*(Ov_ImgP++)  =  (OV_Rx_ImgOne[i] &0x08)?(0x00):(0xFF);
		*(Ov_ImgP++)  =  (OV_Rx_ImgOne[i] &0x04)?(0x00):(0xFF);
		*(Ov_ImgP++)  =  (OV_Rx_ImgOne[i] &0x02)?(0x00):(0xFF);
		*(Ov_ImgP++)  =  (OV_Rx_ImgOne[i] &0x01)?(0x00):(0xFF);
	}
}

/**********************************************************************************
* 函数名称: void camera_get_img
* 函数功能: 配合山外鹰眼 驱动程序 接口 如果使用此函数 请将其注释 并 恢复 VCAN_OV7725_Eagle.h 中14行 宏定义即可
* 函数输入: void input:
* 函数输出: void output:
* 作者 :author:@Kim_alittle_star
**********************************************************************************/
void camera_get_img(void)
{
    OV_Rx_ImgOne =  ov7725_eagle_get_img();
	OV_Refresh_FPS++;
	
}


/****************************************************************************
* 函数名称: 摄像头 设置 输出的图像大小 位置 
* 函数功能 : 设置摄像头输出的图像数据大小，从 sx sy 开始输出，大小为 width * height
* 输入 :sx/sy 开始的X.Y坐标
*		width 长	height 宽
* 输出 :void
* 作者 :
*****************************************************************************/
void OV_Window_Set(u16_Pro sx,u16_Pro  sy,u16_Pro width,u16_Pro height)
{
#if OV_FUNCTION == 1
	#warning "函数未重写"		/*!< 重写后请删除此段 */
#endif
}


/****************************************************************************
* 函数名称: 通过此函数 设置二值化阈值
* 函数功能 : 此函数接收一个值 以这一参考量设置 摄像头二值化阈值
* 输入 :Reference:参考值
* 输出 :void
* 作者 :
*****************************************************************************/
void OV_Set_BorW_Referencr(u8_Pro ReValue)
{
#if OV_FUNCTION == 1
	#warning "函数未重写"		/*!< 重写后请删除此段 */
#endif
}

/****************************************************************************
* 函数名称: OV7725 所需要的 DMA 模块输入 初始化
* 函数功能 : 将 7725 所需要的 DMA模块初始化
* 输入 :void		(此函数具体传参不确定单功能不变)
* 输出 :void
* 作者 :
*****************************************************************************/
void OV_Receive_Of_DMA_Init(void)
{
#if OV_FUNCTION == 1
	#warning "函数未重写"		/*!< 重写后请删除此段 */
#endif
}


/****************************************************************************
* 函数名称: DMA 重新开始函数
* 函数功能 : 此函数用于一帧图形传输完成后 。下一帧图像开始前 重新开始图像传输
* 输入 :void
* 输出 :void
* 作者 :
*****************************************************************************/
void OV_ReStarDMA(void)
{
#if OV_FUNCTION == 1
	#warning "函数未重写"		/*!< 重写后请删除此段 */
#endif
}

/****************************************************************************
* 函数名称: 写摄像头 寄存器指令函数
* 函数功能 : 将地址为 reg 的寄存器 中的值 写为 data
* 输入 :寄存器地址reg   指令内容 data
* 输出 :写入成功 为1 失败为 0
* 作者 :
*****************************************************************************/
u8_Pro OV_SCCB_WR_Reg(u8_Pro reg,u8_Pro data)
{
	
#if OV_FUNCTION == 1
	#warning "函数未重写"		/*!< 重写后请删除此段 */
#endif
	return 0;
}


/****************************************************************************
* 函数名称: 读寄存器数值
* 函数功能 : 将 地址为 reg 中的内容读取出来
* 输入 :寄存器地址reg
* 输出 :该寄存器中的 内容
* 作者 :
*****************************************************************************/
u8_Pro OV_SCCB_RD_Reg(u8_Pro reg)
{
#if OV_FUNCTION == 1
	#warning "函数未重写"		/*!< 重写后请删除此段 */
#endif
	return 0;
}

/****************************************************************************
* 函数名称: IIC 协议开始 时序
* 函数功能 : IIC 协议开始 时序
* 输入 :void
* 输出 :void
* 作者 :
*****************************************************************************/
void OV_IIC_Star(void)
{
#if OV_FUNCTION == 1
	#warning "函数未重写"		/*!< 重写后请删除此段 */
#endif
}


/****************************************************************************
* 函数名称: IIC 协议结束时序
* 函数功能 :  IIC 协议结束时序
* 输入 :void
* 输出 :void
* 作者 :
*****************************************************************************/
void OV_IIC_END(void)
{
#if OV_FUNCTION == 1
	#warning "函数未重写"		/*!< 重写后请删除此段 */
#endif
}


/****************************************************************************
* 函数名称: SCCB 协议特有 每传输一个值 空一个位 此为 接收无用的空位
* 函数功能 :  SCCB 协议特有 每传输一个值 空一个位 此为 接收无用的空位
* 输入 :void
* 输出 :void
* 作者 :
*****************************************************************************/
void OV_I2C_SCCB_No_Ack(void)
{
#if OV_FUNCTION == 1
	#warning "函数未重写"		/*!< 重写后请删除此段 */
#endif
}


/****************************************************************************
* 函数名称: IIC 协议读取数据函数
* 函数功能 : IIC 协议读取数据函数
* 输入 :void
* 输出 :总线上读到的数据
* 作者 :
*****************************************************************************/
u8_Pro OV_I2C_RD_Byte(void)
{
#if OV_FUNCTION == 1
	#warning "函数未重写"		/*!< 重写后请删除此段 */
#endif	
	//return 0;
}

/****************************************************************************
* 函数名称: IIC 总线协议 写一个数据
* 函数功能 : IIC 总线协议 写一个数据
* 输入 :数据 value
* 输出 :发送状态 成功 0，不成功 1 
* 作者 :
*****************************************************************************/
u8_Pro OV_I2C_WR_Byte(u8_Pro dat)
{
#if OV_FUNCTION == 1
	#warning "函数未重写"		/*!< 重写后请删除此段 */
#endif
}


/*!
 *  @brief      PORTA中断服务函数
 *  @since      v5.0
 */
void PORTC_IRQHandler()
{
    uint8  n = 0;    //引脚号
    uint32 flag = PORTC_ISFR;
    PORTC_ISFR  = ~0;                                   //清中断标志位

    n = 6;                                             //场中断
    if(flag & (1 << n))                                 //PTA29触发中断
    {
        camera_vsync();
    }
#if 0             //鹰眼直接全速采集，不需要行中断
    n = 28;
    if(flag & (1 << n))                                 //PTA28触发中断
    {
        camera_href();
    }
#endif
}

/*!
 *  @brief      DMA0中断服务函数
 *  @since      v5.0
 */
void DMA1_IRQHandler()
{
    camera_dma();
}

void OV_Sending(uint8 *imgaddr, uint32 imgsize)
{
    u8_Pro cmd[4] = {0x01,0xFE, 0xFE, 0x01 };    //yy_摄像头串口调试 使用的命令
//    uart_putbuff(VCAN_PORT, cmd, sizeof(cmd));    //先发送命令
	
	u8_Pro temp[20];
// #if WIFI_FUNCTION == 1
// 	#warning "函数未重写"		/*!< 重写后请删除此段 */
// #endif
	// sprintf((char*)temp,"AT+CIPSEND=0,%d",sizeof(cmd));
	// WIFI_CMD(temp,"OK",20);
    // WIFI_prinft("%d%d%d%d\r\n",cmd[0],cmd[1],cmd[2],cmd[3]);

	

// #if WIFI_FUNCTION == 1
// 	#warning "函数未重写"		/*!< 重写后请删除此段 */
// #endif
#if 1	
	sprintf((char*)temp,"AT+CIPSEND=0,%d",imgsize+7);
	WIFI_CMD(temp,"OK",20);
	WIFI_prinft("%c,%c",cmd[0],cmd[1]);
	uart_putchar(WIFI_Uart,cmd[0]);
	uart_putchar(WIFI_Uart,cmd[1]);
	uart_putbuff(WIFI_Uart, imgaddr, imgsize); //再发送图像
	uart_putchar(WIFI_Uart,cmd[2]);
	uart_putchar(WIFI_Uart,cmd[3]);
	WIFI_prinft("\r\n");
#else
	uart_putchar(Kim_UsartPort,cmd[0]);	
	uart_putchar(Kim_UsartPort,cmd[1]);   
	uart_putbuff(Kim_UsartPort, imgaddr, imgsize);
	uart_putchar(Kim_UsartPort,cmd[2]);	
	uart_putchar(Kim_UsartPort,cmd[3]);
#endif	   
	
//    uart_putbuff(VCAN_PORT, imgaddr, imgsize); //再发送图像
}


