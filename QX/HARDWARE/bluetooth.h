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

/*********���ݷ��Ϳ���******************/
#define BLUETOOTH_CONTRO_SEND_PORT GPIOA
#define BLUETOOTH_CONTRO_SEND_RCC RCC_APB2Periph_GPIOA
#define BLUETOOTH_CONTRO_SEND_GPIO GPIO_Pin_0
#define BLUETOOTH_CONTRO_SEND_ON GPIO_SetBits(BLUETOOTH_CONTRO_SEND_PORT,BLUETOOTH_CONTRO_SEND_GPIO)
#define BLUETOOTH_CONTRO_SEND_OFF GPIO_ResetBits(BLUETOOTH_CONTRO_SEND_PORT,BLUETOOTH_CONTRO_SEND_GPIO)
/***************************/

/*********�ָ��������ÿ��ƶ� ����2S���ɻָ���������******************/
#define BLUETOOTH_SET_PORT GPIOC
#define BLUETOOTH_SET_RCC RCC_APB2Periph_GPIOC
#define BLUETOOTH_SET_GPIO GPIO_Pin_2
#define BLUETOOTH_SET_ON GPIO_SetBits(BLUETOOTH_SET_PORT,BLUETOOTH_SET_GPIO)
#define BLUETOOTH_SET_OFF GPIO_ResetBits(BLUETOOTH_SET_PORT,BLUETOOTH_SET_GPIO)
/***************************/

/*********͸��/������ƣ��ߵ�ƽ��ʾ͸�����ݣ��͵�ƽ��ʾ���
Ĭ�ϲ���ӣ��������Ǹߵ�ƽ******************/
#define BLUETOOTH_CMD_PORT GPIOC
#define BLUETOOTH_CMD_RCC RCC_APB2Periph_GPIOC
#define BLUETOOTH_CMD_GPIO GPIO_Pin_3
#define BLUETOOTH_CMD_ON GPIO_SetBits(BLUETOOTH_CMD_PORT,BLUETOOTH_CMD_GPIO)
#define BLUETOOTH_CMD_OFF GPIO_ResetBits(BLUETOOTH_CMD_PORT,BLUETOOTH_CMD_GPIO)
/***************************/

/*********ģ�����ⲿ��������ǰ 800us�����͸����ţ��������ݹ����б�
�ֲ��䣬���ݷ�����ϣ�����(��ģ�����)******************/
#define BLUETOOTH_CTS_PORT GPIOA
#define BLUETOOTH_CTS_RCC RCC_APB2Periph_GPIOA
#define BLUETOOTH_CTS_GPIO GPIO_Pin_1
/***************************/

void Ble_Bluetooth_Init(void);
void Send_DataViaBt(char *buf,int len);
void Send_Reset(void);
void SendCmdViaBt(char *buf,int len);
#endif

