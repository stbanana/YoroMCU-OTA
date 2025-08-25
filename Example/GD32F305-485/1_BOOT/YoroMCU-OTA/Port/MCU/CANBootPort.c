/********************************************************************************


 **** Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : CANBootPort.c
 * Author        : yono
 * Date          : 2025-08-19
 * Version       : 1.0
********************************************************************************/
/**************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include <YoroOTA.h>
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private Constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

#if BOOT_PORT_CAN

/**
 * @brief 向上位模块发送数据的物理通信接口函数
 * @param id 将要发送的帧ID
 * @param pcBuf 将要发送的数据地址头
 * @param ui8Len 将要发送的数据长度
 * @return uint32_t 标准返回
 */
uint32_t CANBootWriteUpper(uint32_t id, uint8_t *pcBuf, uint8_t ui8Len)
{
    /* 平台特有发送函数(需填)*/
    // FDCAN_TxHeaderTypeDef g_Can1TxHeader;
    // g_Can1TxHeader.Identifier          = id;                                         // 初始化发送帧ID
    // g_Can1TxHeader.IdType              = FDCAN_EXTENDED_ID;                          // 配置为拓展帧
    // g_Can1TxHeader.TxFrameType         = FDCAN_DATA_FRAME;                           // 配置为数据帧
    // g_Can1TxHeader.DataLength          = (uint32_t)ui8Len << 16;                     // 配置发送数据长度
    // g_Can1TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;                           // 设置错误状态指示(供接收端了解发送端状态)
    // g_Can1TxHeader.BitRateSwitch       = FDCAN_BRS_OFF;                              // 不可变bit率
    // g_Can1TxHeader.FDFormat            = FDCAN_CLASSIC_CAN;                          // 经典can
    // g_Can1TxHeader.TxEventFifoControl  = FDCAN_NO_TX_EVENTS;                         // 用于发送事件FIFO控制, 不存储
    // g_Can1TxHeader.MessageMarker       = 0;                                          // 用于复制到TX EVENT FIFO的消息Maker来识别消息状态，范围0到0xFF
    // if(HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &g_Can1TxHeader, pcBuf) == HAL_ERROR) // 推入CAN队列并检查错误
    //     return RETURN_ERROR;

    return RETURN_DEFAULT;
}

/**
 * @brief 向上位模块接收一个数据的物理通信接口函数
 * @param id 接收到的帧ID
 * @param data_rx 将要赋值的变量地址
 * @param ui8Len 接收到的帧数据长度
 * @return uint32_t 标准返回
 */
uint32_t CANBootGetcUpper(uint32_t *id, uint8_t *data_rx, uint8_t *ui8Len)
{
    /* 平台特有接收函数(需填)*/
    // if(Queue_CanMsgPop(&RxQueue1, id, data_rx, ui8Len) == RETURN_ERROR) // 尝试从接收队列取出数据
    //     return RETURN_ERROR;

    return RETURN_DEFAULT;
}

/**
 * @brief 向下位模块发送数据的物理通信接口函数
 * @param id 将要发送的帧ID
 * @param pcBuf 将要发送的数据地址头
 * @param ui8Len 将要发送的数据长度
 * @return uint32_t 标准返回
 */
uint32_t CANBootWriteLower(uint32_t id, uint8_t *pcBuf, uint8_t ui8Len)
{
    /* 平台特有发送函数(需填)，如无下位模块，则可留空 */
    // FDCAN_TxHeaderTypeDef g_Can2TxHeader;
    // g_Can2TxHeader.Identifier          = id;                                         // 初始化发送帧ID
    // g_Can2TxHeader.IdType              = FDCAN_EXTENDED_ID;                          // 配置为拓展帧
    // g_Can2TxHeader.TxFrameType         = FDCAN_DATA_FRAME;                           // 配置为数据帧
    // g_Can2TxHeader.DataLength          = (uint32_t)ui8Len << 16;                     // 配置发送数据长度
    // g_Can2TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;                           // 设置错误状态指示(供接收端了解发送端状态)
    // g_Can2TxHeader.BitRateSwitch       = FDCAN_BRS_OFF;                              // 不可变bit率
    // g_Can2TxHeader.FDFormat            = FDCAN_CLASSIC_CAN;                          // 经典can
    // g_Can2TxHeader.TxEventFifoControl  = FDCAN_NO_TX_EVENTS;                         // 用于发送事件FIFO控制, 不存储
    // g_Can2TxHeader.MessageMarker       = 0;                                          // 用于复制到TX EVENT FIFO的消息Maker来识别消息状态，范围0到0xFF
    // if(HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan2, &g_Can2TxHeader, pcBuf) == HAL_ERROR) // 推入CAN队列并检查错误
    //     return RETURN_ERROR;

    return RETURN_DEFAULT;
}

/**
 * @brief 向下位模块接收一个数据的物理通信接口函数
 * @param id 接收到的帧ID
 * @param data_rx 将要赋值的变量地址
 * @param ui8Len 接收到的帧数据长度
 * @return uint32_t 标准返回
 */
uint32_t CANBootGetcLower(uint32_t *id, uint8_t *data_rx, uint8_t *ui8Len)
{
    /* 平台特有接收函数(需填)，
        如无下位模块，则可留空，并常返回默认错误 */
    // if(Queue_CanMsgPop(&RxQueue2, id, data_rx, ui8Len) == RETURN_ERROR) // 尝试从接收队列取出数据
    //     return RETURN_ERROR;
    // return RETURN_DEFAULT;

    return RETURN_ERROR;
}

#endif