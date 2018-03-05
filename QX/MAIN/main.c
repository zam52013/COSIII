/*
 * File      : main.c
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
#include "delay.h"

#include "includes.h"
#include "usart.h"
#include "qxwz_rtcm.h"
#include "socket.h"
#include "prefs.h"

#include "main.h"
#include "sim7600.h"
#include "ef_qx.h"
#include <string.h>

char *ggastring="$GPGGA,102223.00,2232.4394,N,11355.4252,E,4,19,0.7,16.29,M,-3.50,M,01,0000*49\r\n";//right

////////////////////�����ջ����///////////////////////////////////
//�����ջ
OS_STK START_TASK_STK[START_STK_SIZE];
//�����ջ
OS_STK LED0_TASK_STK[LED0_STK_SIZE];
//�����ջ
OS_STK SENDGGA_TASK_STK[SENDGGA_STK_SIZE];
//�����ջ
OS_STK RTCM_TASK_STK[RTCM_STK_SIZE];
/////////////////////////////////////////////////////////////
int main(void)
{
	
//	config.appkey = "514267";
//	config.appsecret = "46766a50a7986f93ea54367d4bd38c815f2ced98fb589e4dececf2c62f4c4a2f";
//	config.device_ID = "EF004";
//	config.device_Type = "EFRTK4";
	
  	//qxwz_prefs_flags_set(QXWZ_PREFS_FLAG_ENABLE_LOG);		//log���ʹ��
	//qxwz_prefs_flags_set(QXWZ_PREFS_FLAG_SOCKET_ASYN);	//log��sokectͬ�����ʹ��
  	//qxwz_prefs_flags_set(QXWZ_PREFS_FLAG_ENABLE_LOG|QXWZ_PREFS_FLAG_SOCKET_ASYN);
     
	delay_init();	   	//��ʱ������ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2
	Usart_init();
	Sim_contro();
	LED_Init();	 	//��ʼ����LED���ӵ�Ӳ���ӿ�
	while(1)
	{
		if(ACCOUNT_MESS.receve_ok)
		{
			ACCOUNT_MESS.receve_ok=FALSE;
			if(!Get_qx_count(&ACCOUNT_MESS.messg_buff[0]))
			{
				break;
			}
		}
	}
	qxwz_setting(&config);
	qxwz_start(qxwz_rtcm_response_callback,qxwz_status_response_callback);//����rtcm 
	OSInit();   
	OSTaskCreate(start_task,(void *)0,(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO );//������ʼ����
	OSStart();
}
//��ʼ����
void start_task(void *pdata) 
{
    OS_CPU_SR cpu_sr = 0;
    pdata = pdata;
    OS_ENTER_CRITICAL();            //�����ٽ���(�޷����жϴ��)
    OSTaskCreate(led0_task, (void *)0, (OS_STK*)&LED0_TASK_STK[LED0_STK_SIZE - 1], LED0_TASK_PRIO);
    OSTaskCreate(sendgga_task, (void *)0, (OS_STK*)&SENDGGA_TASK_STK[SENDGGA_STK_SIZE - 1], SENDGGA_TASK_PRIO);
    OSTaskCreate(rtcm_task, (void *)0, (OS_STK*)&RTCM_TASK_STK[RTCM_STK_SIZE - 1], RTCM_TASK_PRIO);
    OSTaskSuspend(START_TASK_PRIO); //������ʼ����.
    OS_EXIT_CRITICAL();             //�˳��ٽ���(���Ա��жϴ��)
}

//LED0����,������ָʾ�����Ƿ���������
void led0_task(void *pdata) 
{
	static unsigned int i=0;
  while(1) 
	{
    	LED0_ON;
    	delay_ms(500);
    	LED0_OFF;
    	delay_ms(500);
			timer_tick(TIMER_FEQ);
			restart_connect();		
			printf("����ִ�д���=%d\r\n",i++);	  
   };
}
//����GGA����
void sendgga_task(void *pdata)
{	  
	char *gga_addr;
	while(1)
	{
		delay_ms(20);//ÿ��һ��ģ����յ�һ������
//		qxwz_send_gga(ggastring);
		if(GGA_MESS.receve_ok)
		{
			if((gga_addr=strstr(GGA_MESS.messg_buff,"$GPGGA"))!=NULL)
			{
				if(strstr(GGA_MESS.messg_buff,"*66")==NULL)	//���ݴ��ڲ��ǿ�����
				{
						qxwz_send_gga(gga_addr);
				}
			}
			else if((gga_addr=strstr(GGA_MESS.messg_buff,"$CLOSENET"))!=NULL)
			{
				OSTaskDel(LED0_TASK_PRIO);//ɾ������
				OSTaskDel(RTCM_TASK_PRIO);//ɾ������
				SIM_POWER_OFF;
			}
			GGA_MESS.receve_ok=FALSE;
		}
	}
}


void rtcm_task(void *pdata) 
{
	uint32_t qxwz_tick_cnt = 0;
	static int curretnTime = 0;
	qxwz_soc_close(0);
	qxwz_soc_close(1);
  while(1) 
	{
    delay_ms(1000);//ÿ��һ��ģ����������һ��tick
    curretnTime += 1000;
		qxwz_tick_cnt++;
		async_notify();
    qxwz_tick(curretnTime);
		//get_qxwz_sdk_account_info();
  }
}



