/********************************************************************************


 **** Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : CANBoot.c
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
CAN_BOOT_PACK_STRUCT CANBootPack __attribute__((__aligned__));
/* Private Constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* 对上位模块处理 */
extern uint32_t CANUpperPackParser(CAN_BOOT_PACK_STRUCT *pack);
/* 对下位模块处理 */
extern uint32_t CANBootLowerPackForward(void);
/* Private functions ---------------------------------------------------------*/

/**
 * @brief 初始化CAN boot系统
 * 
 */
void CANBootParsetInit(void)
{
    /* CAN总线自管理和验证，暂无额外机制初始化 */
}

/**
 * @brief CAN boot系统运作，周期调用
 * @param msTime 调用此函数的周期 ms
 */
void CANBootRun(uint16_t msTime)
{
    /* 上位模块包处理 */
    CANUpperPackParser(&CANBootPack);

    /* 下位模块包处理 */
    CANBootLowerPackForward( ); // 向上转发，不做解析
}

#endif
