/**
 * YoroOTA_CAN.h
 *
 *  Created on: 2025年8月19日
 *      Author: yono
 *
*/
/**
 *
 * 使用CAN接口的升级协议流
*/

#ifndef _YORO_OTA_CAN_H_
#define _YORO_OTA_CAN_H_
#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/

#if BOOT_PORT_CAN
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/

/*      回复数据的字节定义      */
/* 回复参数实体的总长度 */
#define CAN_BOOT_REPLY_DATA_LEN 8
/* 回复参数保留接收参数的字节 */
#define CAN_BOOT_REPLY_DATA_COPY1 0
#define CAN_BOOT_REPLY_DATA_COPY2 1
/* 回复参数中ACK数据的字节 */
#define CAN_BOOT_REPLY_DATA_ACK 3

/* Exported types ------------------------------------------------------------*/
typedef struct
{
    union BOOT_PARSE_MSG Msg;     /* 烧录指令 */
    uint8_t              Data[8]; /* 数据实体，表示比最大烧录长度 留有10字节裕量 */
    uint8_t              DataLen; /* 数据长度 */
} CAN_BOOT_PACK_STRUCT;
/* Exported variables ---------------------------------------------------------*/

/* Exported functions ---------------------------------------------------------*/
extern void CANBootParsetInit(void);
extern void CANBootRun(uint16_t msTime);

#endif

#ifdef __cplusplus
}
#endif
#endif /* _YORO_OTA_CAN_H_ */
