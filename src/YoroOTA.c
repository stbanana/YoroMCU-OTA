/********************************************************************************


 **** Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : YoroOTA.c
 * Author        : yono
 * Date          : 2025-08-19
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
int32_t  McuBootDownSize; // 接收到的指示信息包，接下来数据包的总长度
uint32_t McuDownChecksum; // 接收到的指示信息包，接下来数据包的和校验
/* 最初4字，暂存，延迟写入 */
uint8_t McuBootFirstFour[32];
/* 回复通用状态 */
uint8_t AckReg; // 寄存回复状态码的变量

/* 为多包分帧烧录提供的寄存区 */
uint8_t  FlashDownFirstFlag;    // 是否首包的flag 1则没有首包 0则已有首包
uint8_t  FlashDownData[1024];   // 存储多包数据
uint32_t FlashDownDataLen;      // 当前存储多包数据的长度
uint32_t FlashDownDataChecksum; // 当前存储多包数据的和校验
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
#if BOOT_PORT_CAN
    CANBootParsetInit( ); // 初始化CAN boot解析系统
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
#if BOOT_PORT_CAN
    CANBootRun(msTime); // 调用CAN boot系统运作函数
#endif

    EventCheck(&tBootEven, msTime); // 事件处理
}