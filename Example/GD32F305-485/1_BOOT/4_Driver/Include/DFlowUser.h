/**
 * DFlowUser.h
 *
 *  Created on: 2024年3月13日
 *      Author: yono
 *
*/
/**
 *
 * 用于UsartStdio的回调绑定函数，用户定义
*/

#ifndef _DFLOW_USER_
#define _DFLOW_USER_

/* Includes ------------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

/* Exported variables ---------------------------------------------------------*/
extern _DFlow DFlow;
/* Exported functions ---------------------------------------------------------*/

extern void DFlow_User_Init(void);

extern void SendBefor485(void);
extern void SendOver485(void);

#endif /* _DFLOW_USER_ */
