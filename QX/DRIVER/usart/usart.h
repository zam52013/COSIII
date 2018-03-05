/*
 * File      : usart.h
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
#ifndef __USART_H
#define __USART_H

#include "stm32f10x.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define SYSTEM_SUPPORT_OS_II		//֧�ֲ���ϵͳ

#define TRUE 1
#define FALSE 0


/******���ڶ���********/
#define USINGE_USART1
#define USINGE_USART2
#define USINGE_USART3
//#define USINGE_UART4
//#define USINGE_UART5
/***************************/


/************������Զ˿�****************/
#define DEBUG_PERPH
#define DEBUG_UART USART3
/***************************************/

/************���巢�͵��װ�Ĵ���****************/
#define CONNECT_TO_NOVAL USART3
/***************************************/

/******DMAʹ��********/
//#define USINGE_USART1_DMA_T
//#define USINGE_USART1_DMA_R

//#define USINGE_USART2_DMA_T
//#define USINGE_USART2_DMA_R

//#define USINGE_USART3_DMA_T
//#define USINGE_USART3_DMA_R

//#define USINGE_UART4_DMA_T
//#define USINGE_UART4_DMA_R
/***************************/


/******�����С********/
#define Usart_size 512
/***************************/

/******������********/
#define BAUD_RATE_2400                  2400
#define BAUD_RATE_4800                  4800
#define BAUD_RATE_9600                  9600
#define BAUD_RATE_38400                 38400
#define BAUD_RATE_57600                 57600
#define BAUD_RATE_115200                115200
#define BAUD_RATE_230400                230400
#define BAUD_RATE_460800                460800
#define BAUD_RATE_921600                921600
/***************************/

/******���ڳ�ʼ������********/
#define SERIAL_CONFIG_1														\
{																									\
	BAUD_RATE_115200,	/* 115200 bits/s */						\
	USART_WordLength_8b,/* 8 databits */ 						\
	USART_StopBits_1,		/* 1 stopbit */							\
	USART_Parity_No,		/* No parity  */						\
	USART_Mode_Rx|USART_Mode_Tx, /* Normal mode */	\
	USART_HardwareFlowControl_None, /* NO hardflow*/\
	USART_IT_RXNE,			/* interrupt mode*/					\
	USART1_IRQn,			/* interrupt usart1*/					\
	3,					/* main interrupt */								\
	1					/* sub interrupt */										\
}
#define SERIAL_CONFIG_DMA1												\
{																									\
	0,					/* dma R */													\
	0,					/* dma R main interrupt */					\
	0,					/* dma R sub interrupt */						\
	0,					/* dma T */													\
	0,					/* dma T main interrupt */					\
	0					/* dma T sub interrupt */							\
}
#define SERIAL_CONFIG_2														\
{																									\
	BAUD_RATE_9600,	/* 115200 bits/s */						\
	USART_WordLength_8b,/* 8 databits */ 						\
	USART_StopBits_1,		/* 1 stopbit */							\
	USART_Parity_No,		/* No parity  */						\
	USART_Mode_Rx|USART_Mode_Tx, /* Normal mode */	\
	USART_HardwareFlowControl_None, /* NO hardflow*/\
	USART_IT_RXNE,			/* interrupt mode*/					\
	USART2_IRQn,			/* interrupt usart1*/					\
	4,					/* main interrupt */								\
	1					/* sub interrupt */										\
}
#define SERIAL_CONFIG_DMA2												\
{																									\
	0,					/* dma R */													\
	0,					/* dma R main interrupt */					\
	0,					/* dma R sub interrupt */						\
	0,					/* dma T */													\
	0,					/* dma T main interrupt */					\
	0					/* dma T sub interrupt */							\
}

#define SERIAL_CONFIG_3														\
{																									\
	BAUD_RATE_115200,	/* 115200 bits/s */						\
	USART_WordLength_8b,/* 8 databits */ 						\
	USART_StopBits_1,		/* 1 stopbit */							\
	USART_Parity_No,		/* No parity  */						\
	USART_Mode_Rx|USART_Mode_Tx, /* Normal mode */	\
	USART_HardwareFlowControl_None, /* NO hardflow*/\
	USART_IT_RXNE,			/* interrupt mode*/					\
	USART3_IRQn,			/* interrupt usart1*/					\
	5,					/* main interrupt */								\
	1					/* sub interrupt */										\
}
#define SERIAL_CONFIG_DMA3												\
{																									\
	1,							/* dma R */											\
	8,					/* dma R main interrupt */					\
	0,					/* dma R sub interrupt */						\
	1,					/* dma T */													\
	9,					/* dma T main interrupt */					\
	0					/* dma T sub interrupt */							\
}

#define SERIAL_CONFIG_4														\
{																									\
	BAUD_RATE_115200,	/* 115200 bits/s */						\
	USART_WordLength_8b,/* 8 databits */ 						\
	USART_StopBits_1,		/* 1 stopbit */							\
	USART_Parity_No,		/* No parity  */						\
	USART_Mode_Rx|USART_Mode_Tx, /* Normal mode */	\
	USART_HardwareFlowControl_None, /* NO hardflow*/\
	USART_IT_RXNE,			/* interrupt mode*/					\
	0,			/* interrupt usart1*/										\
	0,					/* main interrupt */								\
	0					/* sub interrupt */										\
}
#define SERIAL_CONFIG_DMA4				\
{				\
	0,					/* dma R */								\
	0,					/* dma R main interrupt */					\
	0,					/* dma R sub interrupt */					\
	0,					/* dma T */								\
	0,					/* dma T main interrupt */					\
	0					/* dma T sub interrupt */					\
}

#define SERIAL_CONFIG_5														\
{																									\
	BAUD_RATE_115200,	/* 115200 bits/s */						\
	USART_WordLength_8b,/* 8 databits */ 						\
	USART_StopBits_1,		/* 1 stopbit */							\
	USART_Parity_No,		/* No parity  */						\
	USART_Mode_Rx|USART_Mode_Tx, /* Normal mode */	\
	USART_HardwareFlowControl_None, /* NO hardflow*/\
	USART_IT_RXNE,			/* interrupt mode*/					\
	0,			/* interrupt usart1*/										\
	0,					/* main interrupt */								\
	0					/* sub interrupt */										\
}
#define SERIAL_CONFIG_DMA5				\
{				\
	0,					/* dma R */								\
	0,					/* dma R main interrupt */					\
	0,					/* dma R sub interrupt */					\
	0,					/* dma T */								\
	0,					/* dma T main interrupt */					\
	0					/* dma T sub interrupt */					\
}
/***************************/

struct serial_configure
{
    uint32_t baud_rate;					/*****������****/
    uint16_t DATA_BITS;				/*****����λ****/
    uint16_t STOP_BITS;				/*****ֹͣλ****/
    uint16_t parity;				/*****У��λ****/
    uint16_t Mode;					/*****ģʽ****/
		uint16_t Hard_Flow;			/*****Ӳ����****/
		uint16_t Interrupt_mode;	/*****�ж�****/
		uint8_t Nvic_Irq;			/*****�ж����ȼ�****/
		uint8_t Nvic_pree;			/*****�ж������ȼ�****/
		uint8_t Nvic_Subpri;		/*****�жϴ����ȼ�****/
};
struct dma_configure
{
	uint8_t Dma_R;			/*****DMA���ձ�־****/
	uint8_t Dma_Rpree;		/*****DMA���������ȼ�****/
	uint8_t Dma_Rsubpri;	/*****DMA���մ����ȼ�****/
	uint8_t Dma_T;			/*****DMA���ͱ�־****/
	uint8_t Dma_Tpree;		/*****DMA���������ȼ�****/
	uint8_t Dma_Tsubpri;	/*****DMA���ʹ����ȼ�****/
};
struct usart_messg
{
	uint16_t date_lenth;			/*****�������ݳ���****/
	char messg_buff[Usart_size];	/****�������ݻ���*****/
	uint8_t receve_ok;				/*****������ɱ�־****/
};

enum
{
	USE_USART1=0,		/*****printʹ�ô���****/
	USE_USART2,
	USE_USART3,
	USE_UART4,
	USE_UART5,
};
extern unsigned char USE_USART;
#ifdef USINGE_USART1_DMA_R
	 extern char USART1_RECEIVE_DATA[Usart_size];
#endif
#ifdef USINGE_USART1_DMA_T
	 extern char USART1_SEND_DATA[Usart_size];
#endif
#ifdef USINGE_USART2_DMA_R
	 extern char USART2_RECEIVE_DATA[Usart_size];
#endif
#ifdef USINGE_USART2_DMA_T
	 extern char USART2_SEND_DATA[Usart_size];
#endif
#ifdef USINGE_USART3_DMA_R
	 extern char USART3_RECEIVE_DATA[Usart_size];
#endif
#ifdef USINGE_USART3_DMA_T
	 extern char USART3_SEND_DATA[Usart_size];
#endif
#ifdef USINGE_UART4_DMA_R
	 extern char UART4_RECEIVE_DATA[Usart_size];
#endif
#ifdef USINGE_UART4_DMA_T
	 extern char UART4_SEND_DATA[Usart_size];
#endif


#ifdef USINGE_USART1
	extern struct usart_messg USART1_MESSG;
#endif
#ifdef USINGE_USART2
	extern struct usart_messg USART2_MESSG;
#endif
#ifdef USINGE_USART3
	extern struct usart_messg USART3_MESSG;
#endif
#ifdef USINGE_UART4
	extern struct usart_messg UART4_MESSG;
#endif
#ifdef USINGE_UART5
	extern struct usart_messg UART5_MESSG;
#endif



void Usart_init(void);
void UART_SendString(USART_TypeDef* USARTx,char* s);
#endif



