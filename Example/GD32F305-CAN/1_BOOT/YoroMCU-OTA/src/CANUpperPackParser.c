/********************************************************************************


 **** Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : CANUpperPackParser.c
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
extern uint8_t AckReg; // 寄存回复状态码的变量
/* Private Constants ---------------------------------------------------------*/

/* Private macros ------------------------------------------------------------*/
/* 更新回复数据包ACK的宏 */
#define CANBootPackSwitchACK(reg, ack) reg = ack
/* Private function prototypes -----------------------------------------------*/
/* 生成回复包的函数 */
uint32_t CANBootPackReply(CAN_BOOT_PACK_STRUCT *pack);

/* 不同命令的分支处理 */
static void UB_KIND_FLASH_FAT_Handle(CAN_BOOT_PACK_STRUCT *pack);
static void UB_KIND_FLASH_CHEC_Handle(CAN_BOOT_PACK_STRUCT *pack);
static void UB_KIND_FLASH_BURN_Handle(CAN_BOOT_PACK_STRUCT *pack);
static void UB_KIND_FLASH_RESET_Handle(CAN_BOOT_PACK_STRUCT *pack);
/* Private functions ---------------------------------------------------------*/
/**
 * @brief 上位模块信息的包解析
 * 
 * @param pack 传入的完整包信息数据 结构体指针
 * @return uint32_t 标准返回
 */
uint32_t CANUpperPackParser(CAN_BOOT_PACK_STRUCT *pack)
{
    while(CANBootGetcUpper(&pack->Msg.all, pack->Data, &pack->DataLen) == RETURN_DEFAULT)
    {
        if((pack->Msg.bit.bParaID != PackBOOT_DEVICE_PARA_ID && pack->Msg.bit.bParaID != 0) || // 模块ID检查
           (pack->Msg.bit.bCabID != PackBOOT_DEVICE_CAB_ID && pack->Msg.bit.bCabID != 0)       // 机柜ID检查
        )
        {
            break; // 非本机指令，丢弃
        }
        if(pack->Msg.bit.bRX_Mes != PackBOOT_DEVICE_TYPE_RX) // 收件类型检查
        {
            CANBootWriteLower(pack->Msg.all, pack->Data, pack->DataLen);
            break; // 向下转发而不解析
        }

        CANBootPackSwitchACK(AckReg, YORO_OTA_STATE_ACK); // 运行解析前设置为正确回复包
        switch(pack->Msg.bit.bKind)
        {
        case KIND_FLASH_TYPE1_M:
        case KIND_FLASH_TYPE2_M:
        case KIND_FLASH_TYPE3_M:
        case KIND_FLASH_TYPE4_M:
        case KIND_FLASH_TYPE5_M:
            /* 一个握手包，直接回复ACK即可 */
            CANBootPackReply(pack);
            break;
        case KIND_FLASH_FAT:
            CAN_BOOT_PACK_STRUCT regPack = *pack;
            CANBootPackReply(&regPack); // 擦除前回复，表明已收到正在耗时擦除
            UB_KIND_FLASH_FAT_Handle(pack);
            CANBootPackReply(pack);
            break;
        case KIND_FLASH_CHEC:
            UB_KIND_FLASH_CHEC_Handle(pack);
            CANBootPackReply(pack);
            break;
        case KIND_FLASH_BURN:
            UB_KIND_FLASH_BURN_Handle(pack);
            break;
        case KIND_FLASH_ANS_RESET:
            UB_KIND_FLASH_RESET_Handle(pack);
            CANBootPackReply(pack);
            break;
        }
    }

    return RETURN_DEFAULT;
}

/**
 * @brief 向上位模块发送回复数据，依据pack包中的数据自动组成回复
 * @param pack 传入的数据包
 * @return 标准返回
 */
uint32_t CANBootPackReply(CAN_BOOT_PACK_STRUCT *pack)
{
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

    /* 调整回复状态码 */
    pack->Data[CAN_BOOT_REPLY_DATA_ACK] = AckReg;

    return CANBootWriteUpper(pack->Msg.all, pack->Data, CAN_BOOT_REPLY_DATA_LEN);
}

/**
 * @brief 解析命令KIND_FLASH_FAT 擦除包的分支处理
 * @param pack 传入的数据包
 */
static void UB_KIND_FLASH_FAT_Handle(CAN_BOOT_PACK_STRUCT *pack)
{
    McuBootAppStar = (uint32_t)(*(uint32_t *)&pack->Data[0]);
    McuBootAppSize = (uint32_t)(*(uint32_t *)&pack->Data[4]);

    if(McuBootAppStar != MCUBOOT_APP_START_ADD ||               // 检查起始地址范围
       (McuBootAppStar + McuBootAppSize) > MCUBOOT_APP_END_ADD) // 检查结束地址范围
    {
        McuBootAppStar = 0;
        McuBootAppSize = 0;
        CANBootPackSwitchACK(AckReg, YORO_OTA_STATE_ERRADDRESS);
        return;
    }
    FlashDownDataLen      = 0;
    FlashDownDataChecksum = 0;
    FlashDownFirstFlag    = 1;

    if(McuBootFlashErasure(McuBootAppStar, McuBootAppSize) != RETURN_DEFAULT)
    {
        CANBootPackSwitchACK(AckReg, YORO_OTA_STATE_ERRERASE);
        return;
    }
}

/**
 * @brief 解析命令KIND_FLASH_CHEC 烧录信息包的分支处理
 * @param pack 传入的数据包
 */
static void UB_KIND_FLASH_CHEC_Handle(CAN_BOOT_PACK_STRUCT *pack)
{
    McuBootDownStar = (uint32_t)(*(uint32_t *)&pack->Data[0]);
    McuDownChecksum = (uint16_t)(*(uint16_t *)&pack->Data[4]);
    McuBootDownSize = (uint16_t)(*(uint16_t *)&pack->Data[6]);

    if(McuBootDownStar < MCUBOOT_APP_START_ADD ||                 // 检查起始地址范围
       (McuBootDownStar + McuBootDownSize) > MCUBOOT_APP_END_ADD) // 检查结束地址范围
    {
        McuBootDownStar = 0;
        McuBootDownSize = 0;
        CANBootPackSwitchACK(AckReg, YORO_OTA_STATE_ERRADDRESS);
        return;
    }
    FlashDownDataLen      = 0;
    FlashDownDataChecksum = 0;

    if(McuBootDownSize > 1024) // 单次烧录超越长度检查
    {
        CANBootPackSwitchACK(AckReg, YORO_OTA_STATE_ERRADDRESS);
        return;
    }
}

/**
 * @brief 解析命令KIND_FLASH_BURN 烧录包的分支处理
 * @param pack 传入的数据包
 */
static void UB_KIND_FLASH_BURN_Handle(CAN_BOOT_PACK_STRUCT *pack)
{
#define FlashDDPushC(d)                                     \
    FlashDownData[FlashDownDataLen] = d;                    \
    FlashDownDataChecksum += d;                             \
    FlashDownDataChecksum = FlashDownDataChecksum & 0xFFFF; \
    FlashDownDataLen++;
#define FlashDDPushS(dp, len)    \
    for(int i = 0; i < len; i++) \
    {                            \
        FlashDDPushC(dp[i]);     \
    }

    if(FlashDownFirstFlag == 1) // 起始程序寄存
    {
        FlashDDPushS(pack->Data, 8);
        FlashDownDataLen -= 8;
        for(int i = 0; i < 8; i++)
        {
            McuBootFirstFour[FlashDownDataLen] = pack->Data[i];
            FlashDownDataLen++;
        }
        if(FlashDownDataLen >= 32)
        {
            FlashDownFirstFlag = 0;
            for(int i = 0; i < 32; i++)
            {
                FlashDownData[i] = 0xFF; // 清理前期包，避免形成完整程序
            }
        }
    }
    else // 其他包统一寄存
    {
        FlashDDPushS(pack->Data, pack->DataLen);
    }

    McuBootDownSize -= 8;
    if(McuBootDownSize <= 0) //包已收完，处理
    {
        if(McuDownChecksum != FlashDownDataChecksum) //校验错误，回复错误
        {
            CANBootPackSwitchACK(AckReg, YORO_OTA_STATE_ERRCHECKSUM);
        }
        else
        {
            if(McuBootFlashWrite(McuBootDownStar, FlashDownDataLen, FlashDownData) != RETURN_DEFAULT)
            {
                CANBootPackSwitchACK(AckReg, YORO_OTA_STATE_ERRERASE);
            }
            /* 最末包检测 烧录最初寄存 */
            if((McuBootDownStar + FlashDownDataLen) >= (McuBootAppStar + McuBootAppSize))
            {
                /* 烧录最初的一个字 */
                if(McuBootFlashWrite(McuBootAppStar, 32, McuBootFirstFour) != RETURN_DEFAULT)
                {
                    CANBootPackSwitchACK(AckReg, YORO_OTA_STATE_ERRERASE);
                }
            }
            CANBootPackReply(pack);
        }
    }

#undef FlashDDPushC
#undef FlashDDPushS
}

/**
 * @brief 解析命令KIND_FLASH_ANS_RESET 重启包的分支处理
 * @param pack 传入的数据包
 */
static void UB_KIND_FLASH_RESET_Handle(CAN_BOOT_PACK_STRUCT *pack)
{
    /* 自身收到重启包 ，立即进行重启流程*/
    EventTrigger(&tBootEven, EVENT_MCU_RESET);
    return;
}

#endif
