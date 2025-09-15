/********************************************************************************


 **** Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : UartBoot.c
 * Author        : yono
 * Date          : 2025-08-19
 * Version       : 1.0
********************************************************************************/
/**************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include <YoroOTA.h>

#if BOOT_PORT_UART
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
const uint8_t           UartPackHead[] = {UART_BOOT_HEAD1, UART_BOOT_HEAD2};
BOOT_PARSE_STATE_STRUCT UartParseState;
UART_BOOT_PACK_STRUCT   UartBootPack __attribute__((__aligned__));
/* Private Constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* 对上位模块处理 */
extern uint32_t UartUpperPackVerification(BOOT_PARSE_STATE_STRUCT *state, UART_BOOT_PACK_STRUCT *pack, uint16_t msTime);
extern uint32_t UartUpperPackParser(UART_BOOT_PACK_STRUCT *pack);
/* 对下位模块处理 */
extern uint32_t UartBootLowerPackForward(void);
/* Private functions ---------------------------------------------------------*/

/**
 * @brief 初始化串口boot系统
 * 
 */
void UartBootParsetInit(void)
{
    UartParseState.step = 0;
    UartParseState.time = 0;
    //配置包解析超时时间 毫秒
    UartParseState.timeout = 100;
}

/**
 * @brief 串口boot系统运作，周期调用
 * @param msTime 调用此函数的周期 ms
 */
void UartBootRun(uint16_t msTime)
{
    /* 上位模块包处理 */
    if(UartUpperPackVerification(&UartParseState, &UartBootPack, msTime) == RETURN_DEFAULT) // 验证完整包
        UartUpperPackParser(&UartBootPack);                                                 // 取得完整包，进行解析

    /* 下位模块包处理 */
    UartBootLowerPackForward( ); // 向上转发，不做解析
}

#endif
