/*
 * File      : led.h
 * This file is ef comp
 * COPYRIGHT (C) 2017,
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 *
 *
 * Change Logs:
 * Date           Author       ZAM
 * 2017-09-26     Bernard      the first version
 */


#ifndef __LED_H_
#define __LED_H_

#include "stm32f10x.h"


#define LED0_PORT GPIOA
#define LED0_RCC RCC_APB2Periph_GPIOA
#define LED0_IO GPIO_Pin_8

#define LED0_ON GPIO_SetBits(LED0_PORT,LED0_IO)
#define LED0_OFF GPIO_ResetBits(LED0_PORT,LED0_IO)

void LED_Init(void);//≥ı ºªØ

		 				    
#endif
