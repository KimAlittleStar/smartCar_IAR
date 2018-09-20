#ifndef _OV7725DRIVE_H_
#define _OV7725DRIVE_H_

#include "Global_Var.h"
#include "VCAN_camera.h"
#include "WIFIDrive.h"
/*------------------------宏定义  区----------------------*/


extern u8_Pro OV_7725Rx_Img[2][CAMERA_SIZE];                             //定义存储接收图像的数组
extern u8_Pro OV_Img[CAMERA_H][CAMERA_W];                           //由于鹰眼摄像头是一字节8个像素，因而需要解压为 1字节1个像素，方便处理
extern u8_Pro* OV_Rx_ImgOne;


/*------------------------函数声明区----------------------*/

/* !< 摄像头基本使用函数 */

u8_Pro OV_Init(void);
void OV_Window_Set(u16_Pro sx,u16_Pro  sy,u16_Pro width,u16_Pro height);
void OV_Set_BorW_Referencr(u8_Pro ReValue);
void OV_Sending(uint8 *imgaddr, uint32 imgsize);
void IMG_externd(void);

void camera_get_img(void);


/* !< DMA 模块基本命令函数 */

void OV_Receive_Of_DMA_Init(void);
void OV_ReStarDMA(void);
void PORTC_IRQHandler();
void DMA1_IRQHandler();



/* !< 摄像头 SCCB 命令函数  */

u8_Pro OV_SCCB_WR_Reg(u8_Pro reg,u8_Pro data);
u8_Pro OV_SCCB_RD_Reg(u8_Pro reg);


/* !< 摄像头 SCCB/IIC 基础函数 */

void OV_IIC_Star(void);
void OV_IIC_END(void);
void OV_I2C_SCCB_No_Ack(void);
u8_Pro OV_I2C_RD_Byte(void);
u8_Pro OV_I2C_WR_Byte(u8_Pro dat);


#endif

