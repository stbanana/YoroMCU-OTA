/********************************************************************************


 **** Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : CANBootLowerPackForward.c
 * Author        : yono
 * Date          : 2025-08-19
 * Version       : 1.0
********************************************************************************/
/**************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include <YoroOTA.h>

#if BOOT_PORT_CAN
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
uint32_t CANBootLowerPackForward(void)
{
    static uint32_t id;
    static uint8_t  data[8];
    static uint8_t  Len;
    while(CANBootGetcLower(&id, data, &Len) == RETURN_DEFAULT)
        CANBootWriteUpper(id, data, Len);
    return RETURN_DEFAULT;
}

#endif
