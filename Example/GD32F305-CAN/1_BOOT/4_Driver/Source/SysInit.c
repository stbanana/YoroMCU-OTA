/********************************************************************************


 **** Copyright (C), 2025, yono<yono233@outlook.com>  ****


 ********************************************************************************
 * File Name     : SysInit.c
 * Author        : yono
 * Date          : 2024-03-13
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
static void InitCAN(void);
/* Private functions ---------------------------------------------------------*/

void SysInit(void)
{
    InitNVIC( );
    systick_config( ); // 设置系统时钟，支持delay

    InitPinSet( ); // 设置引脚功能复用
    InitCAN( );    // 初始化CAN功能

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
    nvic_vector_table_set(NVIC_VECTTAB_FLASH, 0x0000); // 设置中断表偏移

    __set_PRIMASK(0); // 打开全局中断
}

static void InitPinSet(void)
{
    // 时钟配置
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_AF);

    /* F30x用法*/
    /* CAN1 - TXPB6  CAN1 - RXPB5*/
    gpio_init(GPIOB, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, GPIO_PIN_5);
    gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
    gpio_pin_remap_config(GPIO_CAN1_REMAP, ENABLE);
    /*LED*/
    gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_4);
}

static void InitCAN(void)
{
    // 时钟配置
    rcu_periph_clock_enable(RCU_CAN0); // GD32特性：虽然只使用CAN1，但是CAN0的时钟也必须打开
    rcu_periph_clock_enable(RCU_CAN1);
    /* CAN1参数配置 */
    can_parameter_struct        can_parameter;
    can_filter_parameter_struct can_filter;
    can_struct_para_init(CAN_INIT_STRUCT, &can_parameter);
    can_struct_para_init(CAN_FILTER_STRUCT, &can_filter);

    can_deinit(CAN1);

    can_parameter.time_triggered        = DISABLE;
    can_parameter.auto_bus_off_recovery = ENABLE;
    can_parameter.auto_wake_up          = DISABLE;
    can_parameter.auto_retrans          = ENABLE;
    can_parameter.rec_fifo_overwrite    = DISABLE;
    can_parameter.trans_fifo_order      = DISABLE;
    can_parameter.working_mode          = CAN_NORMAL_MODE;
    can_parameter.resync_jump_width     = CAN_BT_SJW_1TQ;
    can_parameter.time_segment_1        = CAN_BT_BS1_7TQ;
    can_parameter.time_segment_2        = CAN_BT_BS2_2TQ;

    can_parameter.prescaler             = 24; // 250k波特率

    can_init(CAN1, &can_parameter);

    /* CAN1过滤器参数 */
    can_filter.filter_number      = 0;
    can_filter.filter_mode        = CAN_FILTERMODE_MASK;
    can_filter.filter_bits        = CAN_FILTERBITS_32BIT;
    can_filter.filter_list_high   = 0x0000;
    can_filter.filter_list_low    = 0x0000;
    can_filter.filter_mask_high   = 0x0000;
    can_filter.filter_mask_low    = 0x0000;
    can_filter.filter_fifo_number = CAN_FIFO0;
    can_filter.filter_enable      = ENABLE;
    can_filter.filter_number      = 15; // 编号15开始CAN1配置
    can_filter_init(&can_filter);

    nvic_irq_enable(CAN1_RX0_IRQn, 1, 1);
    can_interrupt_enable(CAN1, CAN_INT_RFNE0);
}
