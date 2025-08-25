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
uint8_t      *U0RxBuffer      = NULL; // Uart0接收缓冲区
size_t        U0RxBufferIndex = 0;    // Uart0接收缓冲区长度(已接收)
uint8_t      *U0TxBuffer      = NULL; // Uart0发送
size_t        U0TxBufferIndex = 0;    // 已发送长度
size_t        U0TxBufferLen   = 0;    // Uart0发送缓冲区长度

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
    if(RESET != usart_flag_get(usart_periph, USART_FLAG_ORERR)) // 串口接收溢出
    {
        //清除串口接收溢出中断，必须清除，否则可能导致死机，注意STM32和GD32清除该标志的方法不一样
        usart_flag_clear(usart_periph, USART_FLAG_ORERR);
    }
    else if(RESET != usart_interrupt_flag_get(usart_periph, USART_INT_FLAG_IDLE)) // 空闲中断
    {
        DFlow_Interrupt_IDLE_RC_FTF(&DFlow, U0RxBufferIndex);
        U0RxBufferIndex = 0;
        usart_interrupt_flag_clear(usart_periph, USART_INT_FLAG_IDLE);
    }
    else if(RESET != usart_interrupt_flag_get(usart_periph, USART_INT_FLAG_RBNE)) // 接收中断
    {
        uint8_t _byte                 = usart_data_receive(usart_periph); // 接收数据
        U0RxBuffer[U0RxBufferIndex++] = _byte;
    }
    else if(RESET != usart_interrupt_flag_get(usart_periph, USART_INT_FLAG_TBE)) // 发送中断
    {
        usart_data_transmit(usart_periph, U0TxBuffer[U0TxBufferIndex++]);
        if(U0TxBufferIndex >= U0TxBufferLen)
        {                                                         // 发送完毕
            usart_interrupt_disable(usart_periph, USART_INT_TBE); // 关闭发送中断
            U0TxBufferIndex = 0;
            U0TxBufferLen   = 0;
            DFlow_Interrupt_TC(&DFlow);
        }
    }
}

/* 供uart0的UART0中断 */
/*!
    \brief      this function handles USART RBNE interrupt request and TBE interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
void USART0_IRQHandler(void)
{
    UART_IRQHandler(USART0);
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