/*
 * File      : dht11.h
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
#include "dht11.h"
#include "delay.h"
#include "usart.h"

unsigned int temperature,humidity;
/*******************************************************************************
�� �� ����void DHT11_Rst(void)
���������� ��λDHT11
��ڲ�����	 ��
���ز�������
����ʱ��: 2018-1-13 by zam
********************************************************************************/
void DHT11_Rst(void)	   
{                 
		DHT11_OUT; 	//SET OUTPUT
		DHT11_RESET; 	//����DQ
		delay_ms(2);    	//��������0.8ms
		DHT11_SET; 	//DQ=1 
		//DHT11_IN;
		//delay_us(10);     	//��������20~40us
}
/*******************************************************************************
�� �� ����void DHT11_Check(void)
�����������ȴ�DHT11�Ļ�Ӧ
��ڲ�����	 ��
���ز���������1:δ��⵽DHT11�Ĵ��� ����0:����
����ʱ��: 2018-1-13 by zam
********************************************************************************/
u8 DHT11_Check(void) 	   
{   
	u16 retry=0x00;
	DHT11_IN;//SET INPUT	
	//delay_us(1);
	while(retry++<5);
	while ((GPIO_ReadInputDataBit(DHT11_PORT,DTH11_GPIO))&&(retry<0x1ff))//DHT11������40~80us
	{
		retry++;
		//delay_us(1);
	};
	if(retry>=0x190)
		return 1;
	else retry=0x00;
  while ((!GPIO_ReadInputDataBit(DHT11_PORT,DTH11_GPIO))&&(retry<0x1ff))//DHT11������40~80us
	{
		retry++;
		//delay_us(1);
	};	 
	if(retry>=0x190)
		return 1;
	else 
		retry=0x00;
	while((GPIO_ReadInputDataBit(DHT11_PORT,DTH11_GPIO))&&(retry<0x1ff))//�ȴ���Ϊ�͵�ƽ
	{
		retry++;
		//delay_us(1);
	}
	if(retry>=0x190)
		return 1;
	else 
		return 0;
}
/*******************************************************************************
�� �� ����void DHT11_Read_Bit(void)
������������DHT11��ȡһ��λ
��ڲ�����	 ��
���ز���������ֵ��1/0
����ʱ��: 2018-1-13 by zam
********************************************************************************/
u8 DHT11_Read_Bit(void) 			 
{
 	u16 retry=0;
	while((!GPIO_ReadInputDataBit(DHT11_PORT,DTH11_GPIO))&&(retry<0x1ff))//�ȴ���Ϊ�͵�ƽ
	{
		retry++;
		//delay_us(1);
	}
	retry=0x00;
	while((GPIO_ReadInputDataBit(DHT11_PORT,DTH11_GPIO))&&(retry<0x1ff))//�ȴ���ߵ�ƽ
	{
		retry++;
		//delay_us(1);
	}
	if(retry>0x78) 
		return 1;
	else 
		return 0;		   
}
/*******************************************************************************
�� �� ����void DHT11_Read_Byte(void)
������������DHT11��ȡһ���ֽ�
��ڲ�����	 ��
���ز���������������
����ʱ��: 2018-1-13 by zam
********************************************************************************/
char DHT11_Read_Byte(void)    
{        
    char i,dat;
    dat=0;
		for (i=0;i<8;i++) 
		{
				dat<<=1; 
				dat|=DHT11_Read_Bit();
		}						    
		return dat;
}
/*******************************************************************************
�� �� ����void DHT11_Read_Data(void)
����������temp:�¶�ֵ(��Χ:0~50��)   humi:ʪ��ֵ(��Χ:20%~90%)
��ڲ�����	 ��
���ز�����0,����;1,��ȡʧ��
����ʱ��: 2018-1-13 by zam
********************************************************************************/
u8 DHT11_Read_Data()    
{        
 	u8 buf[5];
	u8 i;
	DHT11_Rst();
	if(DHT11_Check()==0)
	{
		for(i=0;i<5;i++)//��ȡ40λ����
		{
			buf[i]=DHT11_Read_Byte();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
		{
			temperature=buf[2]*256+buf[3];
			humidity=buf[0]*256+buf[1];
		}
	}
	else 
	{
		return 1;
	}
	return 0;	    
}
/*******************************************************************************
�� �� ����void DHT11_Init(void)
������������ʼ��DHT11��IO�� DQ ͬʱ���DHT11�Ĵ���
��ڲ�����	 ��
���ز�����1:������ 0:���� 
����ʱ��: 2018-1-13 by zam
********************************************************************************/   	 
u8 DHT11_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(DHT11_RCC,ENABLE);
	GPIO_InitStructure.GPIO_Pin = DTH11_GPIO;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DHT11_PORT, &GPIO_InitStructure);
	DHT11_SET;		
	#ifdef DEBUG_MEG
			__DBG__("DHT11���ܴ�!\r\n");
		#endif 	
	DHT11_Rst();
	return DHT11_Check();
}

