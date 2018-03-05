/*
 * File      : sys.h
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
#ifndef __SYS_H
#define __SYS_H	
#include "stm32f10x.h"
//0,��֧��ucos
//1,֧��ucos
#define SYSTEM_SUPPORT_OS		1	//����ϵͳ�ļ����Ƿ�֧��UCOS
																	    
//����Ϊ��ຯ��
void WFI_SET(void);		//ִ��WFIָ��
void INTX_DISABLE(void);//�ر������ж�
void INTX_ENABLE(void);	//���������ж�
void MSR_MSP(u32 addr);	//���ö�ջ��ַ

#endif
