/**
 * YoroOTA_Event.h
 *
 *  Created on: 2025年8月19日
 *      Author: yono
 *
*/
/**
 *
 * 专供bootload程序的简易事件系统
*/

#ifndef _YORO_OTA_EVENT_H_
#define _YORO_OTA_EVENT_H_
#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
#define EVENT_MCU_RESET (1 << 1)
/* Exported types ------------------------------------------------------------*/

typedef struct
{
    uint32_t reg          :1;
    uint32_t EventMcuReset:1;
    uint32_t Reg          :30;
} _EVENT_BITS;

typedef struct
{
    union CanProtoMsgID
    {
        uint32_t    all;
        _EVENT_BITS bit;
    } Event;

} _EVENT_HANDLE;

/* Exported variables ---------------------------------------------------------*/
extern _EVENT_HANDLE tBootEven;
/* Exported functions ---------------------------------------------------------*/

extern void     EventCheck(_EVENT_HANDLE *pd, uint32_t time_ms);

extern uint32_t EventTrigger(_EVENT_HANDLE *pd, uint32_t event);
extern uint32_t EventClear(_EVENT_HANDLE *pd, uint32_t event);

#ifdef __cplusplus
}
#endif
#endif /* _YORO_OTA_EVENT_H_ */
