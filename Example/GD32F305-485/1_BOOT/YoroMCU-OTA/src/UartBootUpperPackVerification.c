/********************************************************************************


 **** Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : UartBootUpperPackVerification.c
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
extern uint32_t UartBootPackReply(UART_BOOT_PACK_STRUCT *pack);
/* Private functions ---------------------------------------------------------*/
/**
 * @brief 上位模块信息的完整包验证
 * 
 * @param state 完整包验证状态 结构体指针
 * @param pack 包信息数据 结构体指针
 * @param msTime 调用此函数的周期时间指示(ms)
 * @return uint32_t 标准返回
 */
uint32_t UartUpperPackVerification(BOOT_PARSE_STATE_STRUCT *state, UART_BOOT_PACK_STRUCT *pack, uint16_t msTime)
{
    static uint8_t Data; // 取单个数据的变量

    state->time += msTime;
    if(state->time > state->timeout) // 检查超时
    {
        UartBootParsetInit( );
        return RETURN_TIME_OVER;
    }

    while(UartBootGetcUpper(&Data) == RETURN_DEFAULT)
    {
        state->time = 0;
        switch(state->step)
        {
            /* 帧头判断 */
        case UART_BOOT_PACKET_HEAD1:
            if(Data == UART_BOOT_HEAD1) // 帧头1判断
                state->step = UART_BOOT_PACKET_HEAD2;
            break;
        case UART_BOOT_PACKET_HEAD2:
            if(Data == UART_BOOT_HEAD2) // 帧头2判断
                state->step = UART_BOOT_PACKET_MSG1;
            else if(Data != UART_BOOT_HEAD1) // 非帧头1判断
                state->step = UART_BOOT_PACKET_HEAD1;
            break;

            /* 收取命令信息 */
        case UART_BOOT_PACKET_MSG1:
            pack->Msg.all       = 0; // 重置烧录命令
            pack->DataBufferNum = 0; // 重置buffer填充
            pack->SumNot        = 0; // 重置校验

            pack->Msg.all |= (uint32_t)Data; // 收取低8位烧录信息指令
            pack->SumNot += Data;            // 更新校验位
            state->step = UART_BOOT_PACKET_MSG2;
            break;

        case UART_BOOT_PACKET_MSG2:
            pack->Msg.all |= (uint32_t)Data << 8; // 收取次低8位烧录信息指令
            pack->SumNot += Data;                 // 更新校验位
            state->step = UART_BOOT_PACKET_MSG3;
            break;

        case UART_BOOT_PACKET_MSG3:
            pack->Msg.all |= (uint32_t)Data << 16; // 收取次高8位烧录信息指令
            pack->SumNot += Data;                  // 更新校验位
            state->step = UART_BOOT_PACKET_MSG4;
            break;

        case UART_BOOT_PACKET_MSG4:
            pack->Msg.all |= (uint32_t)Data << 24; // 收取最高8位烧录信息指令
            pack->SumNot += Data;                  // 更新校验位
            state->step = UART_BOOT_PACKET_LEN1;
            break;

            /* 收取长度信息 */
        case UART_BOOT_PACKET_LEN1:
            pack->DataLen = Data; // 收取低8位长度指示
            pack->SumNot += Data; // 更新校验位
            state->step = UART_BOOT_PACKET_LEN2;
            break;

        case UART_BOOT_PACKET_LEN2:
            pack->DataLen += (uint16_t)Data << 8; // 收取高8位长度指示
            pack->SumNot += Data;                 // 更新校验位

            if(pack->Msg.bit.bRX_Mes != PackBOOT_DEVICE_TYPE_RX) // 指示数据收取完毕，判断是否是自己的包
            {
                UartBootWriteLower((uint8_t *)UartPackHead, 2); // 非自己的包，进入转发态向下转发
                UartBootWriteLower((uint8_t *)pack, 6);         // 转发此前接收的数据
                pack->DataLen++;                                // 将校验字节也做数据转发
                state->step = UART_BOOT_PACKET_FRW;
            }
            else
                state->step = UART_BOOT_PACKET_DATA;
            break;

            /* 收取数据信息 */
        case UART_BOOT_PACKET_DATA:
            pack->Data[pack->DataBufferNum] = Data; // 获取数据入buffer
            pack->DataBufferNum++;
            pack->SumNot += Data; // 更新校验位
            if(pack->DataBufferNum >= pack->DataLen)
            {
                state->step = UART_BOOT_PACKET_TAIL;
            }
            break;

            /* 转发态 */
        case UART_BOOT_PACKET_FRW:
            UartBootWriteLower(&Data, 1); // 向下转发数据
            pack->DataBufferNum++;
            pack->SumNot += Data; // 更新校验位
            if(pack->DataBufferNum >= pack->DataLen)
            {
                if(pack->Msg.bit.bKind == KIND_FLASH_ANS_RESET) // 向下的重启包，本机也需要跟随重启
                {
                    EventTrigger(&tBootEven, EVENT_MCU_RESET);
                }
                state->step = UART_BOOT_PACKET_HEAD1;
            }
            break;

            /* 比对校验信息 */
        case UART_BOOT_PACKET_TAIL:
            pack->SumNot = ~pack->SumNot;
            state->step  = UART_BOOT_PACKET_HEAD1;
            if(pack->SumNot == Data) //判断校验,完整正确包立即返回
                return RETURN_DEFAULT;
            break;

        default:
            state->step = UART_BOOT_PACKET_HEAD1; // 未知状态，调整进入工作流
            break;
        }
    }

    return RETURN_TIME_WAITING;
}

#endif
