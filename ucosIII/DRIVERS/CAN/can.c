/*
 * File      : can.c
 * This file is ef comp
 * COPYRIGHT (C) 2018,
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at my addr
 * 
 * Change Logs:
 * Date           Author       ZAM
 * 2018-01-9     Bernard      the first version
 */
 
 #include "can.h"
 #include <string.h>
  #include "usart.h"
	/*******************************************************************************
�� �� ����void Can_Config_Init(void)
����������can���߳�ʼ������
��ڲ�������
���ز�������
����ʱ��: 2018-1-05 by zam
********************************************************************************/

 void Can_Config_Init()
 {
		GPIO_InitTypeDef  GPIO_InitStructure;
		CAN_InitTypeDef        CAN_InitStructure;
		CAN_FilterInitTypeDef  CAN_FilterInitStructure;
		#ifdef CAN1_PERPH
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
		GPIO_Init(GPIOA, &GPIO_InitStructure);			

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
		GPIO_Init(GPIOA, &GPIO_InitStructure);	

		CAN_InitStructure.CAN_TTCM=DISABLE;			
		CAN_InitStructure.CAN_ABOM=DISABLE;			 
		CAN_InitStructure.CAN_AWUM=DISABLE;			
		CAN_InitStructure.CAN_NART=ENABLE;			
		CAN_InitStructure.CAN_RFLM=DISABLE;		 	  
		CAN_InitStructure.CAN_TXFP=DISABLE;	
		CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
		CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;  
		CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;
		CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;
		
		#if CAN_BAUDRATE == 1000 /* 1MBps */
			CAN_InitStructure.CAN_Prescaler =6;
		#elif CAN_BAUDRATE == 500 /* 500KBps */
			CAN_InitStructure.CAN_Prescaler =12;
		#elif CAN_BAUDRATE == 250 /* 250KBps */
			CAN_InitStructure.CAN_Prescaler =24;
		#elif CAN_BAUDRATE == 125 /* 125KBps */
			CAN_InitStructure.CAN_Prescaler =48;
		#elif  CAN_BAUDRATE == 100 /* 100KBps */
			CAN_InitStructure.CAN_Prescaler =60;
		#elif  CAN_BAUDRATE == 50 /* 50KBps */
			CAN_InitStructure.CAN_Prescaler =120;
		#elif  CAN_BAUDRATE == 20 /* 20KBps */
			CAN_InitStructure.CAN_Prescaler =300;
		#elif  CAN_BAUDRATE == 10 /* 10KBps */
			CAN_InitStructure.CAN_Prescaler =600;
		#else
			 #error "Please select first the CAN Baudrate in Private defines in main.c "
		#endif  /* CAN_BAUDRATE == 1000 */
		CAN_Init(CAN1, &CAN_InitStructure);
		
		CAN_FilterInitStructure.CAN_FilterNumber = 1;		//��������
		CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;//����λ
		CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;	//32λ��
		CAN_FilterInitStructure.CAN_FilterIdHigh = 0x6420;	//��λ
		CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;		//��λ
		CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;	//mask��λ
		CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;	//��λ
		CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;//������0
		CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
		CAN_FilterInit(&CAN_FilterInitStructure);
		#ifdef DEBUG_MEG
			__DBG__("CAN1���ܴ�!\r\n");
		#endif
		#endif
 }
  /*******************************************************************************
�� �� ���� Can_Send_date(void)
���������� can��������   
��ڲ�����CANx: can��    date_config����������  msg������ len������
���ز�����0�����ͳɹ�  1������ʧ��
����ʱ��: 2018-1-05 by zam
********************************************************************************/
 u8 Can_Send_date(CAN_TypeDef* CANx,struct Can_Date_Config date_config,char* msg,char len)
 {
		u16 i=0;
	  u8 mbox;
		CanTxMsg TxMessage;
		TxMessage.StdId=date_config.StdId;			
		TxMessage.ExtId=date_config.ExtId;		
		TxMessage.IDE=date_config.IDE;	
		TxMessage.RTR=date_config.RTR;
		TxMessage.DLC=len;
	  for(i=0;i<len;i++)
		{
				TxMessage.Data[i]=msg[i];
		}
		i=0;
	  mbox= CAN_Transmit(CANx, &TxMessage);   
		while((CAN_TransmitStatus(CANx, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//���ͳ�ʱ�ж�
		if(i>=0XFFF)return 1;
		return 0;
 }
 /*******************************************************************************
�� �� ���� Can_Receive_Msg(void)
���������� can��������
��ڲ�����CANx: can��   buf�����ݴ洢
���ز�����0:û���յ�����   ����Ϊ�յ������ݳ���
����ʱ��: 2018-1-05 by zam
********************************************************************************/ 
u8 Can_Receive_Msg(CAN_TypeDef* CANx,u8 *buf)
{		   		   
		u32 i;
		CanRxMsg RxMessage;
    if( CAN_MessagePending(CANx,CAN_FIFO0)==0)return 0;		//����Ƿ��յ�����
    CAN_Receive(CANx, CAN_FIFO0, &RxMessage);//ע��  �����������Ҫ�����ö�Ӧ
    for(i=0;i<8;i++)
    {
			buf[i]=RxMessage.Data[i];  
		}
		return RxMessage.DLC;	
}

