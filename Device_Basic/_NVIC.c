/****************************************************************************
* �ļ����� : _NVIC.h
* �ļ����� : ���ļ������е��ж����á��жϷ����������������д
* ע������ : ע���жϷ��飬ÿ�ν����жϺ�Ӧ�ü�ʱ����жϱ�־λ
*****************************************************************************/

#include "_NVIC.h"

void NVIC_SET_init(void)
{
    NVIC_SetPriorityGrouping(4);
    set_irq_priority(PORTC_IRQn,1);/*!< ����ͷ��ȡ���ݺ��� */
    set_irq_priority(DMA1_IRQn,0);/*!< ����ͷ��ȡ����ж� */

    set_irq_priority(PIT0_IRQn,2);/*!< �������ж� */
    set_irq_priority(FTM2_CH0_PIN,3);
    set_irq_priority(PIT1_IRQn,4);/*!< PID�ж� */
    set_irq_priority(UART3_RX_TX_IRQn,5);/*!< wifi�����ж� */
    set_irq_priority(PIT3_IRQn,7);
    set_irq_priority(PIT2_IRQn,9);
    /*!< �����ж� */
    set_irq_priority(UART1_RX_TX_IRQn,15);
    EnableInterrupts;
}

