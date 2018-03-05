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

#include "led.h"
#include "usart.h"
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(LED_POW1_RCC | LED_POW2_RCC | LED_STATU_RCC, ENABLE);	 
	
 GPIO_InitStructure.GPIO_Pin = LED_POW1_GPIO;				
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
 GPIO_Init(LED_POW1_PORT, &GPIO_InitStructure);	
 LED_PWO1_OFF;

 GPIO_InitStructure.GPIO_Pin = LED_POW2_GPIO;				
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
 GPIO_Init(LED_POW2_PORT, &GPIO_InitStructure);	
 LED_PWO2_OFF;

 GPIO_InitStructure.GPIO_Pin = LED_STATU_GPIO;				
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
 GPIO_Init(LED_STATU_PORT, &GPIO_InitStructure);
 LED_STATU_OFF;
 #ifdef DEBUG_PERPH
 printf("led open!\r\n");
 #endif
 
}
