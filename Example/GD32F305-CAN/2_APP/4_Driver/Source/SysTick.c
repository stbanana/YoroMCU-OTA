/*!
    \file  systick.c
    \brief the systick configuration file
*/

/*
    Copyright (C) 2017 GigaDevice

    2017-06-06, V1.0.0, firmware for GD32F3x0
*/

#include "gd32f30x.h"
#include "systick.h"

static volatile float count_1us = 0;
static volatile float count_1ms = 0;

/**
 * @brief 设置系统时钟源，并且计算1us和1ms需要多少个系统时钟
 * 
 */
void systick_config(void)
{
    // setup systick timer for 1000Hz interrupts
    //if (SysTick_Config(SystemCoreClock / 10000U))	// 从APP跳进来时这个变量被改变，现改为宏定义
    systick_clksource_set(SYSTICK_CLKSOURCE_HCLK_DIV8);
    count_1us = (float)120000000 / 12000000;
    count_1ms = (float)count_1us * 1000;
}

/**
 * @brief 阻塞等待，1ms单位
 * 
 * @param count 指定等待多少个1ms
 */
void delay_1ms(uint32_t count)
{
    uint32_t ctl;

    /* reload the count value */
    SysTick->LOAD = (uint32_t)(count * count_1ms);
    /* clear the current count value */
    SysTick->VAL = 0x0000U;
    /* enable the systick timer */
    SysTick->CTRL = SysTick_CTRL_ENABLE_Msk;
    /* wait for the COUNTFLAG flag set */
    do
    {
        ctl = SysTick->CTRL;
    }
    while((ctl & SysTick_CTRL_ENABLE_Msk) && !(ctl & SysTick_CTRL_COUNTFLAG_Msk));
    /* disable the systick timer */
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    /* clear the current count value */
    SysTick->VAL = 0x0000U;
}

/**
 * @brief 阻塞等待，1us单位
 * 
 * @param count 指定等待多少个1us
 */
void delay_1us(uint32_t count)
{
    uint32_t ctl;

    /* reload the count value */
    SysTick->LOAD = (uint32_t)(count * count_1us);
    /* clear the current count value */
    SysTick->VAL = 0x0000U;
    /* enable the systick timer */
    SysTick->CTRL = SysTick_CTRL_ENABLE_Msk;
    /* wait for the COUNTFLAG flag set */
    do
    {
        ctl = SysTick->CTRL;
    }
    while((ctl & SysTick_CTRL_ENABLE_Msk) && !(ctl & SysTick_CTRL_COUNTFLAG_Msk));
    /* disable the systick timer */
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    /* clear the current count value */
    SysTick->VAL = 0x0000U;
}
