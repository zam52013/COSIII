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
//0,不支持ucos
//1,支持ucos
#define SYSTEM_SUPPORT_OS		1	//定义系统文件夹是否支持UCOS
																	    
//以下为汇编函数
void WFI_SET(void);		//执行WFI指令
void INTX_DISABLE(void);//关闭所有中断
void INTX_ENABLE(void);	//开启所有中断
void MSR_MSP(u32 addr);	//设置堆栈地址

#endif
