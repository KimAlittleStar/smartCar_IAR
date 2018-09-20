#ifndef _OV7725DRIVE_H_
#define _OV7725DRIVE_H_

#include "Global_Var.h"
#include "VCAN_camera.h"
#include "WIFIDrive.h"
/*------------------------�궨��  ��----------------------*/


extern u8_Pro OV_7725Rx_Img[2][CAMERA_SIZE];                             //����洢����ͼ�������
extern u8_Pro OV_Img[CAMERA_H][CAMERA_W];                           //����ӥ������ͷ��һ�ֽ�8�����أ������Ҫ��ѹΪ 1�ֽ�1�����أ����㴦��
extern u8_Pro* OV_Rx_ImgOne;


/*------------------------����������----------------------*/

/* !< ����ͷ����ʹ�ú��� */

u8_Pro OV_Init(void);
void OV_Window_Set(u16_Pro sx,u16_Pro  sy,u16_Pro width,u16_Pro height);
void OV_Set_BorW_Referencr(u8_Pro ReValue);
void OV_Sending(uint8 *imgaddr, uint32 imgsize);
void IMG_externd(void);

void camera_get_img(void);


/* !< DMA ģ���������� */

void OV_Receive_Of_DMA_Init(void);
void OV_ReStarDMA(void);
void PORTC_IRQHandler();
void DMA1_IRQHandler();



/* !< ����ͷ SCCB �����  */

u8_Pro OV_SCCB_WR_Reg(u8_Pro reg,u8_Pro data);
u8_Pro OV_SCCB_RD_Reg(u8_Pro reg);


/* !< ����ͷ SCCB/IIC �������� */

void OV_IIC_Star(void);
void OV_IIC_END(void);
void OV_I2C_SCCB_No_Ack(void);
u8_Pro OV_I2C_RD_Byte(void);
u8_Pro OV_I2C_WR_Byte(u8_Pro dat);


#endif

