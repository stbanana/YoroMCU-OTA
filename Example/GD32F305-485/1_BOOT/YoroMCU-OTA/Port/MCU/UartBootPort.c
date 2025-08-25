/********************************************************************************


 **** Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : UartBootPort.c
 * Author        : yono
 * Date          : 2025-08-19
 * Version       : 1.0
********************************************************************************/
/**************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include <YoroOTA.h>
#include <Includes.h>
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern _DFlow DFlow;
/* Private Constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

#if BOOT_PORT_UART

/**
 * @brief 向上位模块发送数据的物理通信接口函数
 * @param pcBuf 将要发送的数据地址头
 * @param ui32Len 将要发送的数据长度
 * @return uint32_t 标准返回
 */
uint32_t UartBootWriteUpper(uint8_t *pcBuf, uint32_t ui32Len)
{
    /* 平台特有发送函数(需填)*/
    if(DFlow_Write(&DFlow, pcBuf, ui32Len) != DFLOW_API_RETURN_DEFAULT)
        return RETURN_ERROR;

    return RETURN_DEFAULT;
}

/**
 * @brief 向上位模块接收一个数据的物理通信接口函数
 * @param data_rx 将要赋值的变量地址
 * @return uint32_t 标准返回
 */
uint32_t UartBootGetcUpper(uint8_t *data_rx)
{
    /* 平台特有接收函数(需填)*/
    if(DFlow_Getc(&DFlow, data_rx) != DFLOW_API_RETURN_DEFAULT)
        return RETURN_ERROR;

    return RETURN_DEFAULT;
}

/**
 * @brief 向下位模块发送数据的物理通信接口函数
 * @param pcBuf 将要发送的数据地址头
 * @param ui32Len 将要发送的数据长度
 * @return uint32_t 标准返回
 */
uint32_t UartBootWriteLower(uint8_t *pcBuf, uint32_t ui32Len)
{
    /* 平台特有发送函数(需填)，如无下位模块，则可留空 */
    // if(UartWrite(&tUart1, pcBuf, ui32Len) == false)
    //     return RETURN_ERROR;

    return RETURN_DEFAULT;
}

/**
 * @brief 向下位模块接收一个数据的物理通信接口函数
 * @param data_rx 将要赋值的变量地址
 * @return uint32_t 标准返回
 */
uint32_t UartBootGetcLower(uint8_t *data_rx)
{
    /* 平台特有接收函数(需填)，
        如无下位模块，则可留空，并常返回默认错误 */
    // if(UartGetc(&tUart1, data_rx)== false)
    //     return RETURN_ERROR;

    // return RETURN_DEFAULT;
    return RETURN_ERROR;
}

#endif