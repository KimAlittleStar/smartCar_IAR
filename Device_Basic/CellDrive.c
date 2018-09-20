/***************************************************************************
* �ļ����� : CellDrive.c
* �ļ����� : ����� ���������ڶ��ٵ���ʱ ���屨��
* ע������ :
*****************************************************************************/
#include "CellDrive.h"
#include "EncordDrive.h"
#include "MotorDrive.h"
#include "OledDrive.h"
#include "KeyScan.h"
u16_Pro Cell_Vsum = 0;
/****************************************************************************
* ��������: ��ؼ���ʼ������
* �������� : ������ص� ADCģ���ʼ��
* ���� :void
* ��� :void
* ���� :
*****************************************************************************/
void Cell_Init(void)
{
	// #if CELL_FUNCTION ==1
	// #warning "����δ�ر༭"			/* !< �༭����ɾ������ */
	// #endif
	u8_Pro i;
	adc_init(ADC0_SE16);	      // ADC0_SE16
	gpio_init(PTE24,GPO,LOW);
	Cell_BEEP_ON;
	DELAY_MS(200);
	Cell_BEEP_OFF;
	i = Key_Of_Cancel(KEY_MODE_Once);
	DELAY_MS(500);
	i = Key_Of_Cancel(KEY_MODE_Continuous);
	if(i == 0)
	{
		pit_init_ms(PIT2,200);
		/*!< �ض����жϺ��� */
		set_vector_handler(PIT2_VECTORn,Cell_PIT2_Protect_NVIC);
		enable_irq(PIT2_IRQn);
		/*!< ʹ���жϺ��� */
	}else
	{
	  	Cell_BEEP_ON;
		DELAY_MS(200);
		Cell_BEEP_OFF;
	}


}


/****************************************************************************
* ��������: �õ���صĵ�ѹֵ
* �������� : ��ADC ģ���еõ� ��ص�ǰ��ѹ
*	�õ��� ȷ��ֵ  3300 ʵ��ֵΪ  3.300 V
* ���� :void
* ��� :��ǰ��ص�ѹ ѹֵ3300 ʵ��ֵΪ  3.300 V
* ���� :
*****************************************************************************/
u16_Pro Cell_GetBatteryPer(void)
{
	u16_Pro CELL_A = 0;
// 	#if CELL_FUNCTION ==1
// 	#warning "����δ�ر༭"			/* !< �༭����ɾ������ */
// #endif
	CELL_A =  adc_once(ADC0_SE16,ADC_16bit);			/* ȥADC0 ģ��� SE8 ͨ�� ģ���� ���ݴ�СΪ 8λ */
	
	return 3300*CELL_A/0xFFFF;
}

/**********************************************************************************
* ��������: void Cell_Protection(void);
* ��������: ��ص�Դ�������ܣ���ֹ�̽ӵ������� ���ŵ��
* ��������: void input:
* �������: void output:
* ���� :author:@Kim_alittle_star
**********************************************************************************/
void Cell_Protection(void)
{
  static u8_Pro Protect[2] = {0,0};
	if(Protect[0] > 3)
	{
		Protect[0] = 0;
		if((Cell_Vsum>>2)>2065 && Protect[1]<=2)
		{
		  Protect[1] = 0;
		  Cell_Vsum = 0;
		}else
		{
			if((Cell_Vsum>>2)<=2265)
		  		Cell_Warning("Voltage");
			else
			{
				Cell_Warning("speed");
			}
		}

	}else
	{
		Protect[0]++;
		Cell_Vsum += Cell_GetBatteryPer();
		if(Motor_Speed_Now > 850 && Encord_GetMotorTime()<50)
		{
			Protect[1]++;
		}
	}

}

void Cell_Warning(String_Pro warning)
{
	u8_Pro* B = (u8_Pro* )BBmp;
	u8_Pro i,j;
	Motor_Set_Speed(MOTOR_CH1_DUTY);
	Encord_GetSpeed_Close();
	DisableInterrupts;	/*!< �����ж� ����ģʽ */
	while(1)
	{
		for(i = 0;i<8;i++)
		{
			for(j = 0;j<128;j++)
			{
				OLED_Set_Byte(j,i*8,*B,POINT_STATU_SET);
				B++;
			}
		}
		OLED_Refrech_Gram();
		PTE24_OUT = 0;
		DELAY_MS(750);
		OLED_ResetGRAM();
		OLED_Show_String(0,0,warning,charSize16);
		OLED_Refrech_Gram();
		PTE24_OUT = 1;
		DELAY_MS(250);
		OLED_ResetGRAM();
		B =(u8_Pro* ) BBmp;
	}
}

void Cell_PIT2_Protect_NVIC(void)
{
	static u8_Pro i = 0;
	PIT_Flag_Clear(PIT2);		 /* ��� ��ʱ�ж� */
	/* ����� PIT_Flag_Clear �������� ��ô �жϽ��������������� */
	// DisableInterrupts;		/* ���ܽ������ж϶� ���� ֻ������ ��ʱ�������ж� */

	Cell_Protection();
	i++;
	if(i == 5)
	{
		OLED_Refrech_FPS = OV_Refresh_FPS;
		OV_Refresh_FPS = 0;
		i = 0;
//		printf("%d",OLED_Refrech_FPS);
	}
}