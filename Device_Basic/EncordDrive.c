/****************************************************************************
* �ļ����� : EncordDrive.h
* �ļ����� : �������Ļ������ƹ���
* ע������ :
*****************************************************************************/

#include "EncordDrive.h"

u32_Pro Encord_IRQ_COUNT = 0;
u32_Pro Encord_Hz = 0;
u16_Pro Encord_NVIC_Time_Weith = 0;
/****************************************************************************
* �������� : ������������ʼ��
* �������� : ��ʼ�������� ɨ��Ƶ��1M ���� ���˲�
*			 ����ʱ��ʹ���жϷ���������
* ���� :void
* ��� :void
* ���� :
*****************************************************************************/
void Encord_Init(void)
{
// 	#if ENCORD_FUNCTION ==1
// 	#warning "����δ�ر༭"			/* !< �༭����ɾ������ */
// #endif
	ftm_input_init(FTM2,FTM_CH0,FTM_Rising,FTM_PS_1);	/* ���� TFM2 Ϊ ���벶�������� ��ʼ�����˲� */
	set_vector_handler(FTM2_VECTORn, NVIC_Encord_FTM_IRQHander);
	set_vector_handler(PIT0_VECTORn,NVIC_Encord_PIT_IRQHander);
//	enable_irq(FTM2_IRQn);
	pit_init_ms(PIT0,20);			//��ʱ�ж�  1 ��һ��;
	Encord_NVIC_Time_Weith = (1000/20);		/* ���ڼ��㺯�� ���� һ�������м��� �ж� */
//	enable_irq(PIT0_IRQn);
}

/****************************************************************************
* ��������: 
* �������� : 
* ���� :
* ��� :
* ���� :
*****************************************************************************/
u32_Pro Encord_GetMotorTime(void)
{
//	Motor_FeedBack_SpeedTime =0;
	
	// #if ENCORD_FUNCTION ==1
	// #warning "����δ�ر༭"			/* !< �༭����ɾ������ */
	// #endif
	return Encord_Hz;
}


/****************************************************************************
* ��������: �������жϵȼ��ȳ�ʼ������
* �������� : ��ʼ�� ����������ж� �ṹ�庯��
* ���� : void
* ��� : void
* ���� :
*****************************************************************************/
void Encord_NVIC_Init(void)
{
	#if ENCORD_FUNCTION ==1
	#warning "����δ�ر༭"			/* !< �༭����ɾ������ */
	#endif
}


void NVIC_Encord_FTM_IRQHander(void)
{
 //   u8_Pro s = FTM2_STATUS;          //��ȡ��׽�ͱȽ�״̬  All CHnF bits can be checked using only one read of STATUS.
 //   u8_Pro CHn;


 //   CHn = 0;						/* ���ͨ��0  �ж�״̬ */
    if( FTM2_STATUS & 0x01 )		/* ftm2_status & 1<<CHn */
    {	     
  /*     �û�����       */			
       Encord_IRQ_COUNT++;
		FTM2_STATUS = 0x00;				/* ����ж�״̬		 */
		
        /*********************/
    }
}
void NVIC_Encord_PIT_IRQHander(void)
{
	PIT_Flag_Clear(PIT0);		 /* ��� ��ʱ�ж� */
	/* ����� PIT_Flag_Clear �������� ��ô �жϽ��������������� */
	// DisableInterrupts;		/* ���ܽ������ж϶� ���� ֻ������ ��ʱ�������ж� */
	disable_irq(FTM2_IRQn);
	Encord_Hz = Encord_IRQ_COUNT;
	Encord_IRQ_COUNT = 0;
	enable_irq(FTM2_IRQn);

	
	// EnableInterrupts;
}
/**********************************************************************************
* ��������: Encord_GetSpeed_Prepare
* ��������: �����������ɼ�����
* ��������: void input:
* �������: void output:
* ���� :author:@Kim_alittle_star
**********************************************************************************/
void Encord_GetSpeed_Prepare(void)
{
	enable_irq(FTM2_IRQn);
	enable_irq(PIT0_IRQn);
}

/**********************************************************************************
* ��������: Encord_GetSpeed_Close
* ��������: �رձ������ɼ�����
* ��������: void input:
* �������: void output:
* ���� :author:@Kim_alittle_star
**********************************************************************************/
void Encord_GetSpeed_Close(void)
{
	disable_irq(FTM2_IRQn);
	disable_irq(PIT0_IRQn);
}
