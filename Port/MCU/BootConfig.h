/**
 * BootConfig.h
 *
 *  Created on: 2024年3月13日
 *      Author: skonda15
 *
*/
/**
 *
 * boot升级程序配置
*/

#ifndef _BOOT_CONFIG_H_
#define _BOOT_CONFIG_H_

/* Includes ------------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/

/* 一次flash写入最大数据长度 */
#define BOOT_SINGLE_WRITE_SIZE_MAX 512

/* 使用命令指示，但以此校验 */
/* 定义用户APP起始地址 烧录起始地址 */
#define MCUBOOT_APP_START_ADD 0x8002000
/* 定义用户APP可用的结尾地址 超出此地址禁止烧录并回报错误 */
#define MCUBOOT_APP_END_ADD 0x8010000

/* 定义本机类型 */
#define PackBOOT_DEVICE_TYPE_RX RX_OBJECT_TYPE2
#define PackBOOT_DEVICE_TYPE_TX TX_OBJECT_TYPE2

/* 物理接口的裁剪，为0则裁剪掉对应接口，以供节省资源 */
#define BOOT_PORT_UART 1 // 使用UART接口
#define BOOT_PORT_CAN  1 // 使用CAN接口

/* Exported types ------------------------------------------------------------*/

/* Exported variables ---------------------------------------------------------*/

/* Exported functions ---------------------------------------------------------*/

#endif /* _BOOT_CONFIG_H_ */
