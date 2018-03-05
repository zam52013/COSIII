/*
 * File      : bluetooth.h
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
#ifndef __BLUETOOTH_H_
#define __BLUETOOTH_H_

#include "stm32f10x.h"
#include "usart.h"

/****************************************/
#define BLUETOOTH_USART USART2
#define BLUETOOTH_MESSG USART2_MESSG
/****************************************/

/*********数据发送控制******************/
#define BLUETOOTH_CONTRO_SEND_PORT GPIOA
#define BLUETOOTH_CONTRO_SEND_RCC RCC_APB2Periph_GPIOA
#define BLUETOOTH_CONTRO_SEND_GPIO GPIO_Pin_0
#define BLUETOOTH_CONTRO_SEND_ON GPIO_SetBits(BLUETOOTH_CONTRO_SEND_PORT,BLUETOOTH_CONTRO_SEND_GPIO)
#define BLUETOOTH_CONTRO_SEND_OFF GPIO_ResetBits(BLUETOOTH_CONTRO_SEND_PORT,BLUETOOTH_CONTRO_SEND_GPIO)
/***************************/

/*********恢复出厂设置控制端 拉低2S即可恢复出厂设置******************/
#define BLUETOOTH_SET_PORT GPIOC
#define BLUETOOTH_SET_RCC RCC_APB2Periph_GPIOC
#define BLUETOOTH_SET_GPIO GPIO_Pin_2
#define BLUETOOTH_SET_ON GPIO_SetBits(BLUETOOTH_SET_PORT,BLUETOOTH_SET_GPIO)
#define BLUETOOTH_SET_OFF GPIO_ResetBits(BLUETOOTH_SET_PORT,BLUETOOTH_SET_GPIO)
/***************************/

/*********透传/命令控制，高电平表示透传数据，低电平表示命令；
默认不外接，该引脚是高电平******************/
#define BLUETOOTH_CMD_PORT GPIOC
#define BLUETOOTH_CMD_RCC RCC_APB2Periph_GPIOC
#define BLUETOOTH_CMD_GPIO GPIO_Pin_3
#define BLUETOOTH_CMD_ON GPIO_SetBits(BLUETOOTH_CMD_PORT,BLUETOOTH_CMD_GPIO)
#define BLUETOOTH_CMD_OFF GPIO_ResetBits(BLUETOOTH_CMD_PORT,BLUETOOTH_CMD_GPIO)
/***************************/

/*********模块向外部发送数据前 800us，拉低该引脚，发送数据过程中保
持不变，数据发送完毕，拉高(由模块控制)******************/
#define BLUETOOTH_CTS_PORT GPIOA
#define BLUETOOTH_CTS_RCC RCC_APB2Periph_GPIOA
#define BLUETOOTH_CTS_GPIO GPIO_Pin_1
/***************************/

void Ble_Bluetooth_Init(void);
void Send_DataViaBt(char *buf,int len);
void Send_Reset(void);
void SendCmdViaBt(char *buf,int len);
#endif

