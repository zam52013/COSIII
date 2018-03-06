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
 #include <string.h>
 #include "usart.h"
 #include "delay.h"
 #include <stdlib.h>
#include <stdio.h>

/*******************************************************************************
�� �� ����void Sim_contro(void)
���������� ��ʼ�����ƶ˿�
��ڲ�����	��
���ز�������
����ʱ��: 2018-1-05 by zam
********************************************************************************/
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
		#ifdef DEBUG_MEG
			__DBG__("SIM7600���ܴ�!\r\n");
		#endif
// 		SIM_POWKEY_OFF;
 }
/*******************************************************************************
�� �� ���� Sim_Net_cmd(void)
���������� ����ģ�鷢������
��ڲ�����	cmd ��Ҫ���͵����� reply1��Ҫ���ص�����	reply2��Ҫ���ص�����	wait_time��ʱʱ��ms
���ز����������Ƿ�ɹ� 1���ɹ� 0:�����ַ� 2 :��ʱ
����ʱ��: 2018-1-12 by zam
********************************************************************************/
 unsigned char Sim_Net_cmd(char *cmd,char *reply1,unsigned int wait_time) 
 {
		unsigned char ret=0;
		SIMsend("%s\r\n",cmd);
		if ( ( reply1 == NULL ) || (wait_time==0)) //��������
		{
			return 1;
		}
		Time_wait_cnt=wait_time;
		Time_statr_flag=TRUE;
		while(1)
	{
		if(SIM_Mess.receve_ok)
		{
			if(strstr(SIM_Mess.messg_buff,reply1) != NULL)
			{
				ret=0;//�����ַ���
				SIM_Mess.receve_ok=FALSE;
				break;		
			}
			SIM_Mess.receve_ok=FALSE;
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
 �� �� ����int8_t Init_Gsm_Gprs(void)
 ������������ʼ������				 
 ��ڲ����� 						 
 ���ز�����0: �ɹ�   ����: ʧ��
 ����ʱ��: 2017-11-02 by zam
 ********************************************************************************/

 unsigned char Init_Gsm_Gprs(void)
 {
	int8_t ret = 0;
	if(Sim_Net_cmd("AT+CICCID","SIM not",500)==0)
	{
		#ifdef DEBUG_MEG
			__DBG__("û��sim��!\r\n");
		#endif
		return 1;
	}
	if(Sim_Net_cmd("ATE0","SIM not",500)!=0)
	{
		return 1;
	}
	if((ret = Sim_Net_cmd("AT+CGREG=1","OK",500)) != 0) //����GPRS�ƶ�̨���ΪA,֧�ְ����������ݽ��� 
	{
		#ifdef DEBUG_MEG
			__DBG__("����ע��ʧ��!\r\n");
		#endif
		return ret;
	}	
	if((ret =Sim_Net_cmd("AT+CGDCONT=1,\"IP\",\"CMNET\"","OK",200)) != 0)
	{
		#ifdef DEBUG_MEG
			__DBG__("IP���ô���!\r\n");
		#endif
		//����PDP������,��������Э��,��������Ϣ
		return ret;
	}
	if((ret = Sim_Net_cmd("AT+CSOCKSETPN=1","OK",200)) != 0) 
	{
		#ifdef DEBUG_MEG
			__DBG__("CSOCKSETPN����ʧ��!\r\n");
		#endif
		return ret;
	}
	if((ret = Sim_Net_cmd("AT+CIPMODE=0","OK",200)) != 0) 
	{
		#ifdef DEBUG_MEG
			__DBG__("ģʽ����ʧ��!\r\n");
		#endif
		//����GPRSҵ��
		return ret;
	}
	if((ret = Sim_Net_cmd("AT+NETOPEN","OK",200)) != 0) 
	{
		#ifdef DEBUG_MEG
			__DBG__("����δ��!\r\n");
		#endif
		return ret;
	}
	if((ret = Sim_Net_cmd("AT+CIPHEAD=1","OK",200)) != 0) 
	{
		#ifdef DEBUG_MEG
			__DBG__("����IPͷʧ��!\r\n");
		#endif
		//���ý���������ʾIPͷ(�����ж�������Դ,���ڵ�·������Ч)
		return ret;
	}
	return ret;
 }
 /*******************************************************************************
 �� �� ����void TCP_Soc_Connet
 �����������������������
 ��ڲ�����soc:ͨ�� address:��������ַ
 ���ز�����0 �ɹ�
 ����ʱ��: 2018-01-12 by zam
 ********************************************************************************/
unsigned char TCP_Soc_Connet(tcp_soc_address address)
{
	int8_t ret = 0;
	char connect_buf[200];
	memset(connect_buf,0,sizeof(connect_buf));
	sprintf(connect_buf,"AT+CIPOPEN=0,\"TCP\",\"%s\",%d",address.hostName,address.port);
	if((ret = Sim_Net_cmd(connect_buf,"OPEN",1000)) != 0)
	{
		#ifdef DEBUG_MEG
			__DBG__("���ӳ���!\r\n");
		#endif
		return ret;
	}
	return 0;
}
 /*******************************************************************************
�� �� ����void TCP_Soc_Send
������������������
��ڲ�����send_buffer:���������� length:���ݳ���
���ز�����
����ʱ��: 2017-11-02 by zam
********************************************************************************/
unsigned char TCP_Soc_Send(char *send_buffer,unsigned int length)
{
	int8_t ret = 0;
	char sendbuf[1024];
	memset(sendbuf,0,sizeof(sendbuf));
	if((ret=Sim_Net_cmd("AT+CIPSEND=0,",">",1000))!=0)
	{
		#ifdef DEBUG_MEG
			__DBG__("�޷��������ݵ�������!\r\n");
		#endif
		return ret;
	}
	sprintf(sendbuf,"%s\32\0",send_buffer);
	if((ret=Sim_Net_cmd(send_buffer,"OK",1000))!=0)
	{
		#ifdef DEBUG_MEG
			__DBG__("�������ݴ���.....\r\n");
		#endif
		return ret;
	}
	return ret;
}
 /*******************************************************************************
 �� �� ����void qxwz_soc_close
 �����������ر�����ģ������
 ��ڲ�����
 ���ز�����
 ����ʱ��: 2017-11-02 by zam
 ********************************************************************************/
 unsigned char TPC_Soc_Close(void)
 {
	 int8_t ret = 0; 
	 ret = Sim_Net_cmd("AT+CIPCLOSE=0","CLOSE",2000);	 //�ر�����
	 if(ret != 0)
	 {
	 	#ifdef DEBUG_MEG
			__DBG__("�ر�socket����.....\r\n");
		#endif
		 return ret;
	 }
	 return ret;
 }

unsigned char Http_Soc_Send(tcp_soc_address address,char *send_buffer)
{
	int8_t ret = 0;
	char sendbuf[1024];
	char connect_buf[200];
	memset(sendbuf,0,sizeof(sendbuf));
	memset(connect_buf,0,sizeof(connect_buf));
	sprintf(connect_buf,"AT+CHTTPACT=\"%s\",%d",address.hostName,address.port);
	if((ret=Sim_Net_cmd(connect_buf,"REQUEST",2000))!=0)
	{
		#ifdef DEBUG_MEG
			__DBG__("�޷��������ݵ�������!\r\n");
		#endif
		return ret;
	}
	sprintf(sendbuf,"%s\32\0",send_buffer);//////�������Ҫ����һ��
	if((ret=Sim_Net_cmd(send_buffer,"OK",1000))!=0)
	{
		#ifdef DEBUG_MEG
			__DBG__("�������ݴ���.....\r\n");
		#endif
		return ret;
	}
	return ret;
}


