/****************************************************************************
* �ļ����� : KeyScan.c
* �ļ����� : ��ʼ������ ��غ��� ɨ���жϰ���
* ע������ :
*****************************************************************************/
#include "KeyScan.h"

u8_Pro key_up1 = 0;
u8_Pro key_up2 = 0;
u8_Pro key_up3 = 0;
u8_Pro key_up4 = 0;

/****************************************************************************
* ��������: ������ʼ������ ��ʼ������
* �������� : ���� IO �� ʹ��Ϊ ����ʹ��
* ���� : void
* ��� : void
* ���� :
*****************************************************************************/
void KeyScan_Init(void)
{
	gpio_init(Key_Cancel,GPI,0);
	gpio_init(Key_Confirm,GPI,0);
	gpio_init(Key_Up,GPI,0);
	gpio_init(Key_Down,GPI,0);
	port_init_NoALT(Key_Cancel,PULLUP);
	port_init_NoALT(Key_Confirm,PULLUP);
	port_init_NoALT(Key_Up,PULLUP);
	port_init_NoALT(Key_Down,PULLUP);
	// #if KEY_FUNCTION ==1
	// #warning "����δ�ر༭"			/* !< �༭����ɾ������ */
	// #endif
}


/****************************************************************************
* ��������: ȷ�ϼ� ����״̬ȷ������
* �������� : �ж� ȷ�ϼ��Ƿ��¡�
* ���� :����ɨ��ģʽ: KEY_MODE_Once  or  KEY_MODE_Continuous
* ��� :����״̬ wei���� Ϊ�� == 0  or ����  ���� != 0
* ���� :
*****************************************************************************/
u8_Pro Key_Of_Confirm(u8_Pro KEY_MODE)
{
	if(KEY_MODE)key_up1=1;  //֧������		  
	if(key_up1 &&  Key_Confirm_IN == 0)
	{
		DELAY_MS(10);
		key_up1=0;
		if( Key_Confirm_IN== 0)
			return 1;
	}else if( Key_Confirm_IN==1)
			key_up1=1; 	   
 	return 0;// �ް�������
	// u8_Pro key_precess = 0;
	// #if KEY_FUNCTION ==1
	// #warning "����δ�ر༭"			/* !< �༭����ɾ������ */
	// #endif
	
}


/****************************************************************************
* ��������: ȡ���� ����״̬ȷ������
* �������� : �ж� ȡ�����Ƿ��Ѿ�����
* ���� :����ɨ��ģʽ: KEY_MODE_Once  or  KEY_MODE_Continuous
* ��� :����״̬ �Ѱ��� Ϊ�� == 0  or ����  ���� != 0
* ���� :
*****************************************************************************/
u8_Pro Key_Of_Cancel(u8_Pro KEY_MODE)
{
	if(KEY_MODE)key_up2=1;  //֧������		  
	if(key_up2 &&  Key_Cancel_IN == 0)
	{
		DELAY_MS(10);
		key_up2=0;
		if( Key_Cancel_IN== 0)
			return 1;
	}else if( Key_Cancel_IN==1)
			key_up2=1; 	   
 	return 0;// �ް�������

	// u8_Pro key_precess = 0;
	// #if KEY_FUNCTION ==1
	// #warning "����δ�ر༭"			/* !< �༭����ɾ������ */
	// #endif
	// return key_precess;
}


/****************************************************************************
* ��������: ���ϰ���״̬ȷ������
* �������� : �ж����ϰ����Ƿ���
* ���� :����ɨ��ģʽ: KEY_MODE_Once  or  KEY_MODE_Continuous
* ��� :����״̬ �Ѱ��� Ϊ�� == 0  or ����  ���� != 0
* ���� :
*****************************************************************************/
u8_Pro Key_Of_Up(u8_Pro KEY_MODE)
{
	if(KEY_MODE)key_up3=1;  //֧������
	if(key_up3 &&  Key_Up_IN == 0)
	{
		DELAY_MS(10);
		key_up3=0;
		if( Key_Up_IN== 0)
			return 1;
	}else if( Key_Up_IN==1)
			key_up3=1; 	   
 	return 0;// �ް�������

	// u8_Pro key_precess = 0;
	// #if KEY_FUNCTION ==1
	// #warning "����δ�ر༭"			/* !< �༭����ɾ������ */
	// #endif
	// return key_precess;
	
}


/****************************************************************************
* ��������: ���°���״̬ȷ������
* �������� : �ж����°����Ƿ���
* ���� :����ɨ��ģʽ: KEY_MODE_Once  or  KEY_MODE_Continuous
* ��� :����״̬ �Ѱ��� Ϊ�� == 0  or ����  ���� != 0
* ���� :
*****************************************************************************/
u8_Pro Key_Of_Down(u8_Pro KEY_MODE)
{
	if(KEY_MODE)key_up4=1;  //֧������
	if(key_up4 &&  Key_Down_IN == 0)
	{
		DELAY_MS(10);
		key_up4=0;
		if( Key_Down_IN== 0)
			return 1;
	}else if( Key_Down_IN==1)
			key_up4=1; 	   
 	return 0;// �ް�������

	// u8_Pro key_precess = 0;
	// #if KEY_FUNCTION ==1
	// #warning "����δ�ر༭"			/* !< �༭����ɾ������ */
	// #endif
	// return key_precess;
}

