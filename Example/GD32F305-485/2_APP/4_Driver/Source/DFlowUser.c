/********************************************************************************


 **** Copyright (C), 2020, Shenzhen SKONDA ELECTRONIC LTD  ****


 ********************************************************************************
 * File Name     : DFlowUser.c
 * Author        : SKONDA
 * Date          : 2024-03-13
 * Version       : 1.0
********************************************************************************/
/**************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include <includes.h>
/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
extern uint8_t *U0RxBuffer;
extern size_t   U0RxBufferIndex;
extern uint8_t *U0TxBuffer;
extern size_t   U0TxBufferIndex;
extern size_t   U0TxBufferLen;
/* Private Constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
uint32_t Transmit485(volatile void *Data, size_t Len);
uint32_t Receive485(volatile void *Data, size_t Len);
uint32_t TransmitGetState485(void);
void     SendBefor485(void);
void     SendOver485(void);
/* Private functions ---------------------------------------------------------*/

_DFlow                 DFlow;
_DFLOW_COMMON_FUNCTION DFlowFunc = {.Transmit         = Transmit485, //
                                    .Receive          = Receive485,
                                    .TransmitGetState = TransmitGetState485,
                                    .SendBefor        = SendBefor485,
                                    .SendOver         = SendOver485};

/**
 * @brief 供Dataflow绑定的发送通道
 * 
 */
uint32_t Transmit485(volatile void *Data, size_t Len)
{
    U0TxBuffer    = (uint8_t *)Data;
    U0TxBufferLen = Len;
    usart_interrupt_enable(USART0, USART_INT_TBE);

    return DFLOW_PORT_RETURN_DEFAULT;
}

/**
 * @brief 供Dataflow绑定的接收通道
 * 
 */
uint32_t Receive485(volatile void *Data, size_t Len)
{
    U0RxBuffer      = (uint8_t *)Data;
    U0RxBufferIndex = 0;

    return DFLOW_PORT_RETURN_DEFAULT;
}

/**
 * @brief 供Dataflow绑定的获得发送空闲态，主要DMA使用，非DMA可以直接返回正确值
 * 
 */
uint32_t TransmitGetState485(void)
{
    if(U0TxBufferLen != 0)
        return DFLOW_PORT_RETURNT_ERR_INDEFINITE;

    return DFLOW_PORT_RETURN_DEFAULT;
}

/**
 * @brief 供Usart绑定的回调函数，RS485发送数据前的准备工作,切换485芯片为发送状态
 * 
 */
void SendBefor485(void)
{
    /* 切换RS485收发芯片为发送模式 */
    gpio_bit_write(GPIOB, GPIO_PIN_1, RESET);
}

/**
 * @brief 供Usart绑定的回调函数，RS485发送完毕的处理工作,切换485芯片为接收状态
 * 
 */
void SendOver485(void)
{
    /* 切换RS485收发芯片为接收模式 */
    gpio_bit_write(GPIOB, GPIO_PIN_1, SET);
}
