/*
 * File      : wifi_esp.c
 * This file is ef comp
 * COPYRIGHT (C) 2018,
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at my addr
 *
 *
 * Change Logs:
 * Date           Author       ZAM
 * 2018-1-5     Bernard      the first version
 */
#include "wifi_esp.h"
#include "delay.h"
#include "usart.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifdef SYSTEM_SUPPORT_OS
#include "app.h"
#endif

wifi_soc_address MY_COMP={"192.168.0.101",8080};

#define BUF_MAX 40
#define RECV_DATA_MAX_BUF 2048

/************************************************************************/
static unsigned int wifi_recived_len=0;			//�����յ������ݼ�����
static unsigned int wifi_get_date_len=0;		//�����ȡ�����ݳ���
static unsigned char wifi_cnd_len=0;	//�����յ������ݼ�����
static unsigned char begin_recive_flag=FALSE;	//��ʼ�ռ��������ݱ�־
char wifi_date_buf[RECV_DATA_MAX_BUF]={0};			//������ȡ�����ݻ���
char wifi_cmd_date[BUF_MAX]={0};
/************************************************************************/

/*******************************************************************************
�� �� ���� Wifi_Esp_Cmd(void)
���������� wifi��������
��ڲ�����	cmd ��Ҫ���͵����� reply1��Ҫ���ص�����	reply2��Ҫ���ص�����	wait_time��ʱʱ��ms
���ز����������Ƿ�ɹ� 1���ɹ� 0:�����ַ� 2 :��ʱ
����ʱ��: 2018-1-05 by zam
********************************************************************************/
void USART3_IRQHandler(void)
{
		u8 RES;
	#ifdef SYSTEM_SUPPORT_OS
			CPU_SR_ALLOC();
			OS_CRITICAL_ENTER();
	#endif
	if(USART_GetITStatus(WIFI_UART_N, USART_IT_RXNE)!=RESET)
	{
		RES =USART_ReceiveData(WIFI_UART_N);
		if(begin_recive_flag)
		{
			wifi_date_buf[wifi_recived_len]=RES;
			wifi_recived_len++;
			if(wifi_recived_len>=wifi_get_date_len)		//������ȡ�������
			{
				begin_recive_flag=FALSE;
				wifi_date_buf[wifi_recived_len+1]=0;
			}
		}
		else
		{
			wifi_cmd_date[wifi_cnd_len]=RES;
			wifi_cnd_len++;
			if(wifi_cnd_len>=BUF_MAX)//ȷ���������ݲ��ᳬ��
			{
				wifi_cnd_len=0;
			}
		}
	}
	#ifdef SYSTEM_SUPPORT_OS
			OS_CRITICAL_EXIT();
	#endif
}
/*******************************************************************************
�� �� ����void clear_buf(void)
����������������ڽ��ջ�����,���ý���ƫ����Ϊ0		
��ڲ�����							
���ز����� 
����ʱ��: 2017-11-02 by zam
********************************************************************************/
static void clear_buf(void)
{
	memset(wifi_cmd_date, 0, BUF_MAX);
  	wifi_cnd_len = 0;              //�����ַ�������ʼ�洢λ��
}
/*******************************************************************************
�� �� ���� Wifi_Esp_Cmd(void)
���������� wifi��������
��ڲ�����	cmd ��Ҫ���͵����� reply1��Ҫ���ص�����	reply2��Ҫ���ص�����	wait_time��ʱʱ��ms
���ز����������Ƿ�ɹ� 1���ɹ� 0:�����ַ� 2 :��ʱ
����ʱ��: 2018-1-05 by zam
********************************************************************************/
unsigned char Wifi_Esp_Cmd(char *cmd,char *reply1,unsigned int wait_time)
{
	unsigned char ret=0;
	clear_buf();
	Wif_Send("%s\r\n",cmd);
	if ( ( reply1 == NULL ) || (wait_time==0)) //��������
	{
		return 1;
	}
	Time_wait_cnt=wait_time;
	Time_statr_flag=TRUE;
	while(1)
	{
		if(strstr(wifi_cmd_date,reply1) != NULL)
		{
			ret=0;//�����ַ���
			break;		
		}
		if(Time_out_flag)
		{
			ret=2;//��ʱ
			break;	
		}
	}
	Time_statr_flag=FALSE;
	Time_out_flag=FALSE;
	return ret;
	
}
/*******************************************************************************
�� �� ����void Wifi_Io_Init(void)
���������� wifi���ƶ˿ڳ�ʼ��
��ڲ�����	��
���ز�������
����ʱ��: 2018-1-05 by zam
********************************************************************************/
void Wifi_Io_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(WIFI_CH_RCC,ENABLE);
	GPIO_InitStructure.GPIO_Pin = WIFI_CH_GPIO;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(WIFI_CH_PORT, &GPIO_InitStructure);
	WIFI_CH_OFF;
	
	RCC_APB2PeriphClockCmd(WIFI_RST_RCC,ENABLE);
	GPIO_InitStructure.GPIO_Pin = WIFI_RST_GPIO;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(WIFI_RST_PORT, &GPIO_InitStructure);
	WIFI_RST_ON;
	#ifdef DEBUG_MEG
		__DBG__("WIFI���ܴ�!\r\n");
	#endif 
}
/*******************************************************************************
�� �� ����void Wifi_Net_Mode(void)
���������� wifiģʽ����
��ڲ�����	mode 
���ز�������
����ʱ��: 2018-1-05 by zam
********************************************************************************/
unsigned char Wifi_Net_Mode(ENUM_Net_ModeTypeDef mode)
{
	switch ( mode )
	{
		case STA:
			return Wifi_Esp_Cmd ( "AT+CWMODE=1", "OK", 2500 ); 
		
	  	case AP:
		  	return Wifi_Esp_Cmd ( "AT+CWMODE=2", "OK",  2500 ); 
		
		case STA_AP:
		  	return Wifi_Esp_Cmd ( "AT+CWMODE=3", "OK",  2500 ); 
		
	  	default:
		  	return FALSE;
  }
}
/*******************************************************************************
�� �� ����unsigned char Wifi_JoinAP(void)
���������� wifi���뵽����
��ڲ�����	pSSID ��������  pPassWord��������
���ز�����1���ɹ� 0:�����ַ� 2 :��ʱ
����ʱ��: 2018-1-05 by zam
********************************************************************************/
unsigned char Wifi_JoinAP ( char * pSSID, char * pPassWord )
{
	char cCmd [120];

	sprintf ( cCmd, "AT+CWJAP=\"%s\",\"%s\"", pSSID, pPassWord );
	
	return Wifi_Esp_Cmd ( cCmd, "OK", 7000 );
	
}
/*******************************************************************************
�� �� ����Wifi_Connet
���������� wifi���ӵ�������
��ڲ�����	��������ַ�Ͷ˿ں�
���ز�����0:���ӳɹ� 1:����ʧ��
����ʱ��: 2018-1-05 by zam
********************************************************************************/
unsigned char Wifi_Connet(wifi_soc_address addrss)
{
	char connect_buf[200];
	Wifi_Esp_Cmd ( "AT+CIPMUX=0", "OK", 500 );
	memset(connect_buf,0,sizeof(connect_buf));
	sprintf(connect_buf,"AT+CIPSTART=\"TCP\",\"%s\",%d",addrss.hostName,addrss.port);
	#ifdef DEBUG_MEG
		__DBG__("%s\r\n",connect_buf);
	#endif
	if(Wifi_Esp_Cmd(connect_buf,"OK",1000)!=0)
	{
		#ifdef DEBUG_MEG
			__DBG__("���ӷ���������!\r\n");
		#endif
		return 1;	//����
	}
	else
		return 0;
}
/*******************************************************************************
�� �� ����Wifi_Clse
���������� wifi�ر����ӵ�������
��ڲ�������
���ز�����0:���ӳɹ� 1:����ʧ��
����ʱ��: 2018-1-05 by zam
********************************************************************************/
unsigned char Wifi_Clse(void)
{
	if(Wifi_Esp_Cmd("AT+CIPCLOSE","OK",5000)!=0)
	{
		#ifdef DEBUG_MEG
			__DBG__("�޷��رշ�����!\r\n");
		#endif
		return 1;	//����
	}
	else
		return 0;
}

/*******************************************************************************
�� �� ����Wifi_Send
���������� wifi�������ݵ�������
��ڲ�����	send_buffer:����
���ز�����0:���ӳɹ� 1:����ʧ��
����ʱ��: 2018-1-05 by zam
********************************************************************************/
unsigned char Wifi_Send(char *send_buffer)
{
	char sendbuf[1024];
	Wifi_Esp_Cmd("AT+CIPMODE=1","OK",500);	//͸��ģʽ
	if(Wifi_Esp_Cmd("AT+CIPSEND=0,",">",500)!=0)	//���뷢��ģʽ
	{
		#ifdef DEBUG_MEG
			__DBG__("�޷����뷢������ģʽ!\r\n");
		#endif
		return 1;
	}
	sprintf(sendbuf,"%s\32\0",send_buffer);
	if(Wifi_Esp_Cmd(sendbuf,"OK",5000)!=0)
	{
		#ifdef DEBUG_MEG
			__DBG__("��������ʧ��!\r\n");
		#endif
		return 1;
	}
	else
		return 0;
}
/*******************************************************************************
�� �� ����Wifi_Init
���������� wifi��ʼ��
��ڲ�����	��
���ز�������
����ʱ��: 2018-1-05 by zam
********************************************************************************/
void Wifi_Init()
{
	unsigned char ret=0;
	WIFI_CH_ON;
	WIFI_RST_ON;
//	Wifi_Esp_Cmd("AT+RST", "OK",1000);
	Wifi_Net_Mode(AP);
//	Wifi_Esp_Cmd("AT+RST", "OK",1000);
	Wifi_Esp_Cmd ( "AT+CWLAP", "OK",500 );
	//Wifi_Esp_Cmd("AT+RST", "OK",1000);
	ret=Wifi_JoinAP(Pssid,PassWord);//����wifi
	#ifdef DEBUG_MEG
		if(ret==0)
		{
			__DBG__("����wifi:%s,����:%s\r\n",Pssid,PassWord);
		}
		else if(ret==2)
		{
			__DBG__("����wifi��ʱ!\r\n");
		}
	#endif
}



