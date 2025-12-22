
/********************************************************************************


 **** Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : BootPort.c
 * Author        : yono
 * Date          : 2025-08-19
 * Version       : 1.0
********************************************************************************/
/**************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include <YoroOTA.h>
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private Constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
 * @brief 擦除程序存储区的接口函数
 * @param Addr 擦除起始地址
 * @param Len 擦除长度
 * @return 标准返回
 */
uint32_t McuBootFlashErasure(volatile uint32_t Addr, volatile uint32_t Len)
{
    static uint32_t PageNum;

    /* 检查起始地址是否是1024对齐 */
    if(Addr % 0x400 != 0)
        return RETURN_ERR_PARAM;

    /* 计算擦除页数 */
    PageNum = Len / 0x400;
    if(PageNum % 0x400 != 0)
        PageNum += 1;

    /* 平台特定的擦除流程*/
    /* 解锁Flash(需填) */
    // fmc_unlock( );
    /* 擦除Flash */
    while(PageNum != 0)
    {
        PageNum--;
        /* 需要修改调用自己的擦除函数(需填) */
        //     if(fmc_page_erase(Addr + PageNum * 0x400) != FMC_READY) // 擦除失败，立即返回
        //         return RETURN_FLASH_ERASE_ERR;
        //     fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_WPERR | FMC_FLAG_PGERR);
    }
    /* 上锁Flash(需填) */
    // fmc_lock( );

    return RETURN_DEFAULT;
}

/**
 * @brief 写入程序存储区的接口函数
 * @param Addr 写入起始地址
 * @param Len 写入长度
 * @param w_buff 写入数据地址头
 * @return 标准返回
 */
uint32_t McuBootFlashWrite(volatile uint32_t Addr, volatile uint32_t Len, volatile uint8_t *w_buff)
{
    static uint32_t  OffsetLen; // 记录当前的偏移长度
    static uint32_t *WordData;  // 记录当前写入的字地址

    WordData = (uint32_t *)w_buff;

    /* 平台特定的写入流程*/
    /* 解锁Flash(需填) */
    // fmc_unlock( );
    for(OffsetLen = 0; OffsetLen < Len; OffsetLen += 4) // 逐字写入
    {
        /* 需要修改调用自己的写入函数(需填) */
        // if(fmc_word_program(Addr + OffsetLen, *WordData) != FMC_READY) // 写入失败，立即返回
        //     return RETURN_FLASH_WRITE_ERR;
        // fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_WPERR | FMC_FLAG_PGERR);
        WordData += 1;
    }
    /* 上锁Flash(需填) */
    // fmc_lock( );

    return RETURN_DEFAULT;
}

/**
 * @brief 依据平台的重启接口函数，需要是可以延迟的重启，否则最后的通信无法完成
 * @return 标准返回
 */
uint32_t McuBootReset(void)
{
    volatile uint32_t i = 0;
    /* 少量硬延迟，完成回复包和其他工作 */
    while(i < 0x00CFFFF)
        i++;
    /* 进行芯片特定的重启流程(需填) */
    // InitNVIC( );
    // NVIC_SystemReset( ); //reset

    while(1)
        ;
}
