/********************************************************************************


 **** Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : SysInit.c
 * Author        : yono
 * Date          : 2025-08-19
 * Version       : 1.0
********************************************************************************/
/**************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include <includes.h>
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private Constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void InitPinSet(void);
static void InitUart(void);
/* Private functions ---------------------------------------------------------*/

void SysInit(void)
{
    InitNVIC( );
    systick_config( ); // 设置系统时钟，支持delay

    InitPinSet( ); // 设置引脚功能复用
    InitUart( );   // 初始化Uart功能

    __enable_irq( );
}

/**
  * @brief 关闭并清除所有中断
  * @param None
  * @retval None
*/
void InitNVIC(void)
{
    uint32_t i = 0;
    __set_PRIMASK(1); // 关闭全局中断

    /* 关闭滴答定时器，复位到默认值 */
    SysTick->CTRL = 0;
    SysTick->LOAD = 0;
    SysTick->VAL  = 0;

    /* 关闭所有中断，清除所有中断挂起标志 */
    for(i = 0; i < 8; i++)
    {
        NVIC->ICER[i] = 0xFFFFFFFF;
        NVIC->ICPR[i] = 0xFFFFFFFF;
    }

    /* 设置中断向量表偏移量 */
    nvic_vector_table_set(NVIC_VECTTAB_FLASH, 0x2000); // 设置中断表偏移

    __set_PRIMASK(0); // 打开全局中断
}

static void InitPinSet(void)
{
    // 时钟配置
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_AF);

    /* F30x用法*/
    /* USART2 - TXPB10  USART2 - RXPB11*/
    gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
    gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_11);
    /*RS485 EN*/
    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_1);
    /*LED*/
    gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_4);

    /* UART3 - TXPC10  UART3 - RXPC11*/
    gpio_init(GPIOC, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
    gpio_init(GPIOC, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_11);
    // gpio_pin_remap_config(GPIO_USART2_PARTIAL_REMAP, ENABLE);

    /* USART0 - TXPA9  USART0 - RXPA10*/
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_10);

    /* F3x0用法 */
    // /*PB6  USART0 - TX PB7 USART0 - RX*/
    // gpio_af_set(GPIOB, GPIO_AF_0, GPIO_PIN_6 | GPIO_PIN_7); // PB6 U0TX PB7 U0RX

    // gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_6);
    // gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6);

    // gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_7);
    // gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_7);

    // /*RS485 EN*/
    // gpio_mode_set(GPIOF, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_7);
    // gpio_output_options_set(GPIOF, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_7);
    // gpio_bit_write(GPIOF, GPIO_PIN_7, SET); // RS485使能引脚拉高
}

static void InitUart(void)
{
    // 时钟配置
    rcu_periph_clock_enable(RCU_USART2);
    /* Uart2参数配置 */
    usart_deinit(USART2);
    usart_baudrate_set(USART2, const_BR_9600bps);         // 配置波特率
    usart_word_length_set(USART2, USART_WL_8BIT);         // 数据位8位
    usart_stop_bit_set(USART2, USART_STB_1BIT);           // 停止位1位
    usart_parity_config(USART2, USART_PM_NONE);           // 无校验位
    usart_receive_config(USART2, USART_RECEIVE_ENABLE);   // 启用接收
    usart_transmit_config(USART2, USART_TRANSMIT_ENABLE); // 启用发送
    usart_interrupt_enable(USART2, USART_INT_RBNE | USART_INT_IDLE);
    usart_enable(USART2); // 配置使能
    nvic_irq_enable(USART2_IRQn, 0U, 0U);

    // 时钟配置
    rcu_periph_clock_enable(RCU_USART0);
    /* Uart0参数配置 */
    usart_deinit(USART0);
    usart_baudrate_set(USART0, const_BR_9600bps);         // 配置波特率
    usart_word_length_set(USART0, USART_WL_8BIT);         // 数据位8位
    usart_stop_bit_set(USART0, USART_STB_1BIT);           // 停止位1位
    usart_parity_config(USART0, USART_PM_NONE);           // 无校验位
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);   // 启用接收
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE); // 启用发送
    usart_enable(USART0);                                 // 配置使能

    // 时钟配置
    rcu_periph_clock_enable(RCU_UART3);
    /* Uart3参数配置 */
    usart_deinit(UART3);
    usart_baudrate_set(UART3, const_BR_9600bps);         // 配置波特率
    usart_word_length_set(UART3, USART_WL_8BIT);         // 数据位8位
    usart_stop_bit_set(UART3, USART_STB_1BIT);           // 停止位1位
    usart_parity_config(UART3, USART_PM_NONE);           // 无校验位
    usart_receive_config(UART3, USART_RECEIVE_ENABLE);   // 启用接收
    usart_transmit_config(UART3, USART_TRANSMIT_ENABLE); // 启用发送
    usart_enable(UART3);                                 // 配置使能
}
