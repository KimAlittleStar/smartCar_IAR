#ifndef _VCAN_OV7725_EAGLE_H_
#define _VCAN_OV7725_EAGLE_H_

#include "VCAN_OV7725_REG.h"

#if ( USE_CAMERA == CAMERA_OV7725_EAGLE )
//��������ͷ������
#define CAMERA_USE_HREF     0               //�Ƿ�ʹ�� ���ж� (0 Ϊ ��ʹ�ã�1Ϊʹ��)
#define CAMERA_COLOR        0               //����ͷ�����ɫ �� 0 Ϊ �ڰ׶�ֵ��ͼ�� ��1 Ϊ �Ҷ� ͼ�� ��2 Ϊ RGB565 ͼ��
#define CAMERA_POWER        0               //����ͷ ��Դѡ�� 0 Ϊ 3.3V ,1 Ϊ 5V

//��������ͷ����ӿ�
#define camera_init(imgaddr)    ov7725_eagle_init(imgaddr)
//#define camera_get_img()        ov7725_eagle_get_img()

#define camera_vsync()          ov7725_eagle_vsync()
#define camera_href()           //ov7725_eagle_href()
#define camera_dma()            ov7725_eagle_dma()

//���� ����ͷ ����
#define CAMERA_DMA_CH       OV7725_EAGLE_DMA_CH         //��������ͷ��DMA�ɼ�ͨ��
#define CAMERA_W            OV7725_EAGLE_W              //��������ͷͼ����
#define CAMERA_H            OV7725_EAGLE_H              //��������ͷͼ��߶�

#define CAMERA_SIZE         OV7725_EAGLE_SIZE           //ͼ��ռ�ÿռ��С
#define CAMERA_DMA_NUM      OV7725_EAGLE_DMA_NUM        //DMA�ɼ�����

#endif  //#if ( USE_CAMERA == CAMERA_OV7725_EAGLE )


//��������ͷ ����
#define OV7725_EAGLE_DMA_CH       DMA_CH1                               //��������ͷ��DMA�ɼ�ͨ��
#define OV7725_EAGLE_W            80                                    //��������ͷͼ����
#define OV7725_EAGLE_H            60                                    //��������ͷͼ��߶�
#define OV7725_EAGLE_SIZE         (OV7725_EAGLE_W * OV7725_EAGLE_H/8 )  //ͼ��ռ�ÿռ��С
#define OV7725_EAGLE_DMA_NUM      (OV7725_EAGLE_SIZE )                  //DMA�ɼ�����


//�û��Զ����ź�����
#define VSYNC_PIN PTC6                  /*!< ���ж��ź� */
#define VSYNC_IN PTC6_IN

#define HERF_PIN PTC8                   /*!< ���ж��ź� */
#define HERF_IN PTC8_IN

#define PCLK_PIN PTC5                   /*!< ����ͷʱ���ź� */
#define PCLK_IN PTC5_IN





extern  uint8   ov7725_eagle_init(uint8 *imgaddr);
extern  uint8*    ov7725_eagle_get_img(void);

extern  void    ov7725_eagle_vsync(void);
extern  void    ov7725_eagle_dma(void);


#endif  //_VCAN_OV7725_EAGLE_H_


