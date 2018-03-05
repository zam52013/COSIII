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
#include "bluetooth.h"
#include "delay.h"
#include "usart.h"
#include "main.h"
#ifdef SYSTEM_SUPPORT_OS_II
#include "includes.h"					//ucos 使用	  
#endif

/*******************************************************************************
函 数 名：void Ble_Bluetooth_Init(void)
功能描述： 蓝牙初始化端口
入口参数：							
返回参数：
创建时间: 2017-11-02 by zam
********************************************************************************/
void Ble_Bluetooth_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(BLUETOOTH_CONTRO_SEND_RCC | BLUETOOTH_SET_RCC | BLUETOOTH_CMD_RCC |BLUETOOTH_CTS_RCC,ENABLE);
	GPIO_InitStructure.GPIO_Pin=BLUETOOTH_CONTRO_SEND_GPIO;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(BLUETOOTH_CONTRO_SEND_PORT,&GPIO_InitStructure);
	BLUETOOTH_CONTRO_SEND_OFF;//随时准备发送数据

	GPIO_InitStructure.GPIO_Pin=BLUETOOTH_SET_GPIO;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(BLUETOOTH_SET_PORT,&GPIO_InitStructure);
	BLUETOOTH_SET_ON; //设置为高

	GPIO_InitStructure.GPIO_Pin=BLUETOOTH_CMD_GPIO;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(BLUETOOTH_CMD_PORT,&GPIO_InitStructure);
	BLUETOOTH_CMD_ON; //设置为高 数据传输

	GPIO_InitStructure.GPIO_Pin=BLUETOOTH_CTS_GPIO;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init(BLUETOOTH_CTS_PORT,&GPIO_InitStructure);
	#ifdef DEBUG_PERPH
	printf("bluetooth open!\r\n");
	#endif
}
/*******************************************************************************
函 数 名：void Send_DataViaBt(char *buf,int len)
功能描述：发送数据
入口参数：	buf 数据   len:长度						
返回参数：
创建时间: 2017-11-02 by zam
********************************************************************************/
void Send_DataViaBt(char *buf,int len)
{
	len++;
	while(len--)
	{
		USART_SendData(BLUETOOTH_USART,*buf);
		while(USART_GetFlagStatus(BLUETOOTH_USART, USART_FLAG_TC) == RESET);
		buf++;
	}
}
/*******************************************************************************
函 数 名：void Sendble_Str(char *buf)
功能描述：发送数据
入口参数：	buf 数据 				
返回参数：
创建时间: 2017-11-02 by zam
********************************************************************************/
void Sendble_Str(char *buf)
{
	while(*buf!='\0')
	{
		USART_SendData(BLUETOOTH_USART,*buf);
		while(USART_GetFlagStatus(BLUETOOTH_USART, USART_FLAG_TC) == RESET);
		buf++;
	}
}
/*******************************************************************************
函 数 名：void Send_Reset(char *buf)
功能描述：模块复位控制
入口参数：				
返回参数：
创建时间: 2017-11-02 by zam
********************************************************************************/
const unsigned char reset_buletooth[]={0xfa,0x08,0x00,0xaa};
void Send_Reset()
{
	unsigned char i;
	BLUETOOTH_CMD_OFF;
	delay_us(600);
	for(i=0;i<4;i++)
	{
		USART_SendData(BLUETOOTH_USART,reset_buletooth[i]);
		while(USART_GetFlagStatus(BLUETOOTH_USART, USART_FLAG_TC) == RESET);	
	}
	delay_us(600);
	BLUETOOTH_CMD_ON;
}
/*******************************************************************************
函 数 名：void SendCmdViaBt(char *buf)
功能描述：蓝牙名字修改
入口参数：				
返回参数：
创建时间: 2017-11-02 by zam
********************************************************************************/
const unsigned char name_buff[10]={0xfa,0x06,0x0c,0x41,0x47,0x31,0x53,0x32,0x31,0x37};
void SendCmdViaBt(char *buf,int len)
{
	unsigned char i;
	BLUETOOTH_CMD_OFF;
	delay_us(600);
	for(i=0;i<10;i++)
	{
		USART_SendData(BLUETOOTH_USART,name_buff[i]);
		while(USART_GetFlagStatus(BLUETOOTH_USART, USART_FLAG_TC) == RESET);
	}
	while(len--)
	{
		USART_SendData(BLUETOOTH_USART,*buf);
		while(USART_GetFlagStatus(BLUETOOTH_USART, USART_FLAG_TC) == RESET);
		buf++;
	}
	USART_SendData(BLUETOOTH_USART,0xaa);
	while(USART_GetFlagStatus(BLUETOOTH_USART, USART_FLAG_TC) == RESET);
	delay_us(600);
	BLUETOOTH_CMD_ON;
}


void USART2_IRQHandler(void)
{
	#ifndef USINGE_USART2_DMA_R
		u8 RES;
		static unsigned char statu=0;
	#else
		u8 RES;
	#endif
	#ifdef SYSTEM_SUPPORT_OS_II
		OSIntEnter();
	#endif
	#ifdef USINGE_USART2_DMA_R
		if(!BLUETOOTH_MESSG.receve_ok)
		{
			if(USART_GetITStatus(BLUETOOTH_USART, USART_IT_IDLE)!=RESET)
			{
				DMA_Cmd(DMA1_Channel6,DISABLE);
				BLUETOOTH_MESSG.date_lenth=Usart_size-DMA_GetCurrDataCounter(DMA1_Channel6);
				memcpy(&BLUETOOTH_MESSG.messg_buff[0],&BLUETOOTH_MESSG[0],BLUETOOTH_MESSG.date_lenth);
				BLUETOOTH_MESSG.receve_ok=TRUE;
				DMA_ClearFlag(DMA1_FLAG_GL6 | DMA1_FLAG_TC6 | DMA1_FLAG_TE6 | DMA1_FLAG_HT6);
				DMA1_Channel6->CNDTR=Usart_size;
				DMA_Cmd(DMA1_Channel6,ENABLE);
				RES=BLUETOOTH_USART->SR;
				RES=BLUETOOTH_USART->DR;
			}
		}
		RES=BLUETOOTH_USART->SR;
		RES=BLUETOOTH_USART->DR;
	#else
		if(USART_GetITStatus(BLUETOOTH_USART, USART_IT_RXNE)!=RESET)
		{
			RES =USART_ReceiveData(BLUETOOTH_USART);
			if(!BLUETOOTH_MESSG.receve_ok)
			{
				switch(statu)
				{
					case 0:
						memset(&BLUETOOTH_MESSG.messg_buff[0],0,Usart_size); 
						BLUETOOTH_MESSG.date_lenth=0;
						statu=1;
						BLUETOOTH_MESSG.messg_buff[BLUETOOTH_MESSG.date_lenth]=RES;
						BLUETOOTH_MESSG.date_lenth++;
						break;
					case 1:
						if(RES==(0x0D))
						{
							statu=2;
							BLUETOOTH_MESSG.messg_buff[BLUETOOTH_MESSG.date_lenth]=RES;
							BLUETOOTH_MESSG.date_lenth++;
						}
						else
						{
							BLUETOOTH_MESSG.messg_buff[BLUETOOTH_MESSG.date_lenth]=RES;
							BLUETOOTH_MESSG.date_lenth++;
						}
						if(BLUETOOTH_MESSG.date_lenth>Usart_size)
						{
							statu=0;
						}
						break;
					case 2:
						if(RES==(0x0A))
						{
							statu=0;
							BLUETOOTH_MESSG.receve_ok=TRUE;
							BLUETOOTH_MESSG.messg_buff[BLUETOOTH_MESSG.date_lenth]=RES;
							BLUETOOTH_MESSG.date_lenth++;
							OSSemPost(USART2_Semp);  //发送信号量
						}
						else
						{
							statu=0;
						}
						break;
					default:
						statu=0;
				}
			}
		}
	#endif
	#ifdef SYSTEM_SUPPORT_OS_II
		OSIntExit();
	#endif
}




