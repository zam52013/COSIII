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

#include "usart.h"
#include <string.h>
#ifdef SYSTEM_SUPPORT_OS_II
#include "includes.h"					//ucos ʹ��	  
#endif
#include "bluetooth.h"
#include "main.h"

#ifdef USINGE_USART1_DMA_R
	 char USART1_RECEIVE_DATA[Usart_size];
#endif
#ifdef USINGE_USART1_DMA_T
	 char USART1_SEND_DATA[Usart_size];
#endif
#ifdef USINGE_USART2_DMA_R
	 char USART2_RECEIVE_DATA[Usart_size];
#endif
#ifdef USINGE_USART2_DMA_T
	 char USART2_SEND_DATA[Usart_size];
#endif
#ifdef USINGE_USART3_DMA_R
	 char USART3_RECEIVE_DATA[Usart_size];
#endif
#ifdef USINGE_USART3_DMA_T
	 char USART3_SEND_DATA[Usart_size];
#endif
#ifdef USINGE_UART4_DMA_R
	 char UART4_RECEIVE_DATA[Usart_size];
#endif
#ifdef USINGE_UART4_DMA_T
	 char UART4_SEND_DATA[Usart_size];
#endif

#ifdef USINGE_USART1
	struct usart_messg USART1_MESSG;
#endif
#ifdef USINGE_USART2
	struct usart_messg USART2_MESSG;
#endif
#ifdef USINGE_USART3
	struct usart_messg USART3_MESSG;
#endif
#ifdef USINGE_UART4
	struct usart_messg UART4_MESSG;
#endif
#ifdef USINGE_UART5
	struct usart_messg UART5_MESSG;
#endif


#if 1

#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{     
	if(!Task_Flag.Flag_Bit.Debug_Date)
	{
		while((DEBUG_UART->SR&0X40)==0);//ѭ������,ֱ���������   
    	DEBUG_UART->DR = (u8) ch;      
	}
	else
	{
		while((BLUETOOTH_USART->SR&0X40)==0);//ѭ������,ֱ���������   
    	BLUETOOTH_USART->DR = (u8) ch;     
	}
	return ch;
}
#endif

static void Usat_rcc_init(void)
{
	#ifdef USINGE_USART1
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1,ENABLE);
	#endif
	#ifdef USINGE_USART2
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	#endif
	#ifdef USINGE_USART3
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	#endif
	#ifdef USINGE_UART4
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
	#endif
	#ifdef USINGE_UART5
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD,ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);
	#endif
	#if (defined USINGE_USART1_DMA_R) || (defined USINGE_USART1_DMA_T) \
		||(defined USINGE_USART2_DMA_R) ||(defined USINGE_USART2_DMA_T) \
		|| (defined USINGE_USART3_DMA_R) || (defined USINGE_USART3_DMA_T)
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	#endif
	#if (defined USINGE_UART4_DMA_T) || (defined USINGE_UART4_DMA_R)
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2,ENABLE);
	#endif
}
static void Usart_io_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	#ifdef USINGE_USART1
		struct serial_configure config1 = SERIAL_CONFIG_1;
	#endif
	#ifdef USINGE_USART2
		struct serial_configure config2 = SERIAL_CONFIG_2;
	#endif
	#ifdef USINGE_USART3
		struct serial_configure config3 = SERIAL_CONFIG_3;
	#endif
	#ifdef USINGE_UART4
		struct serial_configure config4 = SERIAL_CONFIG_4;
	#endif
	#ifdef USINGE_UART5
		struct serial_configure config5 = SERIAL_CONFIG_5;
	#endif
	#ifdef USINGE_USART1
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;//���ڷ�������
		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//�����������
		GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//�ܽ�����
		GPIO_Init(GPIOA,&GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;//���ڽ�������
		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;//��������
		GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//�ܽ�����
		GPIO_Init(GPIOA,&GPIO_InitStructure);
		NVIC_InitStructure.NVIC_IRQChannel=config1.Nvic_Irq;//�����ж�
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=config1.Nvic_pree;//�����ȼ�
		NVIC_InitStructure.NVIC_IRQChannelSubPriority=config1.Nvic_Subpri;//�����ȼ�
		NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;//ʹ��
		NVIC_Init(&NVIC_InitStructure);
		USART_InitStructure.USART_BaudRate=config1.baud_rate;//����������
		USART_InitStructure.USART_WordLength=config1.DATA_BITS;
		USART_InitStructure.USART_StopBits=config1.STOP_BITS;
		USART_InitStructure.USART_Parity=config1.parity;
		USART_InitStructure.USART_Mode=config1.Mode;
		USART_InitStructure.USART_HardwareFlowControl=config1.Hard_Flow;
		USART_Init(USART1,&USART_InitStructure);//��ʼ������
		USART_ITConfig(USART1,config1.Interrupt_mode,ENABLE);//�����ڽ����ж�
		USART_Cmd(USART1,ENABLE);//ʹ������
	#endif
	#ifdef USINGE_USART2
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;//���ڷ�������
		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//�����������
		GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//�ܽ�����
		GPIO_Init(GPIOA,&GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;//���ڽ�������
		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;//��������
		GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//�ܽ�����
		GPIO_Init(GPIOA,&GPIO_InitStructure);
		NVIC_InitStructure.NVIC_IRQChannel=config2.Nvic_Irq;//�����ж�
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=config2.Nvic_pree;//�����ȼ�
		NVIC_InitStructure.NVIC_IRQChannelSubPriority=config2.Nvic_Subpri;//�����ȼ�
		NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;//ʹ��
		NVIC_Init(&NVIC_InitStructure);
		USART_InitStructure.USART_BaudRate=config2.baud_rate;//����������
		USART_InitStructure.USART_WordLength=config2.DATA_BITS;
		USART_InitStructure.USART_StopBits=config2.STOP_BITS;
		USART_InitStructure.USART_Parity=config2.parity;
		USART_InitStructure.USART_Mode=config2.Mode;
		USART_InitStructure.USART_HardwareFlowControl=config2.Hard_Flow;
		USART_Init(USART2,&USART_InitStructure);//��ʼ������
		USART_ITConfig(USART2,config2.Interrupt_mode,ENABLE);//�����ڽ����ж�
		USART_Cmd(USART2,ENABLE);//ʹ������
	#endif
	#ifdef USINGE_USART3
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;//���ڷ�������
		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//�����������
		GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//�ܽ�����
		GPIO_Init(GPIOB,&GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;//���ڽ�������
		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;//��������
		GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//�ܽ�����
		GPIO_Init(GPIOB,&GPIO_InitStructure);
		NVIC_InitStructure.NVIC_IRQChannel=config3.Nvic_Irq;//�����ж�
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=config3.Nvic_pree;//�����ȼ�
		NVIC_InitStructure.NVIC_IRQChannelSubPriority=config3.Nvic_Subpri;//�����ȼ�
		NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;//ʹ��
		NVIC_Init(&NVIC_InitStructure);
		USART_InitStructure.USART_BaudRate=config3.baud_rate;//����������
		USART_InitStructure.USART_WordLength=config3.DATA_BITS;
		USART_InitStructure.USART_StopBits=config3.STOP_BITS;
		USART_InitStructure.USART_Parity=config3.parity;
		USART_InitStructure.USART_Mode=config3.Mode;
		USART_InitStructure.USART_HardwareFlowControl=config3.Hard_Flow;
		USART_Init(USART3,&USART_InitStructure);//��ʼ������
		USART_ITConfig(USART3,config3.Interrupt_mode,ENABLE);//�����ڽ����ж�
		USART_Cmd(USART3,ENABLE);//ʹ������
	#endif
	#ifdef USINGE_UART4
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;//���ڷ�������
		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//�����������
		GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//�ܽ�����
		GPIO_Init(GPIOC,&GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;//���ڽ�������
		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;//��������
		GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//�ܽ�����
		GPIO_Init(GPIOC,&GPIO_InitStructure);
		if(config5.Nvic_Irq==52)
		{
			NVIC_InitStructure.NVIC_IRQChannel=config4.Nvic_Irq;//�����ж�
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=config4.Nvic_pree;//�����ȼ�
			NVIC_InitStructure.NVIC_IRQChannelSubPriority=config4.Nvic_Subpri;//�����ȼ�
			NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;//ʹ��
			NVIC_Init(&NVIC_InitStructure);
		}
		USART_InitStructure.USART_BaudRate=config4.baud_rate;//����������
		USART_InitStructure.USART_WordLength=config4.DATA_BITS;
		USART_InitStructure.USART_StopBits=config4.STOP_BITS;
		USART_InitStructure.USART_Parity=config4.parity;
		USART_InitStructure.USART_Mode=config4.Mode;
		USART_InitStructure.USART_HardwareFlowControl=config4.Hard_Flow;
		USART_Init(UART4,&USART_InitStructure);//��ʼ������; 
		USART_ITConfig(UART4,config4.Interrupt_mode,ENABLE);//�����ڽ����ж�
		USART_Cmd(UART4,ENABLE);//ʹ������
	#endif
	#ifdef USINGE_UART5
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;//���ڷ�������
		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//�����������
		GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//�ܽ�����
		GPIO_Init(GPIOC,&GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;//���ڽ�������
		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;//��������
		GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//�ܽ�����
		GPIO_Init(GPIOD,&GPIO_InitStructure);
		if(config5.Nvic_Irq==53)
		{
			NVIC_InitStructure.NVIC_IRQChannel=config5.Nvic_Irq;//�����ж�
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=config5.Nvic_pree;//�����ȼ�
			NVIC_InitStructure.NVIC_IRQChannelSubPriority=config5.Nvic_Subpri;//�����ȼ�
			NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;//ʹ��
			NVIC_Init(&NVIC_InitStructure);
		}
		USART_InitStructure.USART_BaudRate=config5.baud_rate;//����������
		USART_InitStructure.USART_WordLength=config5.DATA_BITS;
		USART_InitStructure.USART_StopBits=config5.STOP_BITS;
		USART_InitStructure.USART_Parity=config5.parity;
		USART_InitStructure.USART_Mode=config5.Mode;
		USART_InitStructure.USART_HardwareFlowControl=config5.Hard_Flow;
		USART_Init(UART5,&USART_InitStructure);//��ʼ������; 
		USART_ITConfig(UART5,config5.Interrupt_mode,ENABLE);//�����ڽ����ж�
		USART_Cmd(UART5,ENABLE);//ʹ������
	#endif
}
static void Dma_init(void)
{
	DMA_InitTypeDef DMA_InitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;
 	#if (defined USINGE_USART1_DMA_R) || (defined USINGE_USART1_DMA_T)
		struct dma_configure config1=SERIAL_CONFIG_DMA1;
	#endif
	#if (defined USINGE_USART2_DMA_R) || (defined USINGE_USART2_DMA_T)
		struct dma_configure config2=SERIAL_CONFIG_DMA2;
	#endif
	#if (defined USINGE_USART3_DMA_R) || (defined USINGE_USART3_DMA_T)
		struct dma_configure config3=SERIAL_CONFIG_DMA3;
	#endif
	#if (defined USINGE_UART4_DMA_R) || (defined USINGE_UART4_DMA_T)
		struct dma_configure config4=SERIAL_CONFIG_DMA4;
	#endif
	
	#ifdef USINGE_USART1_DMA_R
		if(config1.Dma_R)
		{
			DMA_DeInit(DMA1_Channel5);
			DMA_InitStruct.DMA_PeripheralBaseAddr=(u32)&USART1->DR;
			DMA_InitStruct.DMA_MemoryBaseAddr=(u32)USART1_RECEIVE_DATA;
			DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;
			DMA_InitStruct.DMA_BufferSize = Usart_size;
			DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
			DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
			DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
			DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
			DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;
			DMA_InitStruct.DMA_Priority = DMA_Priority_High;
			DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
			DMA_Init(DMA1_Channel5,&DMA_InitStruct);
			DMA_ITConfig(DMA1_Channel5,DMA_IT_TC,ENABLE);
			DMA_ITConfig(DMA1_Channel5,DMA_IT_TE,ENABLE);
			/* Enable USART1 DMA RX request */
			USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);
			DMA_Cmd(DMA1_Channel5,ENABLE);
			NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =config1.Dma_Rpree;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = config1.Dma_Rsubpri;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);
		}
	#endif
	#ifdef USINGE_USART1_DMA_T
		if(config1.Dma_T)
		{
			DMA_DeInit(DMA1_Channel4);
			DMA_InitStruct.DMA_PeripheralBaseAddr=(u32)&USART1->DR;
			DMA_InitStruct.DMA_MemoryBaseAddr=(u32)USART1_SEND_DATA;
			DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;
			DMA_InitStruct.DMA_BufferSize = Usart_size;
			DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
			DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
			DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
			DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
			DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;
			DMA_InitStruct.DMA_Priority = DMA_Priority_High;
			DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
			DMA_Init(DMA1_Channel4,&DMA_InitStruct);
			DMA_ITConfig(DMA1_Channel4,DMA_IT_TC,ENABLE);
			DMA_ITConfig(DMA1_Channel4,DMA_IT_TE,ENABLE);
			/* Enable USART1 DMA RX request */
			USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);
			DMA_Cmd(DMA1_Channel4,ENABLE);
			NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =config1.Dma_Tpree;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = config1.Dma_Tsubpri;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);
		}
	#endif
	#ifdef USINGE_USART2_DMA_R
		if(config2.Dma_R)
		{
			DMA_DeInit(DMA1_Channel6);
			DMA_InitStruct.DMA_PeripheralBaseAddr=(u32)&USART2->DR;
			DMA_InitStruct.DMA_MemoryBaseAddr=(u32)USART2_RECEIVE_DATA;
			DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;
			DMA_InitStruct.DMA_BufferSize = Usart_size;
			DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
			DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
			DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
			DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
			DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;
			DMA_InitStruct.DMA_Priority = DMA_Priority_High;
			DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
			DMA_Init(DMA1_Channel6,&DMA_InitStruct);
			DMA_ITConfig(DMA1_Channel6,DMA_IT_TC,ENABLE);
			DMA_ITConfig(DMA1_Channel6,DMA_IT_TE,ENABLE);
			/* Enable USART2 DMA RX request */
			USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);
			DMA_Cmd(DMA1_Channel6,ENABLE);
			NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel6_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =config2.Dma_Rpree;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = config2.Dma_Rsubpri;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);
		}
		#endif
		#ifdef USINGE_USART2_DMA_T
		if(config2.Dma_T)
		{
			DMA_DeInit(DMA1_Channel7);
			DMA_InitStruct.DMA_PeripheralBaseAddr=(u32)&USART2->DR;
			DMA_InitStruct.DMA_MemoryBaseAddr=(u32)USART2_SEND_DATA;
			DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;
			DMA_InitStruct.DMA_BufferSize = Usart_size;
			DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
			DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
			DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
			DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
			DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;
			DMA_InitStruct.DMA_Priority = DMA_Priority_High;
			DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
			DMA_Init(DMA1_Channel7,&DMA_InitStruct);
			DMA_ITConfig(DMA1_Channel7,DMA_IT_TC,ENABLE);
			DMA_ITConfig(DMA1_Channel7,DMA_IT_TE,ENABLE);
			/* Enable USART2 DMA RX request */
			USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);
			DMA_Cmd(DMA1_Channel7,ENABLE);
			NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel7_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =config2.Dma_Tpree;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = config2.Dma_Tsubpri;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);
		}
	#endif
	#ifdef USINGE_USART3_DMA_R
		if(config3.Dma_R)
		{
			DMA_DeInit(DMA1_Channel3);
			DMA_InitStruct.DMA_PeripheralBaseAddr=(u32)&USART3->DR;
			DMA_InitStruct.DMA_MemoryBaseAddr=(u32)USART3_RECEIVE_DATA;
			DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;
			DMA_InitStruct.DMA_BufferSize = Usart_size;
			DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
			DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
			DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
			DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
			DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;
			DMA_InitStruct.DMA_Priority = DMA_Priority_High;
			DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
			DMA_Init(DMA1_Channel3,&DMA_InitStruct);
			DMA_ITConfig(DMA1_Channel3,DMA_IT_TC,ENABLE);
			DMA_ITConfig(DMA1_Channel3,DMA_IT_TE,ENABLE);
			/* Enable USART3 DMA RX request */
			USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);
			DMA_Cmd(DMA1_Channel3,ENABLE);
			NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel3_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =config3.Dma_Rpree;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = config3.Dma_Rsubpri;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);
		}
		#endif
		#ifdef USINGE_USART3_DMA_T
		if(config3.Dma_T)
		{
			DMA_DeInit(DMA1_Channel2);
			DMA_InitStruct.DMA_PeripheralBaseAddr=(u32)&USART3->DR;
			DMA_InitStruct.DMA_MemoryBaseAddr=(u32)USART3_SEND_DATA;
			DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;
			DMA_InitStruct.DMA_BufferSize = Usart_size;
			DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
			DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
			DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
			DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
			DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;
			DMA_InitStruct.DMA_Priority = DMA_Priority_High;
			DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
			DMA_Init(DMA1_Channel2,&DMA_InitStruct);
			DMA_ITConfig(DMA1_Channel2,DMA_IT_TC,ENABLE);
			DMA_ITConfig(DMA1_Channel2,DMA_IT_TE,ENABLE);
			/* Enable USART3 DMA RX request */
			USART_DMACmd(USART3,USART_DMAReq_Tx,ENABLE);
			DMA_Cmd(DMA1_Channel2,ENABLE);
			NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =config3.Dma_Tpree;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = config3.Dma_Tsubpri;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);
		}
	#endif
	#ifdef USINGE_UART4_DMA_R
		if(config4.Dma_R)
		{
			DMA_DeInit(DMA2_Channel3);
			DMA_InitStruct.DMA_PeripheralBaseAddr=(u32)&UART4->DR;
			DMA_InitStruct.DMA_MemoryBaseAddr=(u32)UART4_RECEIVE_DATA;
			DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;
			DMA_InitStruct.DMA_BufferSize = Usart_size;
			DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
			DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
			DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
			DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
			DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;
			DMA_InitStruct.DMA_Priority = DMA_Priority_High;
			DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
			DMA_Init(DMA2_Channel3,&DMA_InitStruct);
			DMA_ITConfig(DMA2_Channel3,DMA_IT_TC,ENABLE);
			DMA_ITConfig(DMA2_Channel3,DMA_IT_TE,ENABLE);
			/* Enable USART2 DMA RX request */
			USART_DMACmd(UART4,USART_DMAReq_Rx,ENABLE);
			DMA_Cmd(DMA2_Channel3,ENABLE);
			NVIC_InitStructure.NVIC_IRQChannel = DMA2_Channel3_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =config4.Dma_Rpree;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = config4.Dma_Rsubpri;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);
		}
		#endif
		#ifdef USINGE_UART4_DMA_T
		if(config4.Dma_T)
		{
			DMA_DeInit(DMA2_Channel5);
			DMA_InitStruct.DMA_PeripheralBaseAddr=(u32)&UART4->DR;
			DMA_InitStruct.DMA_MemoryBaseAddr=(u32)UART4_SEND_DATA;
			DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;
			DMA_InitStruct.DMA_BufferSize = Usart_size;
			DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
			DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
			DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
			DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
			DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;
			DMA_InitStruct.DMA_Priority = DMA_Priority_High;
			DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
			DMA_Init(DMA2_Channel5,&DMA_InitStruct);
			DMA_ITConfig(DMA2_Channel5,DMA_IT_TC,ENABLE);
			DMA_ITConfig(DMA2_Channel5,DMA_IT_TE,ENABLE);
			/* Enable USART3 DMA RX request */
			USART_DMACmd(UART4,USART_DMAReq_Tx,ENABLE);
			DMA_Cmd(DMA2_Channel5,ENABLE);
			NVIC_InitStructure.NVIC_IRQChannel = DMA2_Channel5_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =config4.Dma_Tpree;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = config4.Dma_Tsubpri;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);
		}
	#endif
}
/*
#ifdef USINGE_USART1
	void USART1_IRQHandler(void)
	{
		#ifndef USINGE_USART1_DMA_R
			u8 RES;
			static unsigned char statu=0;
		#else
			u8 RES;
		#endif
		#ifdef SYSTEM_SUPPORT_OS_II
			OSIntEnter(); 
		#endif
		#ifdef USINGE_USART1_DMA_R
			if(!USART1_MESSG.receve_ok)
			{
				if(USART_GetITStatus(USART1, USART_IT_IDLE)!=RESET)
				{
					DMA_Cmd(DMA1_Channel5,DISABLE);
					USART1_MESSG.date_lenth=Usart_size-DMA_GetCurrDataCounter(DMA1_Channel5);
					memcpy(&USART1_MESSG.messg_buff[0],&USART1_RECEIVE_DATA[0],USART1_MESSG.date_lenth);
					USART1_MESSG.receve_ok=TRUE;
					DMA_ClearFlag(DMA1_FLAG_GL5 | DMA1_FLAG_TC5 | DMA1_FLAG_TE5 | DMA1_FLAG_HT5);
					DMA1_Channel5->CNDTR=Usart_size;
					DMA_Cmd(DMA1_Channel5,ENABLE);
					
				}
			}
			RES=USART1->SR;
			RES=USART1->DR;
		#else
			if(USART_GetITStatus(USART1, USART_IT_RXNE)!=RESET)
			{
				RES =USART_ReceiveData(USART1);
				if(!USART1_MESSG.receve_ok)
				{
					switch(statu)
					{
						case 0:
							memset(USART1_MESSG.messg_buff,0,Usart_size); 
							USART1_MESSG.date_lenth=0;
							statu=1;
							USART1_MESSG.messg_buff[USART1_MESSG.date_lenth]=RES;
							USART1_MESSG.date_lenth++;
							break;
						case 1:
							if(RES==(0x0D))
							{
								statu=2;
								USART1_MESSG.messg_buff[USART1_MESSG.date_lenth]=RES;
								USART1_MESSG.date_lenth++;
							}
							else
							{
								USART1_MESSG.messg_buff[USART1_MESSG.date_lenth]=RES;
								USART1_MESSG.date_lenth++;
							}
							if(USART1_MESSG.date_lenth>Usart_size)
							{
								statu=0;
							}
							break;
						case 2:
							if(RES==(0x0A))
							{
								statu=0;
								USART1_MESSG.receve_ok=TRUE;
								USART1_MESSG.messg_buff[USART1_MESSG.date_lenth]=RES;
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
#endif
#ifdef USINGE_USART1_DMA_R
	void DMA1_Channel5_IRQHandler(void)
	{
		#ifdef SYSTEM_SUPPORT_OS_II
			OSIntEnter();
		#endif
		DMA_ClearITPendingBit(DMA1_IT_TC5);
		DMA_ClearITPendingBit(DMA1_IT_TE5);
		DMA_Cmd(DMA1_Channel5,DISABLE);//�ر�DMA����ֹ�������������
		DMA1_Channel5->CNDTR = Usart_size;//��װ��
		DMA_Cmd(DMA1_Channel5,ENABLE);//�����꣬�ؿ�DMA
		#ifdef SYSTEM_SUPPORT_OS_II
			OSIntExit();
		#endif
	}
#endif
#ifdef USINGE_USART1_DMA_T
	void DMA1_Channel4_IRQHandler(void)
	{
		#ifdef SYSTEM_SUPPORT_OS_II
			OSIntEnter();
		#endif
		DMA_ClearITPendingBit(DMA1_IT_TC4);
		DMA_ClearITPendingBit(DMA1_IT_TE4);
		DMA_Cmd(DMA1_Channel4,DISABLE);//�ر�DMA
		#ifdef SYSTEM_SUPPORT_OS_II
			OSIntExit();
		#endif
	}

#endif
*/
/*#ifdef USINGE_USART2

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
			if(!USART2_MESSG.receve_ok)
			{
				if(USART_GetITStatus(USART2, USART_IT_IDLE)!=RESET)
				{
					DMA_Cmd(DMA1_Channel6,DISABLE);
					USART2_MESSG.date_lenth=Usart_size-DMA_GetCurrDataCounter(DMA1_Channel6);
					memcpy(&USART2_MESSG.messg_buff[0],&USART2_RECEIVE_DATA[0],USART2_MESSG.date_lenth);
					USART2_MESSG.receve_ok=TRUE;
					DMA_ClearFlag(DMA1_FLAG_GL6 | DMA1_FLAG_TC6 | DMA1_FLAG_TE6 | DMA1_FLAG_HT6);
					DMA1_Channel6->CNDTR=Usart_size;
					DMA_Cmd(DMA1_Channel6,ENABLE);
					RES=USART2->SR;
					RES=USART2->DR;
				}
			}
			RES=USART2->SR;
			RES=USART2->DR;
		#else
			if(USART_GetITStatus(USART2, USART_IT_RXNE)!=RESET)
			{
				RES =USART_ReceiveData(USART2);
				if(!USART2_MESSG.receve_ok)
				{
					switch(statu)
					{
						case 0:
							memset(&USART2_MESSG.messg_buff[0],0,Usart_size); 
							USART2_MESSG.date_lenth=0;
							statu=1;
							USART2_MESSG.messg_buff[USART2_MESSG.date_lenth]=RES;
							USART3_MESSG.date_lenth++;
							break;
						case 1:
							if(RES==(0x0D))
							{
								statu=2;
								USART2_MESSG.messg_buff[USART2_MESSG.date_lenth]=RES;
								USART2_MESSG.date_lenth++;
							}
							else
							{
								USART2_MESSG.messg_buff[USART2_MESSG.date_lenth]=RES;
								USART2_MESSG.date_lenth++;
							}
							if(USART2_MESSG.date_lenth>Usart_size)
							{
								statu=0;
							}
							break;
						case 2:
							if(RES==(0x0A))
							{
								statu=0;
								USART2_MESSG.receve_ok=TRUE;
								USART2_MESSG.messg_buff[USART2_MESSG.date_lenth]=RES;
								USART2_MESSG.date_lenth++;
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
#endif*/
#ifdef USINGE_USART2_DMA_R
	void DMA1_Channel6_IRQHandler(void)
	{
		#ifdef SYSTEM_SUPPORT_OS_II
			OSIntEnter();
		#endif
		DMA_ClearITPendingBit(DMA1_IT_TC6);
		DMA_ClearITPendingBit(DMA1_IT_TE6);
		DMA_Cmd(DMA1_Channel6,DISABLE);//�ر�DMA����ֹ�������������
		DMA1_Channel6->CNDTR = Usart_size;//��װ��
		DMA_Cmd(DMA1_Channel6,ENABLE);//�����꣬�ؿ�DMA
		#ifdef SYSTEM_SUPPORT_OS_II
			OSIntExit();
		#endif
	}

#endif
#ifdef USINGE_USART2_DMA_T
	void DMA1_Channel7_IRQHandler(void)
	{
		#ifdef SYSTEM_SUPPORT_OS_II
			OSIntEnter();
		#endif
		DMA_ClearITPendingBit(DMA1_IT_TC7);
		DMA_ClearITPendingBit(DMA1_IT_TE7);
		DMA_Cmd(DMA1_Channel7,DISABLE);//�ر�DMA
		#ifdef SYSTEM_SUPPORT_OS_II
			OSIntExit();
		#endif
	}
#endif


/*
#ifdef USINGE_USART3
	void USART3_IRQHandler(void)
	{
		#ifndef USINGE_USART3_DMA_R
			u8 RES;
			static unsigned char statu=0;
		#else
			u8 RES;
		#endif
		
#ifdef SUPPORT_OS_FLAG
		INT8U  err = 0; 
		#endif
		#ifdef SYSTEM_SUPPORT_OS_II
			OSIntEnter();
		#endif
		#ifdef USINGE_USART3_DMA_R
			if(!USART3_MESSG.receve_ok)
			{
				if(USART_GetITStatus(USART3, USART_IT_IDLE)!=RESET)
				{
					DMA_Cmd(DMA1_Channel3,DISABLE);
					USART3_MESSG.date_lenth=Usart_size-DMA_GetCurrDataCounter(DMA1_Channel3);
					memcpy(&USART3_MESSG.messg_buff[0],&USART3_RECEIVE_DATA[0],USART3_MESSG.date_lenth);
					USART3_MESSG.receve_ok=TRUE;
					DMA_ClearFlag(DMA1_FLAG_GL3 | DMA1_FLAG_TC3 | DMA1_FLAG_TE3 | DMA1_FLAG_HT3);
					DMA1_Channel3->CNDTR=Usart_size;
					DMA_Cmd(DMA1_Channel3,ENABLE);
				}
			}
			RES=USART3->SR;
			RES=USART3->DR;
		#else
			if(USART_GetITStatus(USART3, USART_IT_RXNE)!=RESET)
			{
				RES =USART_ReceiveData(USART3);
				if(!USART3_MESSG.receve_ok)
				{
					switch(statu)
					{
						case 0:
							memset(&USART3_MESSG.messg_buff[0],0,Usart_size); 
							USART3_MESSG.date_lenth=0;
							statu=1;
							USART3_MESSG.messg_buff[USART3_MESSG.date_lenth]=RES;
							USART3_MESSG.date_lenth++;
							break;
						case 1:
							if(RES==(0x0D))
							{
								statu=2;
								USART3_MESSG.messg_buff[USART3_MESSG.date_lenth]=RES;
								USART3_MESSG.date_lenth++;
							}
							else
							{
								USART3_MESSG.messg_buff[USART3_MESSG.date_lenth]=RES;
								USART3_MESSG.date_lenth++;
							}
							if(USART3_MESSG.date_lenth>Usart_size)
							{
								statu=0;
							}
							break;
						case 2:
							if(RES==(0x0A))
							{
								statu=0;
								USART3_MESSG.receve_ok=TRUE;
								USART3_MESSG.messg_buff[USART3_MESSG.date_lenth]=RES;
								USART3_MESSG.date_lenth++;
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
#endif*/
#ifdef USINGE_USART3_DMA_R
	void DMA1_Channel3_IRQHandler(void)
	{
		#ifdef SYSTEM_SUPPORT_OS_II
			OSIntEnter();
		#endif
		DMA_ClearITPendingBit(DMA1_IT_TC3);
		DMA_ClearITPendingBit(DMA1_IT_TE3);
		DMA_Cmd(DMA1_Channel3,DISABLE);//�ر�DMA����ֹ�������������
		DMA1_Channel3->CNDTR = Usart_size;//��װ��
		DMA_Cmd(DMA1_Channel3,ENABLE);//�����꣬�ؿ�DMA
		#ifdef SYSTEM_SUPPORT_OS_II
			OSIntExit();
		#endif
	}
#endif
#ifdef USINGE_USART3_DMA_T
	void DMA1_Channel2_IRQHandler(void)
	{
		#ifdef SYSTEM_SUPPORT_OS_II
			OSIntEnter();
		#endif
		DMA_ClearITPendingBit(DMA1_IT_TC2);
		DMA_ClearITPendingBit(DMA1_IT_TE2);
		DMA_Cmd(DMA1_Channel2,DISABLE);//�ر�DMA
		#ifdef SYSTEM_SUPPORT_OS_II
			OSIntExit();
		#endif
	}
#endif
#ifdef USINGE_UART4
	void USART4_IRQHandler(void)
	{
		#ifndef USINGE_UART4_DMA_R
			u8 RES;
			static unsigned char statu=0;
		#else
			u8 RES;
		#endif
		#ifdef SYSTEM_SUPPORT_OS_II
			OSIntEnter();
		#endif
		#ifdef USINGE_UART4_DMA_R
			if(!UART4_MESSG.receve_ok)
			{
				if(USART_GetITStatus(USART3, USART_IT_IDLE)!=RESET)
				{
					DMA_Cmd(DMA2_Channel3,DISABLE);
					UART4_MESSG.date_lenth=Usart_size-DMA_GetCurrDataCounter(DMA2_Channel3);
					memcpy(&UART4_MESSG.messg_buff[0],&UART4_RECEIVE_DATA[0],UART4_MESSG.date_lenth);
					UART4_MESSG.receve_ok=TRUE;
					DMA_ClearFlag(DMA2_FLAG_GL3 | DMA2_FLAG_TC3 | DMA2_FLAG_TE3 | DMA2_FLAG_HT3);
					DMA2_Channel3->CNDTR=Usart_size;
					DMA_Cmd(DMA2_Channel3,ENABLE);
				}
			}
			RES=UART4->SR;
			RES=UART4->DR;
		#else
			if(USART_GetITStatus(UART4, USART_IT_RXNE)!=RESET)
			{
				RES =USART_ReceiveData(UART4);
				if(!UART4_MESSG.receve_ok)
				{
					switch(statu)
					{
						case 0:
							memset(&UART4_MESSG.messg_buff[0],0,Usart_size); 
							UART4_MESSG.date_lenth=0;
							statu=1;
							UART4_MESSG.messg_buff[UART4_MESSG.date_lenth]=RES;
							UART4_MESSG.date_lenth++;
							break;
						case 1:
							if(RES==(0x0D))
							{
								statu=2;
								UART4_MESSG.messg_buff[UART4_MESSG.date_lenth]=RES;
								UART4_MESSG.date_lenth++;
							}
							else
							{
								UART4_MESSG.messg_buff[UART4_MESSG.date_lenth]=RES;
								UART4_MESSG.date_lenth++;
							}
							if(UART4_MESSG.date_lenth>Usart_size)
							{
								statu=0;
							}
							break;
						case 2:
							if(RES==(0x0A))
							{
								statu=0;
								UART4_MESSG.receve_ok=TRUE;
								UART4_MESSG.messg_buff[UART4_MESSG.date_lenth]=RES;
								UART4_MESSG.date_lenth++;
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
#endif
#ifdef USINGE_UART4_DMA_R
	void DMA2_Channel3_IRQHandler(void)
	{
		#ifdef SYSTEM_SUPPORT_OS_II
			OSIntEnter();
		#endif
		DMA_ClearITPendingBit(DMA2_IT_TC3);
		DMA_ClearITPendingBit(DMA2_IT_TE3);
		DMA_Cmd(DMA2_Channel3,DISABLE);//�ر�DMA����ֹ�������������
		DMA2_Channel3->CNDTR = Usart_size;//��װ��
		DMA_Cmd(DMA2_Channel3,ENABLE);//�����꣬�ؿ�DMA
		#ifdef SYSTEM_SUPPORT_OS_II
			OSIntExit();
		#endif
	}
#endif
#ifdef USINGE_UART4_DMA_T
	void DMA2_Channel5_IRQHandler(void)
	{
		#ifdef SYSTEM_SUPPORT_OS_II
		OSIntEnter();
		#endif
		DMA_ClearITPendingBit(DMA2_IT_TC5);
		DMA_ClearITPendingBit(DMA2_IT_TE5);
		DMA_Cmd(DMA2_Channel5,DISABLE);//�ر�DMA
		#ifdef SYSTEM_SUPPORT_OS_II
			OS_CRITICAL_EXIT();
		#endif
	}
#endif
/*******************************************************************************
* ������  : UART_SendString
* ����    : USART�����ַ���
* ����    : *s�ַ���ָ��
* ���    : ��
* ����    : �� 
* ˵��    : ��
*******************************************************************************/
void UART_SendString(USART_TypeDef* USARTx,char* s)
{
	while(*s)//����ַ���������
	{
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET); 
		USART_SendData(USARTx ,*s);//���͵�ǰ�ַ�
		s++;
	}
}

void Usart_init(void)
{
	Usat_rcc_init();
	Usart_io_init();
	Dma_init();
	#ifdef DEBUG_PERPH
		#ifdef USINGE_USART1
		printf("usart1 open!\r\n");
		#endif
		#ifdef USINGE_USART2
		printf("usart2 open!\r\n");
		#endif
		#ifdef USINGE_USART3
		printf("usart3 open!\r\n");
		#endif
		#ifdef USINGE_UART4
		printf("uart4 open!\r\n");
		#endif
		#ifdef USINGE_UART5
		printf("uart5 open!\r\n");
		#endif
		#ifdef USINGE_USART1_DMA_T
		printf("usart1 dma_t open!\r\n");
		#endif
		#ifdef USINGE_USART1_DMA_R
		printf("usart1 dma_r open!\r\n");
		#endif
		#ifdef USINGE_USART2_DMA_T
		printf("usart2 dma_t open!\r\n");
		#endif
		#ifdef USINGE_USART2_DMA_R
		printf("usart2 dma_r open!\r\n");
		#endif
		#ifdef USINGE_USART3_DMA_T
		printf("usart3 dma_t open!\r\n");
		#endif
		#ifdef USINGE_USART3_DMA_R
		printf("usart3 dma_r open!\r\n");
		#endif
		#ifdef USINGE_UART4_DMA_T
		printf("uart4 dma_t open!\r\n");
		#endif
		#ifdef USINGE_UART4_DMA_R
		printf("uart4 dma_r open!\r\n");
		#endif
	#endif
}

