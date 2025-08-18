/********************************************************************************


 **** Copyright (C), 2020, Shenzhen SKONDA ELECTRONIC LTD  ****


 ********************************************************************************
 * File Name     : UartBootUpperPackComlete.c
 * Author        : SKONDA
 * Date          : 2024-03-14
 * Version       : 1.0
********************************************************************************/
/**************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include <YoroOTA.h>

#if BOOT_PORT_UART
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private Constants ---------------------------------------------------------*/

/* Private macros ------------------------------------------------------------*/
/*      完整帧收取流程定义      */
/* 收取帧头 */
#define UART_BOOT_PACKET_HEAD1 0x00
#define UART_BOOT_PACKET_HEAD2 0x10
/* 收取命令指示 */
#define UART_BOOT_PACKET_MSG1 0x20
#define UART_BOOT_PACKET_MSG2 0x21
#define UART_BOOT_PACKET_MSG3 0x22
#define UART_BOOT_PACKET_MSG4 0x23
/* 收取长度指示 */
#define UART_BOOT_PACKET_LEN1 0x30
#define UART_BOOT_PACKET_LEN2 0x31
/* 收取数据 */
#define UART_BOOT_PACKET_DATA 0x40
/* 转发数据 */
#define UART_BOOT_PACKET_FRW 0x50
/* 收取帧尾校验数据 */
#define UART_BOOT_PACKET_TAIL 0x60

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
/**
 * @brief 下位模块向上的转发，本机在bootload中，不对下位模块数据进行解析
 * @return uint32_t 标准返回
 */
uint32_t UartBootLowerPackForward(void)
{
    static uint8_t LowerData;
    while(UartBootGetcLower(&LowerData) == RETURN_DEFAULT)
        UartBootWriteUpper(&LowerData, 1);
    return RETURN_DEFAULT;
}

#endif
