/**
 * CANStdio.h
 *
 *  Created on: 2023年6月29日
 *      Author: yono
 *
*/
/**
 *
 * CAN功能
*/

#ifndef _CAN_STDIO_H_
#define _CAN_STDIO_H_

/* Includes ------------------------------------------------------------------*/
/* User constants --------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/

#ifndef CAN_MSG_QUEUE_BUFF_SIZE
#define CAN_MSG_QUEUE_BUFF_SIZE 64
#endif

/* Exported types ------------------------------------------------------------*/

typedef struct
{
    uint32_t ID;      // ID
    uint16_t MsgLen;  // 数据长度
    uint8_t  Data[8]; // 数据
} _CAN_MSG_FRAME;

typedef struct
{
    _CAN_MSG_FRAME elem[CAN_MSG_QUEUE_BUFF_SIZE];
    uint16_t       front; //队头，出队
    uint16_t       rear;  //队尾，入队
} _CAN_MSG_QUEUE;

/* Exported variables ---------------------------------------------------------*/
extern can_trasnmit_message_struct CAN1TxMessage;
extern _CAN_MSG_QUEUE              CAN1MsgQueue;
/* Exported functions ---------------------------------------------------------*/
extern uint16_t Queue_CanMsgIsEmpty(_CAN_MSG_QUEUE *pQueue);
extern uint16_t Queue_CanMsgIsFull(_CAN_MSG_QUEUE *pQueue);
extern uint16_t Queue_CanMsgPush(_CAN_MSG_QUEUE *pQueue, _CAN_MSG_FRAME val);
extern uint16_t Queue_CanMsgPop(_CAN_MSG_QUEUE *pQueue, _CAN_MSG_FRAME *pVal);

#endif /* _CAN_STDIO_H_ */
