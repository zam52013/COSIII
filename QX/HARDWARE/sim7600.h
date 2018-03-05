/*
 * File      : sim7600.h
 * This file is ef comp
 * COPYRIGHT (C) 2017,
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at my addr
 *
 *
 * Change Logs:
 * Date           Author       ZAM
 * 2017-10-18     Bernard      the first version
 */
 
 #ifndef __SIM7600_H
 #define __SIM7600_H
 
 #include "stm32f10x.h"
 #include "usart.h"

/************GSM串口******************/
 #define GSM_USART USART1
/************************************/

/*********电源控制******************/
#define SIM_POWER_CONTRO_PORT GPIOC
#define SIM_POWER_RCC RCC_APB2Periph_GPIOC
#define SIM_POWER_GPIO GPIO_Pin_7
#define SIM_POWER_ON GPIO_SetBits(SIM_POWER_CONTRO_PORT,SIM_POWER_GPIO)
#define SIM_POWER_OFF GPIO_ResetBits(SIM_POWER_CONTRO_PORT,SIM_POWER_GPIO)
/***************************/

/*********飞行模式控制******************/
#define SIM_FLIGHTMODE_PORT GPIOC
#define SIM_FLIGHTMODE_RCC RCC_APB2Periph_GPIOC
#define SIM_FLIGHTMODE_GPIO GPIO_Pin_4
#define SIM_FLIGHTMODE_ON GPIO_ResetBits(SIM_FLIGHTMODE_PORT,SIM_FLIGHTMODE_GPIO)
#define SIM_FLIGHTMODE_OFF GPIO_SetBits(SIM_FLIGHTMODE_PORT,SIM_FLIGHTMODE_GPIO)
/***************************/

/*********开机模式控制******************/
#define SIM_POWKEY_PORT GPIOC
#define SIM_POWKEY_RCC RCC_APB2Periph_GPIOC
#define SIM_POWKEY_GPIO GPIO_Pin_8
#define SIM_POWKEY_ON GPIO_SetBits(SIM_POWKEY_PORT,SIM_POWKEY_GPIO)
#define SIM_POWKEY_OFF GPIO_ResetBits(SIM_POWKEY_PORT,SIM_POWKEY_GPIO)
/***************************/

void Sim_contro(void);
#endif
 
 
 
