/*
 * Copyright (C), 2024, Yuanlong Xu <Yono233@outlook.com> 
 * All rights reserved
 */

/**
 * @file ReturnCodesC.h
*/

#ifndef _RETURN_CODES_C_H_
#define _RETURN_CODES_C_H_

#define RETURN_CODES_C_Version (1)

/**
 * this macro can be used to define the language version, 
 * which will be helpful for the C/C++ semantic parsing system.
 * jump, you can browse the content conveniently in the desired language.
 *  “You can make the changes here or define it globally.”
 * 
 * 0: English
 * 1: 中文
 * 2: 正體中文
 */
#ifndef _RERURNCC_LANGUAGE_
#define _RERURNCC_LANGUAGE_ 1
#endif

/* English */
#include "inc_i18n/ReturnCodesC_en.h"
/* 简体中文 */
#include "inc_i18n/ReturnCodesC_zh_cn.h"
/* 正體中文 */
#include "inc_i18n/ReturnCodesC_zh_tw.h"

#endif