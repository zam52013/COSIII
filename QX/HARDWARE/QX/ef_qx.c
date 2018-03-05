/*
 * File      : ef_qx.c
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

#include "ef_qx.h"
#include "sim7600.h"
#include "main.h"
#include "bluetooth.h"

qxwz_config config;
unsigned char Init_Sim=0;   //��ʼ����־
qian_xun QIAN_XUN;
gpgga GPGGA;
rtkposa GET_POSA;
rtkposa RTKPOSA_BUFF[3];

/*******************************************************************************
�� �� ����void qxwz_status_response_callback(void)
���������� ��ѯ�û��˺���Ϣ
��ڲ�����							
���ز������˻���Ϣ
����ʱ��: 2017-11-02 by zam
********************************************************************************/
void qxwz_status_response_callback(qxwz_rtcm_status_code status)
{
	
	printf("QXWZ_STATUS =%d",status);
	switch(status)
	{
		case QXWZ_NET_WORK_ERROR:
			UART_SendString(BLUETOOTH_USART,"$QXWZ_NET_WORK_ERROR\r\n");
			break;
		case QXWZ_APPKEY_IDENTIFY_FAIL:
			UART_SendString(BLUETOOTH_USART,"$QXWZ_APPKEY_IDENTIFY_FAIL\r\n");
			break;
		case QXWZ_DEVICEID_NULL:
			UART_SendString(BLUETOOTH_USART,"$QXWZ_DEVICEID_NULL\r\n");
			break;
		case QXWZ_STATUS_OPENAPI_ACCOUNT_NOT_EXIST:
			UART_SendString(BLUETOOTH_USART,"$QXWZ_STATUS_OPENAPI_ACCOUNT_NOT_EXIST\r\n");
			break;
		case QXWZ_STATUS_OPENAPI_DUPLICATE_ACCOUNT:
			UART_SendString(BLUETOOTH_USART,"$QXWZ_STATUS_OPENAPI_DUPLICATE_ACCOUNT\r\n");
			break;
		case QXWZ_STATUS_OPENAPI_DISABLED_ACCOUNT:
			UART_SendString(BLUETOOTH_USART,"$QXWZ_STATUS_OPENAPI_DISABLED_ACCOUNT\r\n");
			break;
		case QXWZ_STATUS_OPENAPI_ACCOUNT_EXPIRED:
			UART_SendString(BLUETOOTH_USART,"$QXWZ_STATUS_OPENAPI_ACCOUNT_EXPIRED\r\n");
			break;
		case QXWZ_STATUS_OPENAPI_ACCOUNT_TOEXPIRE:
			UART_SendString(BLUETOOTH_USART,"$QXWZ_STATUS_OPENAPI_ACCOUNT_TOEXPIRE\r\n");
			break;
		case QXWZ_STATUS_NTRIP_UNAUTHORIZED:
			UART_SendString(BLUETOOTH_USART,"$QXWZ_STATUS_NTRIP_UNAUTHORIZED\r\n");
			break;
		default:
			break;
	}
	if(status == QXWZ_STATUS_OPENAPI_ACCOUNT_TOEXPIRE)
	{
		printf("expire_time=%d\n",getqxwzAccount()->expire_time);
	}
}
/*******************************************************************************
�� �� ����void get_qxwz_sdk_account_info(void)
���������� ��ѯ�û��˺�״̬
��ڲ�����							
���ز�����
����ʱ��: 2017-11-02 by zam
********************************************************************************/
void  get_qxwz_sdk_account_info(void)
{
	qxwz_account_info *p_account_info = NULL;
	p_account_info = getqxwzAccount();
	if(p_account_info->appkey != NULL) 
	{
		printf("appkey=%s\n",p_account_info->appkey);
	}
	if(p_account_info->NtripPassword != NULL) 
	{
		printf("NtripPassword=%s\n",p_account_info->NtripPassword);
	}
}
/*******************************************************************************
�� �� ����void qxwz_rtcm_response_callback(void)
���������� ��ӡ�������
��ڲ�����							
���ز�����
����ʱ��: 2017-11-02 by zam
********************************************************************************/
void qxwz_rtcm_response_callback(char* rtcm, size_t length)
{
	uint16_t i=0;
	unsigned int total_recv_bytes = 0;
	total_recv_bytes += length;
	for(;i<length;i++)
	{
		//printf("%02x",rtcm[i]);
		//printf(" ");
		while(USART_GetFlagStatus(RTCM_USART, USART_FLAG_TC)==RESET); 
		USART_SendData(RTCM_USART ,rtcm[i]);//���͵�ǰ�ַ�
	}
	//printf("total_recv_bytes = %d\n",total_recv_bytes);
	//printf("\n");
}
/*******************************************************************************
�� �� ����void Get_qx_count
���������� ��ȡ�ƶ��˻�
��ڲ�����	����						
���ز�����1��ȡʧ��  0��ȡ�ɹ�
����ʱ��: 2017-11-02 by zam
********************************************************************************/
unsigned char Get_qx_count(char *str)
{
	char *appk_addr,*apps_addr,*devi_id_addr,*devi_type_addr,*nor_addr;
	unsigned char cnt_addr=0;
	if((appk_addr=strstr(str,"appkey"))!=NULL)
	{
		if((apps_addr=strstr(str,"appsecret"))!=NULL)
		{
			if((devi_id_addr=strstr(str,"device_ID"))!=NULL)
			{
				if((devi_type_addr=strstr(str,"device_Type"))!=NULL)
				{		
					if((nor_addr=strstr(str,"\r\n"))!=NULL)
					{	
							cnt_addr=apps_addr-appk_addr-7;
							memcpy(QIAN_XUN.appk_buff,appk_addr+7,cnt_addr);
							QIAN_XUN.appk_buff[cnt_addr-1]='\0';
							printf("%s\r\n",QIAN_XUN.appk_buff);
						
							cnt_addr=devi_id_addr-apps_addr-10;
							memcpy(QIAN_XUN.apps_buff,apps_addr+10,cnt_addr);
							QIAN_XUN.apps_buff[cnt_addr-1]='\0';
							printf("%s\r\n",QIAN_XUN.apps_buff);
						
							cnt_addr=devi_type_addr-devi_id_addr-10;
							memcpy(QIAN_XUN.id_buff,devi_id_addr+10,cnt_addr);
							QIAN_XUN.id_buff[cnt_addr-1]='\0';
							printf("%s\r\n",QIAN_XUN.id_buff);
						
							cnt_addr=nor_addr-devi_type_addr-11;
							memcpy(QIAN_XUN.type_buff,devi_type_addr+12,cnt_addr);
							QIAN_XUN.type_buff[cnt_addr-1]='\0';
							printf("%s\r\n",QIAN_XUN.type_buff);
						
							config.appkey = &QIAN_XUN.appk_buff[0];
							config.appsecret =&QIAN_XUN.apps_buff[0] ;
							config.device_ID = &QIAN_XUN.id_buff[0];
							config.device_Type =&QIAN_XUN.type_buff[0];
							return 0;
						}
						else
						{
							return 1;
						}
				}
				else
				{
					return 1;
				}
			}
			else
			{
				return 1;
			}
		}
		else
		{
			return 1;
		}
	}
	else
	{
		return 1;
	}
}

/********************************************************************
***��������:Nmea_parse_GGA
***����˵��:GGA����֡����
***�������:buffe��ȡ�ļĴ���  buff_len �ܳ���
***�������:1:��ȡ�ɹ�	0:��ȡʧ��
***
********************************************************************/
unsigned Nmea_parse_GGA(char* buffe,int buff_len)
{
	int i=6;
	if(buffe[i]==','&&buffe[++i]==',')
	{
		return 0;
	}
	GPGGA.utc=strtod(&buffe[i],NULL);
	while(buffe[i++]!=',')
	{
		if(i>buff_len)
		{
			return 0;
		}
	}
	GPGGA.lat=strtod(&buffe[i],NULL);
	while(buffe[i++]!=',')
	{
		if(i>buff_len)
		{
			return 0;
		}
	}
	GPGGA.lat_dir=buffe[i];
	while(buffe[i++]!=',')
	{
		if(i>buff_len)
		{
			return 0;
		}
	}
	GPGGA.lon=strtod(&buffe[i],NULL);
	while(buffe[i++]!=',')
	{
		if(i>buff_len)
		{
			return 0;
		}
	}
	GPGGA.lon_dir=buffe[i];
	while(buffe[i++]!=',')
	{
		if(i>buff_len)
		{
			return 0;
		}
	}
	GPGGA.quality=atoi(&buffe[i]);
	while(buffe[i++]!=',')
	{
		if(i>buff_len)
		{
			return 0;
		}
	}
	GPGGA.sats=atoi(&buffe[i]);
	while(buffe[i++]!=',')
	{
		if(i>buff_len)
		{
			return 0;
		}
	}
	GPGGA.hdop=strtof(&buffe[i],NULL);
	while(buffe[i++]!=',')
	{
		if(i>buff_len)
		{
			return 0;
		}
	}
	GPGGA.alt=strtof(&buffe[i],NULL);
	while(buffe[i++]!=',')
	{
		if(i>buff_len)
		{
			return 0;
		}
	}
	GPGGA.a_units=buffe[i];
	while(buffe[i++]!=',')
	{
		if(i>buff_len)
		{
			return 0;
		}
	}
	GPGGA.undulation=strtof(&buffe[i],NULL);
	while(buffe[i++]!=',')
	{
		if(i>buff_len)
		{
			return 0;
		}
	}
	GPGGA.u_units=buffe[i];
	while(buffe[i++]!=',')
	{
		if(i>buff_len)
		{
			return 0;
		}
	}
	GPGGA.age=atoi(&buffe[i]);
	return 1;
}
/********************************************************************
***��������:Parse_EF
***����˵��:��ȡ�ƶ����ݲ�����
***�������:buffe��ȡ�ļĴ���  buff_len �ܳ���
***�������:1:��ȡ�ɹ�	0:��ȡʧ��
***
********************************************************************/
unsigned Parse_EF(char* buffe,int buff_len)
{
	char *str;
	int i=0;
	str=strchr(buffe,',');
	str++;
	if(str)
	{
		GET_POSA.latitude=strtod(str,NULL);
		while(*(str++)!=',')
		{
			i++;
			if(i>buff_len)
			{
				return 0;
			}
		}
		GET_POSA.longitude=strtod(str,NULL);
		while(*(str++)!=',')
		{
			i++;
			if(i>buff_len)
			{
				return 0;
			}
		}
		GET_POSA.hgt=strtod(str,NULL);
		return 1;
	}
	else
	{
		return 0;
	}
}
/********************************************************************
***��������:Conmpera_RTK
***����˵��:���ݿ�������
***�������:buffe��ȡ�ļĴ��� 
***�������:��
***
********************************************************************/
void Conmpera_RTK(rtkposa * rtk_buff)
{
	RTKPOSA_BUFF[2]=RTKPOSA_BUFF[1];
	RTKPOSA_BUFF[1]=RTKPOSA_BUFF[0];
	RTKPOSA_BUFF[0]=*rtk_buff;
}
/********************************************************************
***��������:RTK_Status(char* buffe,int buff_len)
***����˵��:RTK GPGGA���ݽ���
***�������:buffe��ȡ�ļĴ���  buff_len���ݳ���
***�������:��
***
********************************************************************/

void RTK_Status(char* buffe,int buff_len)
{
	static unsigned char Solution_stat=0;//����״̬����
	if((Nmea_parse_GGA(buffe,buff_len))&&(Solution_stat!=GPGGA.quality))
	{
		switch(GPGGA.quality)
		{
			case 1:
				Play_voice(SING_Point);
				LED_STATU_OFF;
				break;
			case 2:
				Play_voice(PSEU_Diff);
				LED_STATU_OFF;
				break;
			case 4:
				Play_voice(RTK_FIX);
				LED_STATU_ON;
				break;
			case 5:
				Play_voice(RTK_Float);
				LED_STATU_ON_OFF;
				break;
			default:
				LED_STATU_OFF;
				break;
		}
		Solution_stat=GPGGA.quality;
	}
}

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
							
							OSSemPost(USART3_Semp);  //�����ź���
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

