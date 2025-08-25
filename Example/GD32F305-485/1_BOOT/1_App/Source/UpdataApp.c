/********************************************************************************


 **** Copyright (C), 2020, Shenzhen SKONDA ELECTRONIC LTD  ****


 ********************************************************************************
 * File Name     : UpdataApp.c
 * Author        : SKONDA
 * Date          : 2024-03-13
 * Version       : 1.0
********************************************************************************/
/**************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include <includes.h>

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern _DFlow   DFlow;

static uint16_t LEDTime = 0;
/* Private Constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

uint32_t UpdataApp(void)
{
    SysInit( );

    YoroOTAInit( );
    //停留在当前BOOT中
    while(1)
    {
        DFlow_Ticks(&DFlow); // 协助485管理
        YoroOTARun(1);       // YoroMCU-OTA 运行

        if(LEDTime++ > 100) // Boot中的LED快速闪烁
        {
            gpio_bit_write(GPIOC, GPIO_PIN_4, !gpio_input_bit_get(GPIOC, GPIO_PIN_4));
            LEDTime = 0;
        }
        delay_1ms(1);
    }
}
