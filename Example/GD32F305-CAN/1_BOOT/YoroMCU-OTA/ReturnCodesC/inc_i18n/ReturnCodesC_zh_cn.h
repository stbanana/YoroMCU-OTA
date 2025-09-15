/*
 * Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com> 
 * All rights reserved
 */

/**
 * @file ReturnCodesC_zh_cn.h
 * 
 * 所有函数返回的代码，包括正确操作返回、各种类型的错误
 * 这些返回统一认为是一种状态，错误也是一种状态
 * 若函数运行符合预期则返回RETURN_DEFAULT，其他不为0的代码则需特别注意
 * 应当使用无符号值，有符号-1并不统一导致无法向低bit范围兼容
*/

#if _RERURNCC_LANGUAGE_ == 1

#ifndef _RETURN_CODES_C_i18n_H_
#define _RETURN_CODES_C_i18n_H_

/**** 最小支持8位 ****/

/* 通用返回 */
#define RETURN_DEFAULT   (0x00) // 默认的无错误
#define RETURN_ERROR     (0x01) // 未定义的通用错误
#define RETURN_BUSY      (0x02) // 繁忙的通用返回
#define RETURN_TIMEOUT   (0x03) // 超时的通用返回
#define RETURN_ERR_PARAM (0x04) // 传递参数错误

/* 数据相关返回 */
#define RETURN_DATA_ABOVE_LIMIT (0x10) // 数据超出上限
#define RETURN_DATA_BELOW_LIMIT (0x11) // 数据超出下限
#define RETURN_DATA_LOSS        (0x12) // 数据丢失
#define RETURN_DATA_INVALID     (0x13) // 数据无效

/* BUFFER相关返回 */
#define RETURN_BUFFER_FULL      (0x20) // BUFFER满
#define RETURN_BUFFER_EMPTY     (0x21) // BUFFER空
#define RETURN_BUFFER_OVERFLOW  (0x22) // BUFFER溢出
#define RETURN_BUFFER_UNDERFLOW (0x23) // BUFFER下溢

/* 位相关返回 */
#define RETURN_BIT_SET   (0x30) // 位已置位
#define RETURN_BIT_UNSET (0x31) // 位未置位

/* 物理线路相关返回 */
#define RETURN_CABLE_FREE    (0x40) // 线路空闲
#define RETURN_CABLE_BUSY    (0x41) // 线路繁忙
#define RETURN_CABLE_LOST    (0x42) // 线路掉线
#define RETURN_CABLE_TIMEOUT (0x43) // 线路超时
#define RETURN_CABLE_ECC     (0x44) // 线路校验错误

/* 时间状态相关返回 */
#define RETURN_TIME_WAITING (0x50) // 等待中
#define RETURN_TIME_OVER    (0x51) // 超时

/* Flash存储返回 */
#define RETURN_FLASH_BUSY      (0x60) // FLASH繁忙
#define RETURN_FLASH_WRITE_ERR (0x61) // FLASH写失败
#define RETURN_FLASH_READ_ERR  (0x62) // FLASH读失败
#define RETURN_FLASH_ERASE_ERR (0x63) // FLASH擦除失败

/* 被控设备状态相关返回 */
#define RETURN_STATE_OFF   (0x70) // 设备关闭
#define RETURN_STATE_ON    (0x71) // 设备开启
#define RETURN_STATE_ERROR (0x72) // 设备错误
#define RETURN_STATE_BUSY  (0x73) // 设备繁忙

/* 模式相关返回 */
#define RETURN_MODE_ENABLED     (0x80) // 模式已启用
#define RETURN_MODE_DISABLED    (0x81) // 模式未启用
#define RETURN_MODE_UNSUPPORTED (0x82) // 模式不支持

/* 内存相关返回 */
#define RETURN_MEMORY_ALLOC (0x90) // 内存分配失败
#define RETURN_MEMORY_FREE  (0x91) // 内存释放失败

/* 文件操作相关返回 */
#define RETURN_FILE_OPEN  (0xA0) // 文件打开失败
#define RETURN_FILE_READ  (0xA1) // 文件读取失败
#define RETURN_FILE_WRITE (0xA2) // 文件写入失败
#define RETURN_FILE_CLOSE (0xA3) // 文件关闭失败

/**** 最小支持16位(待拓展) ****/

#endif /* _RETURN_CODES_C_i18n_H_ */

/**
 * 日志：
 * 版本001：
 *      2025.8.6 发布初版
 */

#endif