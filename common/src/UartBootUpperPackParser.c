/********************************************************************************


 **** Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : UartBootUpperPackParser.c
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
extern uint8_t AckReg; // 寄存回复状态码的变量
/* Private Constants ---------------------------------------------------------*/

/* Private macros ------------------------------------------------------------*/
/* 更新回复数据包ACK的宏 */
#define UartBootPackSwitchACK(reg, ack) reg = ack
/* Private function prototypes -----------------------------------------------*/
/* 生成回复包的函数 */
uint32_t UartBootPackReply(UART_BOOT_PACK_STRUCT *pack);

/* 不同命令的分支处理 */
static void UB_KIND_FLASH_FAT_Handle(UART_BOOT_PACK_STRUCT *pack);
static void UB_KIND_FLASH_CHEC_Handle(UART_BOOT_PACK_STRUCT *pack);
static void UB_KIND_FLASH_BURN_Handle(UART_BOOT_PACK_STRUCT *pack);
static void UB_KIND_FLASH_RESET_Handle(UART_BOOT_PACK_STRUCT *pack);
/* Private functions ---------------------------------------------------------*/
/**
 * @brief 上位模块信息的包解析
 * 
 * @param pack 传入的完整包信息数据 结构体指针
 * @return uint32_t 标准返回
 */
uint32_t UartUpperPackParser(UART_BOOT_PACK_STRUCT *pack)
{
    UartBootPackSwitchACK(AckReg, YORO_OTA_STATE_ACK); // 运行解析前设置为正确回复包

    if((pack->Msg.bit.bParaID != PackBOOT_DEVICE_PARA_ID && pack->Msg.bit.bParaID != 0) || // 模块ID检查
       (pack->Msg.bit.bCabID != PackBOOT_DEVICE_CAB_ID && pack->Msg.bit.bCabID != 0)       // 机柜ID检查
    )
    {
        return RETURN_DEFAULT; // 非本机指令，丢弃
    }

    switch(pack->Msg.bit.bKind)
    {
    case KIND_FLASH_TYPE1_M:
    case KIND_FLASH_TYPE2_M:
    case KIND_FLASH_TYPE3_M:
    case KIND_FLASH_TYPE4_M:
    case KIND_FLASH_TYPE5_M:
        /* 一个握手包，直接回复ACK即可 */
        break;
    case KIND_FLASH_FAT:
        UB_KIND_FLASH_FAT_Handle(pack);
        break;
    case KIND_FLASH_CHEC:
        UB_KIND_FLASH_CHEC_Handle(pack);
        break;
    case KIND_FLASH_BURN:
        UB_KIND_FLASH_BURN_Handle(pack);
        break;
    case KIND_FLASH_ANS_RESET:
        UB_KIND_FLASH_RESET_Handle(pack);
        break;
    }

    return UartBootPackReply(pack); // 解析结束，包包回复
}

/**
 * @brief 向上位模块发送回复数据，依据pack包中的数据自动组成回复
 * @param pack 传入的数据包
 * @return 标准返回
 */
uint32_t UartBootPackReply(UART_BOOT_PACK_STRUCT *pack)
{
    static uint16_t i;

    /*收件方依据寄件方设定*/
    switch(pack->Msg.bit.bTX_Mes)
    {
    case TX_OBJECT_TYPE1:
        pack->Msg.bit.bRX_Mes = RX_OBJECT_TYPE1;
        break;
    case TX_OBJECT_TYPE2:
        pack->Msg.bit.bRX_Mes = RX_OBJECT_TYPE2;
        break;
    case TX_OBJECT_TYPE3:
        pack->Msg.bit.bRX_Mes = RX_OBJECT_TYPE3;
        break;
    case TX_OBJECT_TYPE4:
        pack->Msg.bit.bRX_Mes = RX_OBJECT_TYPE4;
        break;
    case TX_OBJECT_TYPE5:
        pack->Msg.bit.bRX_Mes = RX_OBJECT_TYPE5;
        break;
    case TX_OBJECT_UMP:
        pack->Msg.bit.bRX_Mes = RX_OBJECT_UMP;
        break;
    default:
        return RETURN_ERROR; // 几乎不可能出现的情况，未知寄件方通不过完整包验证
    }

    /*寄件方依据设置决定*/
    pack->Msg.bit.bTX_Mes = PackBOOT_DEVICE_TYPE_TX;

    /*回复机柜和模组号依据设置决定*/
    pack->Msg.bit.bParaID = PackBOOT_DEVICE_PARA_ID;
    pack->Msg.bit.bCabID  = PackBOOT_DEVICE_CAB_ID;

    /* 调整参数长度指示 */
    pack->DataLen = UART_BOOT_REPLY_DATA_LEN; // 校验内容不属于包长计算

    /* 调整回复状态码 */
    pack->Data[UART_BOOT_REPLY_DATA_ACK] = AckReg;

    /*计算校验*/
    pack->SumNot = (pack->Msg.all >> 24) & 0xFF;
    pack->SumNot += (pack->Msg.all >> 16) & 0xFF;
    pack->SumNot += (pack->Msg.all >> 8) & 0xFF;
    pack->SumNot += (pack->Msg.all >> 0) & 0xFF;
    pack->SumNot += (pack->DataLen >> 8) & 0xFF;
    pack->SumNot += (pack->DataLen >> 0) & 0xFF;
    for(i = 0; i < pack->DataLen; i++)
    {
        pack->SumNot += pack->Data[i];
    }
    pack->Data[UART_BOOT_REPLY_DATA_SUMNOT] = ~pack->SumNot;

    UartBootWriteUpper((uint8_t *)UartPackHead, 2); // 发送头
    return UartBootWriteUpper((uint8_t *)pack, 7 + UART_BOOT_REPLY_DATA_LEN);
}

/**
 * @brief 解析命令KIND_FLASH_FAT 擦除包的分支处理
 * @param pack 传入的数据包
 */
static void UB_KIND_FLASH_FAT_Handle(UART_BOOT_PACK_STRUCT *pack)
{
    McuBootAppStar = (uint32_t)(*(uint32_t *)&pack->Data[0]);
    McuBootAppSize = (uint32_t)(*(uint32_t *)&pack->Data[4]);

    if(McuBootAppStar != MCUBOOT_APP_START_ADD ||               // 检查起始地址范围
       (McuBootAppStar + McuBootAppSize) > MCUBOOT_APP_END_ADD) // 检查结束地址范围
    {
        McuBootAppStar = 0;
        McuBootAppSize = 0;
        UartBootPackSwitchACK(AckReg, YORO_OTA_STATE_ERRADDRESS);
        return;
    }

    if(McuBootFlashErasure(McuBootAppStar, McuBootAppSize) != RETURN_DEFAULT)
    {
        UartBootPackSwitchACK(AckReg, YORO_OTA_STATE_ERRERASE);
        return;
    }
}

/**
 * @brief 解析命令KIND_FLASH_CHEC 烧录信息包的分支处理
 * @param pack 传入的数据包
 */
static void UB_KIND_FLASH_CHEC_Handle(UART_BOOT_PACK_STRUCT *pack)
{
    McuBootDownStar = (uint32_t)(*(uint32_t *)&pack->Data[0]);
    McuBootDownSize = (uint16_t)(*(uint16_t *)&pack->Data[6]);

    if(McuBootDownStar < MCUBOOT_APP_START_ADD ||                 // 检查起始地址范围
       (McuBootDownStar + McuBootDownSize) > MCUBOOT_APP_END_ADD) // 检查结束地址范围
    {
        McuBootDownStar = 0;
        McuBootDownSize = 0;
        UartBootPackSwitchACK(AckReg, YORO_OTA_STATE_ERRADDRESS);
        return;
    }

    if(McuBootDownSize > BOOT_SINGLE_WRITE_SIZE_MAX) // 单次烧录超越长度检查
    {
        UartBootPackSwitchACK(AckReg, YORO_OTA_STATE_ERRADDRESS);
        return;
    }
}

/**
 * @brief 解析命令KIND_FLASH_BURN 烧录包的分支处理
 * @param pack 传入的数据包
 */
static void UB_KIND_FLASH_BURN_Handle(UART_BOOT_PACK_STRUCT *pack)
{
    if(McuBootDownStar == McuBootAppStar) // 第一包检测
    {
        /* 暂存最初4字，完整烧录后再填入 */
        for(int i = 0; i < 32; i++)
            McuBootFirstFour[i] = pack->Data[i];

        /* 剔除最初一个字的其他数据，烧录*/
        if(McuBootFlashWrite(McuBootDownStar + 32, (pack->DataLen - 32), &pack->Data[32]) != RETURN_DEFAULT)
        {
            UartBootPackSwitchACK(AckReg, YORO_OTA_STATE_ERRERASE);
            return;
        }
    }

    if(McuBootDownStar != McuBootAppStar) // 剩余包统一处理
    {
        /* 烧录 */
        if(McuBootFlashWrite(McuBootDownStar, pack->DataLen, &pack->Data[0]) != RETURN_DEFAULT)
        {
            UartBootPackSwitchACK(AckReg, YORO_OTA_STATE_ERRERASE);
            return;
        }
    }

    if((McuBootDownStar + McuBootDownSize) == (McuBootAppStar + McuBootAppSize)) // 最末包检测
    {
        /* 烧录最初的一个字 */
        if(McuBootFlashWrite(McuBootAppStar, 32, McuBootFirstFour) != RETURN_DEFAULT)
        {
            UartBootPackSwitchACK(AckReg, YORO_OTA_STATE_ERRERASE);
            return;
        }
    }
}

/**
 * @brief 解析命令KIND_FLASH_ANS_RESET 重启包的分支处理
 * @param pack 传入的数据包
 */
static void UB_KIND_FLASH_RESET_Handle(UART_BOOT_PACK_STRUCT *pack)
{
    /* 自身收到重启包 ，立即进行重启流程*/
    EventTrigger(&tBootEven, EVENT_MCU_RESET);
    return;
}

#endif
