/**
 * YoroOTA_Uart.h
 *
 *  Created on: 2024年3月14日
 *      Author: skonda15
 *
*/
/**
 *
 * 使用串口的升级协议流
*/

#ifndef _UART_BOOT_H_
#define _UART_BOOT_H_

/* Includes ------------------------------------------------------------------*/

#include "BootPort.h"
#include "ReturnCodesC.h"

#if BOOT_PORT_UART
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/*      帧头定义    */
#define UART_BOOT_HEAD1 0xFE
#define UART_BOOT_HEAD2 0xEF

/*      回复数据的字节定义      */
/* 回复参数实体的总长度 */
#define UART_BOOT_REPLY_DATA_LEN 8
/* 回复参数保留接收参数的字节 */
#define UART_BOOT_REPLY_DATA_COPY1 0
#define UART_BOOT_REPLY_DATA_COPY2 1
/* 回复参数中ACK数据的字节 */
#define UART_BOOT_REPLY_DATA_ACK 3
/* 回复参数中和校验取反数据的字节，校验字节参数内容后一个字节 */
#define UART_BOOT_REPLY_DATA_SUMNOT (UART_BOOT_REPLY_DATA_LEN + 1)

/* Exported types ------------------------------------------------------------*/
typedef struct
{
    union BOOT_PARSE_MSG Msg;                                   /* 烧录指令 */
    uint16_t             DataLen;                               /* 数据长度 */
    uint8_t              Data[BOOT_SINGLE_WRITE_SIZE_MAX + 10]; /* 数据实体，表示比最大烧录长度 留有10字节裕量 */
    uint16_t             DataBufferNum;                         /* 数据写入数据指示 */
    uint8_t              SumNot;                                /* 和校验取反 */
} UART_BOOT_PACK_STRUCT;
/* Exported variables ---------------------------------------------------------*/
extern const uint8_t           UartPackHead[];

extern BOOT_PARSE_STATE_STRUCT UartParseState;

/* Exported functions ---------------------------------------------------------*/
extern void UartBootParsetInit(void);
extern void UartBootRun(uint16_t msTime);

#endif

#endif /* _UART_BOOT_H_ */
