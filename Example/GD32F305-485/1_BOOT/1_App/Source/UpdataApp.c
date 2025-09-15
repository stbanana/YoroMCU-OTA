/********************************************************************************


 **** Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : UpdataApp.c
 * Author        : yono
 * Date          : 2025-08-19
 * Version       : 1.0
********************************************************************************/
/**************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include <includes.h>

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint16_t LEDTime = 0;

/* 总线空闲态手动监测 */
extern int8_t U2IDLE_Delay;
extern size_t U2RxBufferIndex;
/* Private Constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

uint32_t UpdataApp(void)
{
    SysInit( );
    DFlow_User_Init( );

    YoroOTAInit( );
    //停留在当前BOOT中
    while(1)
    {
        YoroOTARun(1); // YoroMCU-OTA 运行

        if(LEDTime++ > 100) // Boot中的LED快速闪烁
        {
            gpio_bit_write(GPIOC, GPIO_PIN_4, !gpio_input_bit_get(GPIOC, GPIO_PIN_4));
            LEDTime = 0;
        }

        /* 空闲中断处理手动触发 */
        if(U2IDLE_Delay > 0)
        {
            U2IDLE_Delay--;
            if(U2IDLE_Delay == 0)
            {
                DFlow_Interrupt_IDLE_RC_FTF(&DFlow, U2RxBufferIndex);
                U2RxBufferIndex = 0;
            }
        }
        DFlow_Ticks(&DFlow); // 协助485管理
        delay_1ms(1);
    }
}
