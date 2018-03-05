/*
 * File      : messg_send.c
 * This file is ef comp
 * COPYRIGHT (C) 2017,
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at my addr
 *
 *
 * Change Logs:
 * Date           Author       ZAM
 * 2018-01-8     Bernard      the first version
 */
 
 #ifndef __MESSG_SEND_H
 #define __MESSG_SEND_H
 
 #include "stm32f10x.h"
 
 #define MESSG_STX 254		//������ʼ��־��
 #define SYSID 1					//ϵͳ�汾��
 #define COMPID 1					//ϵͳ�����
 
 #define MESSAGE_USART USART2			//���ʹ���
 
 void Message_Chan_Send(USART_TypeDef* USARTx,uint8_t msgid, const char *packet,uint16_t length,uint8_t bitheart);
 #endif
 



