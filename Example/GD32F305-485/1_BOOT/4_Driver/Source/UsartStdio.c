/********************************************************************************


 **** Copyright (C), 2025, yono<yono233@outlook.com>  ****


 ********************************************************************************
 * File Name     : UsartStdio.c
 * Author        : yono
 * Date          : 2024-03-13
 * Version       : 1.0
********************************************************************************/
/**************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include <includes.h>
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* 一个简单中间层，模仿平台自己写的驱动 */
uint8_t      *U2RxBuffer      = NULL; // Uart2接收缓冲区
size_t        U2RxBufferIndex = 0;    // Uart2接收缓冲区长度(已接收)
uint8_t      *U2TxBuffer      = NULL; // Uart2发送
size_t        U2TxBufferIndex = 0;    // 已发送长度
size_t        U2TxBufferLen   = 0;    // Uart2发送缓冲区长度
int8_t        U2IDLE_Delay    = 0;    // 手动监测空闲

extern _DFlow DFlow;

/* Private Constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
 * @brief UART中断，综合调用
 *
 * @param pd Uart通道结构体
 */
void UART_IRQHandler(uint32_t usart_periph)
{
    if(RESET != usart_interrupt_flag_get(usart_periph, USART_INT_FLAG_IDLE)) // 空闲中断（纯寄存器中断应该不会触发）
    {
        DFlow_Interrupt_IDLE_RC_FTF(&DFlow, U2RxBufferIndex);
        U2RxBufferIndex = 0;
        usart_interrupt_flag_clear(usart_periph, USART_INT_FLAG_IDLE);
    }
    else if(RESET != usart_interrupt_flag_get(usart_periph, USART_INT_FLAG_RBNE)) // 接收中断
    {
        uint8_t _byte                 = usart_data_receive(usart_periph); // 接收数据
        U2RxBuffer[U2RxBufferIndex++] = _byte;
        U2IDLE_Delay                  = 5;
    }
    else if(RESET != usart_interrupt_flag_get(usart_periph, USART_INT_FLAG_TBE)) // 发送中断
    {
        usart_data_transmit(usart_periph, U2TxBuffer[U2TxBufferIndex++]);
        if(U2TxBufferIndex >= U2TxBufferLen)
        {                                                         // 发送完毕
            usart_interrupt_disable(usart_periph, USART_INT_TBE); // 关闭发送中断
            U2TxBufferIndex = 0;
            U2TxBufferLen   = 0;
            usart_interrupt_enable(usart_periph, USART_INT_TC);
        }
    }
    else if(RESET != usart_interrupt_flag_get(usart_periph, USART_INT_FLAG_TC)) // 发送中断
    {
        DFlow_Interrupt_TC(&DFlow);
        usart_interrupt_flag_clear(usart_periph, USART_INT_FLAG_TC);
    }
    else if(RESET != usart_interrupt_flag_get(usart_periph, USART_INT_FLAG_RBNE_ORERR)) // 串口接收溢出
    {
        //清除串口接收溢出中断，必须清除，否则可能导致死机，注意STM32和GD32清除该标志的方法不一样
        usart_interrupt_flag_clear(usart_periph, USART_INT_FLAG_RBNE_ORERR);
    }
}

/* 供uart2的UART2中断 */
/*!
    \brief      this function handles USART RBNE interrupt request and TBE interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
void USART2_IRQHandler(void)
{
    UART_IRQHandler(USART2);
}