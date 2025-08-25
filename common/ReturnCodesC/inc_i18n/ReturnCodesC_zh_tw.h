/*
 * Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com> 
 * All rights reserved
 */

/**
 * @file ReturnCodesC_zh_tw.h
 * 
 * 所有函數返回的代碼，包括正確操作返回、各種類型的錯誤
 * 這些返回統一認為是一種狀態，錯誤也是一種狀態
 * 若函數運行符合預期則返回RETURN_DEFAULT，其他不為0的代碼則需特別注意
 * 應當使用無符號值，有符號-1並不統一導致無法向低bit範圍兼容
 */

#if _RERURNCC_LANGUAGE_ == 2

#ifndef _RETURN_CODES_C_i18n_H_
#define _RETURN_CODES_C_i18n_H_

/**** 最小支持8位 ****/

/* 通用返回 */
#define RETURN_DEFAULT   (0x00) // 默認的無錯誤
#define RETURN_ERROR     (0x01) // 未定義的通用錯誤
#define RETURN_BUSY      (0x02) // 繁忙的通用返回
#define RETURN_TIMEOUT   (0x03) // 超時的通用返回
#define RETURN_ERR_PARAM (0x04) // 傳遞參數錯誤

/* 數據相關返回 */
#define RETURN_DATA_ABOVE_LIMIT (0x10) // 數據超出上限
#define RETURN_DATA_BELOW_LIMIT (0x11) // 數據超出下限
#define RETURN_DATA_LOSS        (0x12) // 數據丟失
#define RETURN_DATA_INVALID     (0x13) // 數據無效

/* BUFFER相關返回 */
#define RETURN_BUFFER_FULL      (0x20) // BUFFER滿
#define RETURN_BUFFER_EMPTY     (0x21) // BUFFER空
#define RETURN_BUFFER_OVERFLOW  (0x22) // BUFFER溢出
#define RETURN_BUFFER_UNDERFLOW (0x23) // BUFFER下溢

/* 位相關返回 */
#define RETURN_BIT_SET   (0x30) // 位已置位
#define RETURN_BIT_UNSET (0x31) // 位未置位

/* 物理線路相關返回 */
#define RETURN_CABLE_FREE    (0x40) // 線路空閒
#define RETURN_CABLE_BUSY    (0x41) // 線路繁忙
#define RETURN_CABLE_LOST    (0x42) // 線路掉線
#define RETURN_CABLE_TIMEOUT (0x43) // 線路超時
#define RETURN_CABLE_ECC     (0x44) // 線路校驗錯誤

/* 時間狀態相關返回 */
#define RETURN_TIME_WAITING (0x50) // 等待中
#define RETURN_TIME_OVER    (0x51) // 超時

/* Flash存儲返回 */
#define RETURN_FLASH_BUSY      (0x60) // FLASH繁忙
#define RETURN_FLASH_WRITE_ERR (0x61) // FLASH寫失敗
#define RETURN_FLASH_READ_ERR  (0x62) // FLASH讀失敗
#define RETURN_FLASH_ERASE_ERR (0x63) // FLASH擦除失敗

/* 被控設備狀態相關返回 */
#define RETURN_STATE_OFF   (0x70) // 設備關閉
#define RETURN_STATE_ON    (0x71) // 設備開啟
#define RETURN_STATE_ERROR (0x72) // 設備錯誤
#define RETURN_STATE_BUSY  (0x73) // 設備繁忙

/* 模式相關返回 */
#define RETURN_MODE_ENABLED     (0x80) // 模式已啟用
#define RETURN_MODE_DISABLED    (0x81) // 模式未啟用
#define RETURN_MODE_UNSUPPORTED (0x82) // 模式不支持

/* 內存相關返回 */
#define RETURN_MEMORY_ALLOC (0x90) // 內存分配失敗
#define RETURN_MEMORY_FREE  (0x91) // 內存釋放失敗

/* 文件操作相關返回 */
#define RETURN_FILE_OPEN  (0xA0) // 文件打開失敗
#define RETURN_FILE_READ  (0xA1) // 文件讀取失敗
#define RETURN_FILE_WRITE (0xA2) // 文件寫入失敗
#define RETURN_FILE_CLOSE (0xA3) // 文件關閉失敗

/**** 最小支持16位(待拓展) ****/

#endif /* _RETURN_CODES_C_i18n_H_ */

/**
 * 日誌：
 * 版本001：
 *      2025.8.6 發布初版
 */

#endif
