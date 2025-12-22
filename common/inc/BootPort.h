/**
 * BootPort.h
 *
 *  Created on: 2025年8月19日
 *      Author: yono
 *
*/
/**
 *
 * 接口层的引入
*/

#ifndef _BOOT_PORT_H_
#define _BOOT_PORT_H_
#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "../Port/MCU/BootConfig.h"
#include <stdint.h>
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/

/* DSP 16位字节兼容层 */
#if defined(__TI_COMPILER_VERSION__) && defined(__TMS320C2000__)
#define _YOROOTA_16BIT_BYTE
#endif

#ifdef _YOROOTA_16BIT_BYTE
#ifndef _UINT8_T_DECLARED
typedef __uint16_t uint8_t;
#define _UINT8_T_DECLARED
#endif
#endif

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
#if BOOT_PORT_CAN
extern uint32_t CANBootWriteUpper(uint32_t id, uint8_t *pcBuf, uint8_t ui8Len);
extern uint32_t CANBootGetcUpper(uint32_t *id, uint8_t *data_rx, uint8_t *ui8Len);
extern uint32_t CANBootWriteLower(uint32_t id, uint8_t *pcBuf, uint8_t ui8Len);
extern uint32_t CANBootGetcLower(uint32_t *id, uint8_t *data_rx, uint8_t *ui8Len);
#endif

/* 与MCU内部相关的函数接口，依据硬件平台修改对应函数内容 */
extern uint32_t McuBootFlashErasure(volatile uint32_t Addr, volatile uint32_t Len);
extern uint32_t McuBootFlashWrite(volatile uint32_t Addr, volatile uint32_t Len, volatile uint8_t *w_buff);
extern uint32_t McuBootReset(void);

#ifdef __cplusplus
}
#endif
#endif /* _BOOT_PORT_H_ */
