/**
 * McuBoot.h
 *
 *  Created on: 2025年8月19日
 *      Author: yono
 *
*/
/**
 *
 * boot升级模组头文件
*/

#ifndef _YORO_OTA_H_
#define _YORO_OTA_H_

/* Includes ------------------------------------------------------------------*/
#include "BootPort.h"
#include "YoroOTA_Def.h"
#include "YoroOTA_Uart.h"
#include "YoroOTA_CAN.h"
#include "YoroOTA_Event.h"
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported variables ---------------------------------------------------------*/

/* Exported functions ---------------------------------------------------------*/

extern void YoroOTAInit(void);
extern void YoroOTARun(uint16_t msTime);

#endif /* _YORO_OTA_H_ */
