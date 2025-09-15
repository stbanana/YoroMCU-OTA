/********************************************************************************


 **** Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : CANStdio.c
 * Author        : yono
 * Date          : 2025-08-19
 * Version       : 1.0
********************************************************************************/
/**************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include <includes.h>
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

can_trasnmit_message_struct CAN1TxMessage;
can_receive_message_struct  receive_message;
_CAN_MSG_QUEUE              CAN1MsgQueue;
/* Private Constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
 * @brief 队空判断
 * @param pQueue 
 * @return 
 */
uint16_t Queue_CanMsgIsEmpty(_CAN_MSG_QUEUE *pQueue)
{
    return pQueue->rear == pQueue->front;
}

/**
 * @brief 队满判断
 * @param pQueue 
 * @return 
 */
uint16_t Queue_CanMsgIsFull(_CAN_MSG_QUEUE *pQueue)
{
    return (pQueue->rear + 1) % CAN_MSG_QUEUE_BUFF_SIZE == pQueue->front;
}

/**
 * @brief 入队
 * @param pQueue 
 * @param val 
 * @return 
 */
uint16_t Queue_CanMsgPush(_CAN_MSG_QUEUE *pQueue, _CAN_MSG_FRAME val)
{
    if(NULL == pQueue)
        return 0;
    //入队之前应该先判断队列是否已经满了
    if(Queue_CanMsgIsFull(pQueue))
    {
        return 0;
    }
    //将元素放入队列，这里也说明front是可取的
    pQueue->elem[pQueue->rear] = val;
    //更新队尾位置
    pQueue->rear = (pQueue->rear + 1) % CAN_MSG_QUEUE_BUFF_SIZE;
    return 1;
}

/**
 * @brief 出队
 * @param pQueue 
 * @param val 
 * @return 
 */
uint16_t Queue_CanMsgPop(_CAN_MSG_QUEUE *pQueue, _CAN_MSG_FRAME *pVal)
{
    if(NULL == pQueue)
        return 0;
    //出队之前应该先判断队列是否是空的
    if(Queue_CanMsgIsEmpty(pQueue))
    {
        return 0;
    }
    *pVal         = pQueue->elem[pQueue->front];                   //取出队头元素
    pQueue->front = (pQueue->front + 1) % CAN_MSG_QUEUE_BUFF_SIZE; //更新队头位置
    return 1;
}

/* 供CAN1读取的UART1中断 */
void CAN1_RX0_IRQHandler(void)
{
    static _CAN_MSG_FRAME rxMsg;
    /* check the receive message */
    can_message_receive(CAN1, CAN_FIFO0, &receive_message);

    if((CAN_FF_STANDARD == receive_message.rx_ff)) // 标准帧
    {
        rxMsg.ID     = receive_message.rx_sfid;
        rxMsg.MsgLen = receive_message.rx_dlen;
        memcpy(rxMsg.Data, receive_message.rx_data, receive_message.rx_dlen);
    }
    else if((CAN_FF_EXTENDED == receive_message.rx_ff)) // 拓展帧
    {
        rxMsg.ID     = receive_message.rx_efid;
        rxMsg.MsgLen = receive_message.rx_dlen;
        memcpy(rxMsg.Data, receive_message.rx_data, receive_message.rx_dlen);
    }
    Queue_CanMsgPush(&CAN1MsgQueue, rxMsg);
}
