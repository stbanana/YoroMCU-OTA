/*
 * PlatformReturn.h
 *
 *  Created on: 2023年6月29日
 *      Author: skonda15
*/
/**
 * 本平台所有函数返回的代码，包括正确操作返回、各种类型的错误
 * 这些返回统一认为是一种状态，错误也是一种状态
 * 若函数运行符合预期则返回RETURN_DEFAULT，其他不为0的代码均认为是异常
*/

#ifndef _PLATFORMRETURN_H_
#define _PLATFORMRETURN_H_

/* 通用返回 */
#define RETURN_DEFAULT        0x00 //默认的无错误
#define RETURN_ERR_INDEFINITE 0x01 //默认的未指定错误
#define RETURN_ERR_PARAM      0x02 //传递参数错误
/* 数据相关返回 */
#define RETURN_DATA_ABOVE_LIMIT 0x10 //数据超出上限
#define RETURN_DATA_BELOW_LIMIT 0x11 //数据超出下限
#define RETURN_DATA_LOSS        0x12 //数据丢失
/* BUFFER相关返回 */
#define RETURN_BUFFER_FULL  0x20 //BUFFER满
#define RETURN_BUFFER_EMPTY 0x21 //BUFFER空
/* 位相关返回 */
#define RETURN_BIT_SETTING   0x30 //位已置位
#define RETURN_BIT_UNSETTING 0x31 //位没有置位
/* 物理线路相关返回 */
#define RETURN_CABLE_FREE 0x40 //线路空闲
#define RETURN_CABLE_BUSY 0x41 //线路繁忙
#define RETURN_CABLE_LOST 0x41 //线路掉线
/* 时间状态相关返回 */
#define RETURN_TIME_WAIL 0x50 // 等待时
#define RETURN_TIME_OVER 0x51 // 超时
/* Flash存储返回 */
#define RETURN_FLASH_BUSY  0x60 // FLASH繁忙
#define RETURN_FLASH_ERROR 0x61 // FLASH操作失败

#endif /* 5_PHYSICALCHIP_CPU_PLATFORMRETURN_H_ */
