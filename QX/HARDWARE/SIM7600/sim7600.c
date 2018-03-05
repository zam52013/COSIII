/*
 * File      : sim7600.c
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
 
 #include "sim7600.h"
 #include "usart.h"

 void Sim_contro(void)
 {
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(SIM_POWER_RCC,ENABLE);
	  RCC_APB2PeriphClockCmd(SIM_FLIGHTMODE_RCC,ENABLE);
	 	RCC_APB2PeriphClockCmd(SIM_POWKEY_RCC,ENABLE);
	 
		GPIO_InitStructure.GPIO_Pin = SIM_POWER_GPIO;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(SIM_POWER_CONTRO_PORT, &GPIO_InitStructure);
		SIM_POWER_ON;
	 
	  GPIO_InitStructure.GPIO_Pin = SIM_FLIGHTMODE_GPIO;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(SIM_FLIGHTMODE_PORT, &GPIO_InitStructure);
		SIM_FLIGHTMODE_OFF;

		GPIO_InitStructure.GPIO_Pin = SIM_POWKEY_GPIO;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(SIM_POWKEY_PORT, &GPIO_InitStructure);
		SIM_POWKEY_ON;
		#ifdef DEBUG_PERPH
		printf("sim7600 open!\r\n");
		#endif
// 		SIM_POWKEY_OFF;
 }
 
