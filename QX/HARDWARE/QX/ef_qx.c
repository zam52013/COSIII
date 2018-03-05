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

qxwz_config config;
qian_xun QIAN_XUN;
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
	char appk_buff[20],apps_buff[100],id_buff[20],type_buff[20];
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
//							memcpy(appk_buff,appk_addr+7,cnt_addr);
//							appk_buff[cnt_addr-1]='\0';
//							printf("%s\r\n",appk_buff);
							memcpy(QIAN_XUN.appk_buff,appk_addr+7,cnt_addr);
							QIAN_XUN.appk_buff[cnt_addr-1]='\0';
							printf("%s\r\n",QIAN_XUN.appk_buff);
						
							cnt_addr=devi_id_addr-apps_addr-10;
//							memcpy(apps_buff,apps_addr+10,cnt_addr);
//							apps_buff[cnt_addr-1]='\0';
//							printf("%s\r\n",apps_buff);
							memcpy(QIAN_XUN.apps_buff,apps_addr+10,cnt_addr);
							QIAN_XUN.apps_buff[cnt_addr-1]='\0';
							printf("%s\r\n",QIAN_XUN.apps_buff);
						
							cnt_addr=devi_type_addr-devi_id_addr-10;
//							memcpy(id_buff,devi_id_addr+10,cnt_addr);
//							id_buff[cnt_addr-1]='\0';
//							printf("%s\r\n",id_buff);
							memcpy(QIAN_XUN.id_buff,devi_id_addr+10,cnt_addr);
							QIAN_XUN.id_buff[cnt_addr-1]='\0';
							printf("%s\r\n",QIAN_XUN.id_buff);
						
							cnt_addr=nor_addr-devi_type_addr-11;
//							memcpy(type_buff,devi_type_addr+12,cnt_addr);
//							type_buff[cnt_addr-1]='\0';
//							printf("%s\r\n",type_buff);
							memcpy(QIAN_XUN.type_buff,devi_type_addr+12,cnt_addr);
							QIAN_XUN.type_buff[cnt_addr-1]='\0';
							printf("%s\r\n",QIAN_XUN.type_buff);
						
//							config.appkey = &appk_buff[0];
//							config.appsecret =&apps_buff[0] ;
//							config.device_ID = &id_buff[0];
//							config.device_Type =&type_buff[0];
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
