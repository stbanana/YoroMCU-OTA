/**
 * BootPort.h
 *
 *  Created on: 2024年3月14日
 *      Author: skonda15
 *
*/
/**
 *
 * 使用串口的升级协议流
*/

#ifndef _UART_BOOT_PORT_H_
#define _UART_BOOT_PORT_H_

/* Includes ------------------------------------------------------------------*/
#include "BootConfig.h"
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
/* Exported variables ---------------------------------------------------------*/
/* Exported functions ---------------------------------------------------------*/

/* 与外设通信接口定义，依据硬件平台修改对应函数内容 */
#if BOOT_PORT_UART
extern uint32_t UartBootWriteUpper(uint8_t *pcBuf, uint32_t ui32Len);
extern uint32_t UartBootGetcUpper(uint8_t *data_rx);
extern uint32_t UartBootWriteLower(uint8_t *pcBuf, uint32_t ui32Len);
extern uint32_t UartBootGetcLower(uint8_t *data_rx);
#endif

/* 与MCU内部相关的函数接口，依据硬件平台修改对应函数内容 */
extern uint32_t McuBootFlashErasure(uint32_t Addr, uint32_t Len);
extern uint32_t McuBootFlashWrite(uint32_t Addr, uint32_t Len, uint8_t *w_buff);
extern uint32_t McuBootReset(void);

#endif /* _UART_BOOT_PORT_H_ */
