/****************************************************************************
* �ļ����� : ֱ�������������ļ� ����
* �ļ����� : ʵ�� ������������й���
* ע������ :
*****************************************************************************/
#include "MotorDrive.h"

u16_Pro Motor_Speed_Now = 0;		/* !< �����ǰ�ٶ� 0~������ֵ */
u32_Pro Motor_FeedBack_SpeedTime;	/* !< ��������ȡ�ĵ���ٶȷ���  */


/****************************************************************************
* ��������: ֱ�����������ʼ������
* �������� : ��ֱ������ȹض�ʱ�� IO�ڳ�ʼ��
* ���� :void
* ��� :void
* ���� :
*****************************************************************************/
void Motor_Init(void)
{
	ftm_pwm_init(FTM0,FTM_CH2,10000,MOTOR_CH1_DUTY);
	
	ftm_pwm_init(FTM0,FTM_CH1,10000,MOTOR_CH1_DUTY);
	
	gpio_init(PTD15,GPO,HIGH);
	gpio_init(PTE27,GPO,HIGH);

	// #if MOTOR_FUNCTION ==1
	// #warning "����δ�ر༭"			/* !< �༭����ɾ������ */
	// #endif
	
}




/****************************************************************************
* ��������: ֱ��������� �����ٶȺ���
* �������� : ��ֱ����� �����ٶ����� ռ�ձ�Ϊ ��Motor_Speed/������ֵ
*			�������ж��Ƿ� �����ں���Χ�� ���ڣ��޸ĵ�������ٶ�ֵ
*										�粻�ڣ��������֮�����޸ĵ�������ٶ�ֵ
* ���� :Motor_Speed (0~������ֵ)
* ��� :void
* ���� :
*****************************************************************************/
void Motor_Set_Speed(int16_Pro Motor_Speed)
{
	Motor_Speed = (Motor_Speed>1000)?(1000):(Motor_Speed);
	Motor_Speed_Now = Motor_Speed;
	ftm_pwm_duty(FTM0,FTM_CH2,Motor_Speed);
//	ftm_pwm_duty(FTM0,FTM_CH1,Motor_Speed);

	// Motor_Speed_Now = Motor_Speed;
	// #if MOTOR_FUNCTION ==1
	// #warning "����δ�ر༭"			/* !< �༭����ɾ������ */
	// #endif
}


/****************************************************************************
* ��������: ֱ��������� ��Ե��ٶ�
* �������� : ʹ������� �������ֵΪ��Motor_IncValue/������ֵ
* ���� :Motor_Value (0~(������ֵ-Motor_Speed_Now))
* ��� :void
* ���� :
*****************************************************************************/
void Motor_Increase_Speed(int16_Pro Motor_IncValue)
{
	Motor_Speed_Now += Motor_IncValue;
	Motor_Set_Speed(Motor_Speed_Now);
//	#if MOTOR_FUNCTION ==1
//	#warning "����δ�ر༭"			/* !< �༭����ɾ������ */
//	#endif
}

/****************************************************************************
* ��������: ֱ��������� ��Ե��ٶ�
* �������� : ʹ������� �������ֵΪ��Motor_RedValue/������ֵ
* ���� :Motor_Value (0~Motor_Speed_Now)
* ��� :void
* ���� :
*****************************************************************************/
void Motor_Reduce_Speed(int16_Pro Motor_RedValue)
{
	if(Motor_RedValue < Motor_Speed_Now)
	{
		Motor_Speed_Now -= Motor_RedValue;
		Motor_Set_Speed(Motor_Speed_Now);
	}
	
//	#if MOTOR_FUNCTION ==1
//	#warning "����δ�ر༭"			/* !< �༭����ɾ������ */
//	#endif
//
}


/****************************************************************************
* ��������: ��ȡ��ǰ�����ת��(�Ƽ�ֻ�ô˺��� ֱ�� ��ȡ Motor_Speed_Now ����)
* �������� : ��ȡ��ǰ�����ת��
* ���� :void
* ��� :��ǰ���ת�� Motor_Speed_Now
* ���� :
*****************************************************************************/
int16_Pro Motor_GetSpeed_Of_Now(void)
{
	return Motor_Speed_Now;
}

