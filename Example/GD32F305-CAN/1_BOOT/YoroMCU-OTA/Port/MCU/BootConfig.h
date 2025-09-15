/**
 * BootConfig.h
 *
 *  Created on: 2025年8月19日
 *      Author: yono
 *
*/
/**
 *
 * boot升级程序配置
*/

#ifndef _BOOT_CONFIG_H_
#define _BOOT_CONFIG_H_
#ifdef __cplusplus
extern "C"
{
#endif

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

/* 定义本机机柜和模块号 
    正常的应用下可替换为函数或者某个保留变量 */
/* 模块号 */
#define PackBOOT_DEVICE_PARA_ID 1
/* 机柜号 */
#define PackBOOT_DEVICE_CAB_ID 1

/* 定义本机类型 */
/* 收件类型 */
#define PackBOOT_DEVICE_TYPE_RX RX_OBJECT_TYPE1
/* 发件类型 */
#define PackBOOT_DEVICE_TYPE_TX TX_OBJECT_TYPE1

/* 物理接口的裁剪，为0则裁剪掉对应接口，以供节省资源 */
#define BOOT_PORT_UART 0 // 使用UART接口
#define BOOT_PORT_CAN  1 // 使用CAN接口

/* Exported types ------------------------------------------------------------*/

/* Exported variables ---------------------------------------------------------*/

/* Exported functions ---------------------------------------------------------*/

#ifdef __cplusplus
}
#endif
#endif /* _BOOT_CONFIG_H_ */
