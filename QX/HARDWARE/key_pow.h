/*
 * File      : key_pow.h
 * This file is ef comp
 * COPYRIGHT (C) 2017,
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at my addr
 *
 *
 * Change Logs:
 * Date           Author       ZAM
 * 2017-09-26     Bernard      the first version
 */
 #ifndef __KEY_POW_H
 #define __KEY_POW_H
 
 #include "stm32f10x.h"

typedef enum
{
	KEY_FILE=1,
	KEY_SHORT_DOWN,
	KEY_SHORT_UP,
	KEY_LONG_DOWN,
	KEY_LONG_UP,
}key_fun;
 
 #define KEY_LONG_TIME 200    //长按时间 200*10 ms
 /*********总电源按键控制端口******************/
#define ALL_POWER_KYE_PORT GPIOC
#define ALL_POWER_KYE_RCC RCC_APB2Periph_GPIOC
#define ALL_POWER_KYE_GPIO GPIO_Pin_15
#define POWER_KEY_READ GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_15)  //按键读取
/***************************/

 /*********总电源控制端口******************/
#define ALL_POWER_CONTRO_PORT GPIOC
#define ALL_POWER_RCC RCC_APB2Periph_GPIOC
#define ALL_POWER_GPIO GPIO_Pin_14
#define POWER_ON GPIO_SetBits(ALL_POWER_CONTRO_PORT,ALL_POWER_GPIO)
#define POWER_OFF GPIO_ResetBits(ALL_POWER_CONTRO_PORT,ALL_POWER_GPIO)
/***************************/
 void All_Pow_Io_Init(void);
 unsigned char Key_Scan(void);

 #endif


 
 
 
 