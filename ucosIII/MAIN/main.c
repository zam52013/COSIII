/*
 * File      : main.c
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
#include "stm32f10x.h"
#include "init.h"
#include "app.h"

int main(int argc,char *argv[])
{
	SystemInit();
	NVIC_Configuration();	//中断优先级设置
	 SysTickInit();
	KernelMain();
//	while(1)
//	{		
//		
//	}
//		return 0;
}

  
