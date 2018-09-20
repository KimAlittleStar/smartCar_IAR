/****************************************************************************
* 文件名称 : _NVIC.h
* 文件功能 : 将文件中所有的中断配置。中断服务函数集中在这里编写
* 注意事项 : 注意中断分组，每次进入判断后应该及时清除中断标志位
*****************************************************************************/

#include "_NVIC.h"

void NVIC_SET_init(void)
{
    NVIC_SetPriorityGrouping(4);
    set_irq_priority(PORTC_IRQn,1);/*!< 摄像头读取数据函数 */
    set_irq_priority(DMA1_IRQn,0);/*!< 摄像头读取完毕中断 */

    set_irq_priority(PIT0_IRQn,2);/*!< 编码器中断 */
    set_irq_priority(FTM2_CH0_PIN,3);
    set_irq_priority(PIT1_IRQn,4);/*!< PID中断 */
    set_irq_priority(UART3_RX_TX_IRQn,5);/*!< wifi串口中断 */
    set_irq_priority(PIT3_IRQn,7);
    set_irq_priority(PIT2_IRQn,9);
    /*!< 串口中断 */
    set_irq_priority(UART1_RX_TX_IRQn,15);
    EnableInterrupts;
}

