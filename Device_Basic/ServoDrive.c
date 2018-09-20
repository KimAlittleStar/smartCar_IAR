/****************************************************************************
* �ļ����� : ServoDrive.h
* �ļ����� : ������������ ����
* ע������ :
*****************************************************************************/

#include "ServoDrive.h"

int16_Pro Servo_Default_Angle = 860;
int16_Pro Servo_Angle_Now = 0;
int16_Pro Servo_MaxTurn_Angle = 450;
Float_Pro Servo_FeedBack_DirDeviation = 0;				/* !< ����ͷ�� �� ����ͷ�������			 		*/

/****************************************************************************
* ��������: �����ʼ������
* �������� : ���������Ҫ�� PWM�� IO �� ����س�ʼ�� ���� ����Ƕȵ���Ϊ Ĭ�ϽǶ�
* ���� :void
* ��� :void
* ���� :
*****************************************************************************/
void Servo_Init(void)
{
// #if SERVO_FUNCTION == 1
// 	#warning "����δ��д"		/*!< ��д����ɾ���˶� */
// #endif
	ftm_pwm_init(FTM1,FTM_CH0,50,0);
	Servo_Set_Angle(Servo_Default_Angle);
}


/****************************************************************************
* ��������: ������ýǶ� Ϊ ang
* �������� : ������ýǶ� Ϊ ang
* ���� :�Ƕ� ��ֵ ang (45~135)
* ��� :void
* ���� :
*****************************************************************************/
void Servo_Set_Angle(int16_Pro ang)
{
// #if SERVO_FUNCTION == 1
// 	#warning "����δ��д"		/*!< ��д����ɾ���˶� */
// #endif


//ʹ��������㷨�����Ż�
#if 0
	ang &= 0X7FFF;
	if(ang < (Servo_Default_Angle-Servo_MaxTurn_Angle))
	{/* !< С����Сת�� */
		ang = Servo_Default_Angle-Servo_MaxTurn_Angle;		
	}else if(ang > (Servo_Default_Angle+Servo_MaxTurn_Angle))
		{/* !< �������ת�� */
			ang = Servo_Default_Angle+Servo_MaxTurn_Angle;
		}
#else
	ang = (ang < (Servo_Default_Angle-Servo_MaxTurn_Angle))?/* !< С����Сת�� */
			( Servo_Default_Angle-Servo_MaxTurn_Angle):
			((ang > (Servo_Default_Angle+Servo_MaxTurn_Angle))?/* !< �������ת�� */
			(ang = Servo_Default_Angle+Servo_MaxTurn_Angle):
			(ang)
			);
#endif
	Servo_Angle_Now = ang;
	u16_Pro temp = (ang + 450) * 1000/1800;			/*!< ���ݽǶ�����������ռ�ձ�  */
	/*!< u16_Pro temp = (ang + 45) * 10000/1800; */
	ftm_pwm_duty(FTM1,FTM_CH0,temp);
}


/****************************************************************************
* ��������: �����ת����
* �������� : ����� �����Ƕ� left ��
* ���� :��ת�Ƕ� (0~(Ĭ��ƫ��+���ƫ��-��ǰƫ��))
* ��� :void
* ���� :
*****************************************************************************/
void Servo_SetLeft_Angle(int16_Pro left)
{
// #if SERVO_FUNCTION == 1
// 	#warning "����δ��д"		/*!< ��д����ɾ���˶� */
// #endif
	/* !< ���� */
	if(left < Servo_Angle_Now)
	{
		
		Servo_Set_Angle(Servo_Angle_Now - left);
	}else
	{
		Servo_Set_Angle(0);
	}
}


/****************************************************************************
* ��������: �����ת����
* �������� : ����� �����Ƕ� left ��
* ���� :��ת�Ƕ� (0~(��ǰƫ��-(Ĭ��ƫ��-���ƫ��)))
* ��� :void
* ���� :
*****************************************************************************/
void Servo_SetRight_Angle(int16_Pro right)
{
// #if SERVO_FUNCTION == 1
// 	#warning "����δ��д"		/*!< ��д����ɾ���˶� */
// #endif
	if((right+Servo_Angle_Now)< 180)
	{
		Servo_Set_Angle(right + Servo_Angle_Now);		
	}
	else
	{
		Servo_Set_Angle(180);
	}
}



