/********************************************************************************


 **** Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : UartBootLowerPackForward.c
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
/* Private Constants ---------------------------------------------------------*/

/* Private macros ------------------------------------------------------------*/

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
