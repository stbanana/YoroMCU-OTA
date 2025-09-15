/********************************************************************************


 **** Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : DFlowUser.c
 * Author        : yono
 * Date          : 2025-08-19
 * Version       : 1.0
********************************************************************************/
/**************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include <includes.h>
/* Private types -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
extern uint8_t *U2RxBuffer;
extern size_t   U2RxBufferIndex;
extern uint8_t *U2TxBuffer;
extern size_t   U2TxBufferIndex;
extern size_t   U2TxBufferLen;

// 建议4字节对齐，否则 DMA 可能存在问题
#define T_LEN_MAX 256
#define R_LEN_MAX 256
/* 预分配内存区 */
uint8_t MEM[T_LEN_MAX * 2 + R_LEN_MAX * 2];
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
 * @brief 用户使用DataFlow初始化函数
 */
void DFlow_User_Init(void)
{
    DFlow_Init(&DFlow, MEM, T_LEN_MAX, R_LEN_MAX, &DFlowFunc);
}

/**
 * @brief 供Dataflow绑定的发送通道
 * 
 */
uint32_t Transmit485(volatile void *Data, size_t Len)
{
    U2TxBuffer    = (uint8_t *)Data;
    U2TxBufferLen = Len;
    usart_interrupt_enable(USART2, USART_INT_TBE);

    return DFLOW_PORT_RETURN_DEFAULT;
}

/**
 * @brief 供Dataflow绑定的接收通道
 * 
 */
uint32_t Receive485(volatile void *Data, size_t Len)
{
    U2RxBuffer      = (uint8_t *)Data;
    U2RxBufferIndex = 0;

    return DFLOW_PORT_RETURN_DEFAULT;
}

/**
 * @brief 供Dataflow绑定的获得发送空闲态，主要DMA使用，非DMA可以直接返回正确值
 * 
 */
uint32_t TransmitGetState485(void)
{
    if(U2TxBufferLen != 0)
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
    gpio_bit_write(GPIOB, GPIO_PIN_1, SET);
}

/**
 * @brief 供Usart绑定的回调函数，RS485发送完毕的处理工作,切换485芯片为接收状态
 * 
 */
void SendOver485(void)
{
    /* 切换RS485收发芯片为接收模式 */
    gpio_bit_write(GPIOB, GPIO_PIN_1, RESET);
}
