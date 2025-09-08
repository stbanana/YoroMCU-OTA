/*!
    \file  gd32f3x0_it.c
    \brief interrupt service routines
*/

/*
    Copyright (C) 2017 GigaDevice

    2017-06-06, V1.0.0, firmware for GD32F3x0
*/

#include "includes.h"
/*!
    \brief      this function handles NMI exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void NMI_Handler(void)
{
}

/*!
    \brief      this function handles HardFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
//void HardFault_Handler(void)
//{
//    /* if Hard Fault exception occurs, go to infinite loop */
//    while(1)
//    {
//    }
//}

/*!
    \brief      this function handles MemManage exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void MemManage_Handler(void)
{
    /* if Memory Manage exception occurs, go to infinite loop */
    while(1)
    {
    }
}

/*!
    \brief      this function handles BusFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void BusFault_Handler(void)
{
    /* if Bus Fault exception occurs, go to infinite loop */
    while(1)
    {
    }
}

/*!
    \brief      this function handles UsageFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void UsageFault_Handler(void)
{
    /* if Usage Fault exception occurs, go to infinite loop */
    while(1)
    {
    }
}

/*!
    \brief      this function handles SVC exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SVC_Handler(void)
{
    // 1. 禁用所有中断
    __disable_irq( );

    // 2. 内联汇编执行完整启动初始化
    __asm volatile(
        // 重置栈指针
        "ldr r0, =_sp\n\t"
        "mov sp, r0\n\t"

        // 重新加载.data段（已初始化变量）
        "ldr r0, =_sdata\n\t" // RAM目标地址
        "ldr r1, =_edata\n\t"
        "ldr r2, =_sidata\n\t" // Flash源地址
        "b 2f\n\t"
        "1:\n\t"
        "ldr r4, [r2], #4\n\t" // 从Flash加载
        "str r4, [r0], #4\n\t" // 存储到RAM
        "2:\n\t"
        "cmp r0, r1\n\t"
        "bcc 1b\n\t"

        // 清零.bss段（未初始化变量）
        "ldr r0, =_sbss\n\t"
        "ldr r1, =_ebss\n\t"
        "mov r2, #0\n\t"
        "b 4f\n\t"
        "3:\n\t"
        "str r2, [r0], #4\n\t" // 清零4字节
        "4:\n\t"
        "cmp r0, r1\n\t"
        "bcc 3b\n\t"

        // 内存屏障确保操作完成
        "dsb\n\t"
        "isb\n\t"

        // 重新初始化系统
        "bl SystemInit\n\t"

        // 跳转到OTA主函数
        "b UpdataApp\n\t");
}
/*!
    \brief      this function handles DebugMon exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void DebugMon_Handler(void)
{
}

/*!
    \brief      this function handles PendSV exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void PendSV_Handler(void)
{
}

/*!
    \brief      this function handles SysTick exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
