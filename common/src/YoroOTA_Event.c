/********************************************************************************


 **** Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com>    ****
 **** All rights reserved                                       ****

 ********************************************************************************
 * File Name     : YoroOTA_Event.c
 * Author        : yono
 * Date          : 2025-08-19
 * Version       : 1.0
********************************************************************************/
/**************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include <YoroOTA.h>
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
_EVENT_HANDLE  tBootEven;

static uint8_t SwitchFQ = 0; // 切换分频
/* Private Constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void McuResetCallBack(void);
/* Private functions ---------------------------------------------------------*/

/**
 * @brief 驱动事件系统动作需要的周期调用
 * 
 * @param pd 事件结构体指针
 * @param time_ms 指示这个函数是多长周期被调用
 */
void EventCheck(_EVENT_HANDLE *pd, uint32_t time_ms)
{
    if(pd->Event.bit.EventMcuReset == 1)
        McuResetCallBack( );
}

/**
 * @brief 触发事件结构体的某个事件
 * 
 * @param pd  事件结构体指针
 * @param event  触发的事件，见宏定义
 * @return uint32_t 标准返回
 */
uint32_t EventTrigger(_EVENT_HANDLE *pd, uint32_t event)
{
    pd->Event.all |= event;
    return RETURN_DEFAULT;
}

/**
 * @brief 清除事件结构体的某个事件
 * 
 * @param pd  事件结构体指针
 * @param event  清除的事件，见宏定义
 * @return uint32_t 标准返回
 */
uint32_t EventClear(_EVENT_HANDLE *pd, uint32_t event)
{
    pd->Event.all &= (~event);
    return RETURN_DEFAULT;
}

/*      事件回调函数        */
static void McuResetCallBack(void)
{
    if(SwitchFQ > 100)
    {
        /* 进入重启流程 */
        McuBootReset( );
        EventClear(&tBootEven, EVENT_MCU_RESET); // 事件结束，清除本事件
    }
    else
    {
        SwitchFQ++;
    }
}
