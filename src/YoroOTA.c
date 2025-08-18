/********************************************************************************


 **** Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : YoroOTA.c
 * Author        : yono
 * Date          : 2025-08-18
 * Version       : 1.0
********************************************************************************/
/**************************************************************************/
/*
    对外核心API
*/

/* Includes ------------------------------------------------------------------*/
#include <YoroOTA.h>
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* 指示信息 */
uint32_t McuBootAppStar;  // 接收到的指示烧录信息，整个烧录的起始地址
uint32_t McuBootAppSize;  // 接收到的指示烧录信息，整个烧录的总长度
uint32_t McuBootDownStar; // 接收到指示信息包，接下来数据包的起始地址
uint32_t McuBootDownSize; // 接收到的指示信息包，接下来数据包的总长度
/* 最初4字节是起始地址，暂存，延迟写入 */
uint8_t McuBootFirstFour[4];
/* 回复通用状态 */
uint8_t AckReg; // 寄存回复状态码的变量
/* Private Constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief 初始化YoroOTA系统
 * 
 */
void YoroOTAInit(void)
{
#if BOOT_PORT_UART
    UartBootParsetInit( ); // 初始化串口boot解析系统
#endif
}

/**
 * @brief YoroOTA系统运作，周期调用
 * @param msTime 调用此函数的周期 ms
 */
void YoroOTARun(uint16_t msTime)
{
#if BOOT_PORT_UART
    UartBootRun(msTime); // 调用串口boot系统运作函数
#endif

    EventCheck(&tBootEven, msTime); // 事件处理
}