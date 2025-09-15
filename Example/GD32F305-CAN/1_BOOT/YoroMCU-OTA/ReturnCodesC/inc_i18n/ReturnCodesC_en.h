/*
 * Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com> 
 * All rights reserved
 */

/**
 * @file ReturnCodesC_en.h
 * 
 * Return codes for all functions, including successful operations and various types of errors.
 * These returns are uniformly considered as a kind of status, where errors are also a type of status.
 * If the function runs as expected, it returns RETURN_DEFAULT; other non-zero codes require special attention.
 * Unsigned values should be used, as signed -1 is not uniform and cannot be compatible with lower bit ranges.
 */

#if _RERURNCC_LANGUAGE_ == 0

#ifndef _RETURN_CODES_C_i18n_H_
#define _RETURN_CODES_C_i18n_H_

/**** Minimum support for 8 bits ****/

/* General returns */
#define RETURN_DEFAULT   (0x00) // Default no error
#define RETURN_ERROR     (0x01) // Undefined general error
#define RETURN_BUSY      (0x02) // General busy return
#define RETURN_TIMEOUT   (0x03) // General timeout return
#define RETURN_ERR_PARAM (0x04) // Parameter error

/* Data-related returns */
#define RETURN_DATA_ABOVE_LIMIT (0x10) // Data exceeds upper limit
#define RETURN_DATA_BELOW_LIMIT (0x11) // Data exceeds lower limit
#define RETURN_DATA_LOSS        (0x12) // Data loss
#define RETURN_DATA_INVALID     (0x13) // Invalid data

/* Buffer-related returns */
#define RETURN_BUFFER_FULL      (0x20) // Buffer full
#define RETURN_BUFFER_EMPTY     (0x21) // Buffer empty
#define RETURN_BUFFER_OVERFLOW  (0x22) // Buffer overflow
#define RETURN_BUFFER_UNDERFLOW (0x23) // Buffer underflow

/* Bit-related returns */
#define RETURN_BIT_SET   (0x30) // Bit is set
#define RETURN_BIT_UNSET (0x31) // Bit is not set

/* Physical line-related returns */
#define RETURN_CABLE_FREE    (0x40) // Line is free
#define RETURN_CABLE_BUSY    (0x41) // Line is busy
#define RETURN_CABLE_LOST    (0x42) // Line is lost
#define RETURN_CABLE_TIMEOUT (0x43) // Line timeout
#define RETURN_CABLE_ECC     (0x44) // Line ECC error

/* Time status-related returns */
#define RETURN_TIME_WAITING (0x50) // Waiting
#define RETURN_TIME_OVER    (0x51) // Timeout

/* Flash storage returns */
#define RETURN_FLASH_BUSY      (0x60) // Flash is busy
#define RETURN_FLASH_WRITE_ERR (0x61) // Flash write error
#define RETURN_FLASH_READ_ERR  (0x62) // Flash read error
#define RETURN_FLASH_ERASE_ERR (0x63) // Flash erase error

/* Controlled device status returns */
#define RETURN_STATE_OFF   (0x70) // Device is off
#define RETURN_STATE_ON    (0x71) // Device is on
#define RETURN_STATE_ERROR (0x72) // Device error
#define RETURN_STATE_BUSY  (0x73) // Device is busy

/* Mode-related returns */
#define RETURN_MODE_ENABLED     (0x80) // Mode is enabled
#define RETURN_MODE_DISABLED    (0x81) // Mode is disabled
#define RETURN_MODE_UNSUPPORTED (0x82) // Mode is unsupported

/* Memory-related returns */
#define RETURN_MEMORY_ALLOC (0x90) // Memory allocation failed
#define RETURN_MEMORY_FREE  (0x91) // Memory free failed

/* File operation-related returns */
#define RETURN_FILE_OPEN  (0xA0) // File open failed
#define RETURN_FILE_READ  (0xA1) // File read failed
#define RETURN_FILE_WRITE (0xA2) // File write failed
#define RETURN_FILE_CLOSE (0xA3) // File close failed

/**** Minimum support for 16 bits (to be expanded) ****/

#endif /* _RETURN_CODES_C_i18n_H_ */

/**
 * Log:
 * Version 001:
 *      2025.8.6 Initial release
 */

#endif
