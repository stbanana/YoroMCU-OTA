/**
 * YoroOTA_Def.h
 *
 *  Created on: 2025年8月19日
 *      Author: yono
 *
*/
/**
 *
 * boot升级模组共用定义
*/

#ifndef _YORO_OTA_DEF_H_
#define _YORO_OTA_DEF_H_
#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
// #include "stdbool.h"
#include "../ReturnCodesC/ReturnCodesC.h"

/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/

// bRX_Mes(6bits):收件方
#define RX_OBJECT_TYPE1 0x01 // 被烧录模块类型1
#define RX_OBJECT_TYPE2 0x02 // 被烧录模块类型2
#define RX_OBJECT_TYPE3 0x04 // 被烧录模块类型3
#define RX_OBJECT_TYPE4 0x08 // 被烧录模块类型4
#define RX_OBJECT_TYPE5 0x10 // 被烧录模块类型5
#define RX_OBJECT_UMP   0x20 // 烧录上位机

// bKind(8bits):数据类型
#define KIND_FLASH_TYPE1_M   0xFA //烧录命令:命令模块类型1烧录主机
#define KIND_FLASH_TYPE2_M   0xFB //烧录命令:命令模块类型2烧录主机
#define KIND_FLASH_TYPE3_M   0xFC //烧录命令:命令模块类型3烧录主机
#define KIND_FLASH_TYPE4_M   0xFD //烧录命令:命令模块类型4烧录主机
#define KIND_FLASH_TYPE5_M   0xFF //烧录命令:命令模块类型5烧录主机

#define KIND_FLASH_INFO      0xF3 //烧录命令:统计烧录台数命令
#define KIND_FLASH_FAT       0xF4 //烧录命令:擦除数据命令
#define KIND_FLASH_BURN      0xF5 //烧录命令:烧录数据命令
#define KIND_FLASH_ANS_INFO  0xF6 //烧录命令:应答统计烧录台数命令
#define KIND_FLASH_CHEC      0xF7 //烧录命令:烧录的地址数据长度数据校验和信息
#define KIND_FLASH_ANS_RESET 0xF8 //烧录命令:应答烧录结束重启芯片，跳转APP

// bTX_Mes(3Bits):寄件方
#define TX_OBJECT_TYPE1 0x00 // 被烧录模块类型1
#define TX_OBJECT_TYPE2 0x01 // 被烧录模块类型2
#define TX_OBJECT_TYPE3 0x02 // 被烧录模块类型3
#define TX_OBJECT_TYPE4 0x03 // 被烧录模块类型4
#define TX_OBJECT_TYPE5 0x04 // 被烧录模块类型5
#define TX_OBJECT_UMP   0x05 // 烧录上位机

// 回复指令状态
#define YORO_OTA_STATE_ACK         0x06 // 命令执行成功
#define YORO_OTA_STATE_ERRERASE    0x60 // 擦除失败，致命ERR上位机报警
#define YORO_OTA_STATE_ERRPROGRAM  0x61 // 写入失败，从擦除开始
#define YORO_OTA_STATE_ERRADDRESS  0x62 // 地址错误
#define YORO_OTA_STATE_ERRCHECKSUM 0x63 // 校验和错误
#define YORO_OTA_STATE_NAK         0x15 // 2s没有接收到数据的在线上报

/* Exported types ------------------------------------------------------------*/
/* bootload通用位定义，这个32位数据应该塞在各升级协议中进行定义和解析 */
typedef struct
{
    uint16_t bParaID:6; //BIT0-BIT5,模块ID(0-63)
    uint16_t bCabID :4; //BIT6-BIT9,机柜ID(0-15)
    uint16_t bRX_Mes:6; //BIT10-BIT15,收件方信息
    uint16_t bKind  :8; //BIT16-BIT23,Command/Event的Kind
    uint16_t bTX_Mes:3; //BIT24-BIT26,寄件人信息
    uint16_t bBITs  :5; //BIT27-BIT31无效
} BOOT_PARSE_MSG_BIT;

union BOOT_PARSE_MSG
{
    uint32_t           all;
    BOOT_PARSE_MSG_BIT bit;
};

/* 判包完整性解析状态结构体 */
typedef struct
{
    uint8_t  step;    /* 当前解析步骤 */
    uint16_t time;    /* 解析函数调用时间周期，毫秒 */
    uint16_t timeout; /* 包解析，超时时间，毫秒 */
} BOOT_PARSE_STATE_STRUCT;
/* Exported variables ---------------------------------------------------------*/
/* 指示信息 */
extern uint32_t McuBootAppStar;  // 接收到的指示烧录信息，整个烧录的起始地址
extern uint32_t McuBootAppSize;  // 接收到的指示烧录信息，整个烧录的总长度
extern uint32_t McuBootDownStar; // 接收到指示信息包，接下来数据包的起始地址
extern int32_t  McuBootDownSize; // 接收到指示信息包，接下来数据包的总长度
extern uint32_t McuDownChecksum; // 接收到指示信息包，接下来数据包的和校验
/* 最初4字节是起始地址，暂存，延迟写入 */
extern uint8_t McuBootFirstFour[64];

/* 为多包分帧烧录提供的寄存区 */
extern uint8_t  FlashDownFirstFlag;    // 是否首包的flag 1则没有首包 0则已有首包
extern uint8_t  FlashDownData[1024];   // 存储多包数据
extern uint32_t FlashDownDataLen;      // 当前存储多包数据的长度
extern uint32_t FlashDownDataChecksum; // 当前存储多包数据的和校验
/* Exported functions ---------------------------------------------------------*/

#ifdef __cplusplus
}
#endif
#endif /* _YORO_OTA_DEF_H_ */
