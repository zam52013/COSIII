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

#include "main.h"

#include <string.h>
#include <stdio.h>


char *ggastring="$GPGGA,102223.00,2232.4394,N,11355.4252,E,4,19,0.7,16.29,M,-3.50,M,01,0000*49\r\n";//right
unsigned char SEND_GPGGA_MEESG=0;
u8 Fram_Date_Buff[40];
////////////////////�����ջ����///////////////////////////////////
//�����ջ
OS_STK START_TASK_STK[START_STK_SIZE];
//�����ջ
OS_STK Tick_TASK_STK[Tick_STK_SIZE];
//�����ջ
OS_STK SENDGGA_TASK_STK[SENDGGA_STK_SIZE];
//�����ջ
OS_STK RTCM_TASK_STK[RTCM_STK_SIZE];
//�����ջ
OS_STK USART1_TASK_STK[USART1_STK_SIZE];
//�����ջ
OS_STK USART2_TASK_STK[USART2_STK_SIZE];
//�����ջ
OS_STK USART3_TASK_STK[USART3_STK_SIZE];
//�����ջ
OS_STK KEY_TASK_STK[KEY_STK_SIZE];
//�����ջ
OS_STK AD_TASK_STK[AD_STK_SIZE];

//�����ź���
OS_EVENT *USART1_Semp;
OS_EVENT *USART2_Semp;
OS_EVENT *USART3_Semp;
/////////////////////////////////////////////////////////////

/*********************************************/
task_flag Task_Flag;
/*********************************************/
u8 a=0x41;
int main(void)
{
//	config.appkey = "514267";
//	config.appsecret = "46766a50a7986f93ea54367d4bd38c815f2ced98fb589e4dececf2c62f4c4a2f";
//	config.device_ID = "EF004";
//	config.device_Type = "EFRTK4";   
	delay_init();	   	//��ʱ������ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2
	Usart_init();
	Spi_init();
	Ble_Bluetooth_Init();
	Task_Flag.Flag_Bit.Debug_Date=TRUE;
	Sim_contro();
	LED_Init();	 	//��ʼ����LED���ӵ�Ӳ���ӿ�
	WT588C_Init();
	All_Pow_Io_Init();
	Adc_Init();
	fm25v_init();
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
    
    OSTaskCreate(Tick_task, (void *)0, (OS_STK*)&Tick_TASK_STK[Tick_STK_SIZE - 1], Tick_TASK_PRIO);
    OSTaskCreate(sendgga_task, (void *)0, (OS_STK*)&SENDGGA_TASK_STK[SENDGGA_STK_SIZE - 1], SENDGGA_TASK_PRIO);
    OSTaskCreate(rtcm_task, (void *)0, (OS_STK*)&RTCM_TASK_STK[RTCM_STK_SIZE - 1], RTCM_TASK_PRIO);
	OSTaskCreate(USART2_task, (void *)0, (OS_STK*)&USART1_TASK_STK[USART1_STK_SIZE - 1], USART1_TASK_PRIO);
	OSTaskCreate(USART3_task, (void *)0, (OS_STK*)&USART3_TASK_STK[USART3_STK_SIZE - 1], USART3_TASK_PRIO);
	OSTaskCreate(KEY_task, (void *)0, (OS_STK*)&KEY_TASK_STK[KEY_STK_SIZE - 1], KEY_TASK_PRIO);
	OSTaskCreate(AD_task, (void *)0, (OS_STK*)&AD_TASK_STK[AD_STK_SIZE - 1], AD_TASK_PRIO);
	
	USART1_Semp = OSSemCreate (0);
	USART2_Semp = OSSemCreate (0);
	USART3_Semp = OSSemCreate (0);
	
	OSTaskSuspend(RTCM_TASK_PRIO); //������ʼ����.
	OSTaskSuspend(Tick_TASK_PRIO); //������ʼ����.
	
    OSTaskSuspend(START_TASK_PRIO); //������ʼ����.
    OS_EXIT_CRITICAL();             //�˳��ٽ���(���Ա��жϴ��)
}

//LED0����,������ָʾ�����Ƿ���������
void Tick_task(void *pdata) 
{
	
  while(1) 
	{
    	delay_ms(1000);
		timer_tick(TIMER_FEQ);
		restart_connect();			  
   };
}
//����GGA����
void sendgga_task(void *pdata)
{	  
	//char *gga_addr;
	static unsigned int i=0;
	while(1)
	{
		delay_ms(100);//ÿ��һ��ģ����յ�һ������
		//printf("%s\r\n",&a);
		LED_STATU_ON_OFF;
		//printf("����ִ�д���=%d\r\n",i++);
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
void KEY_task(void *pdata)//������
{
	unsigned char KEY_ST;
	WTH_2L(0x0a0140); //оƬ��ʼ��
	while(1) 
	{
    	delay_ms(10);//ÿ��һ��ģ����������һ��tick
    	KEY_ST=Key_Scan();
		if(KEY_ST==KEY_LONG_DOWN)
		{
			if(!Task_Flag.Flag_Bit.Pow_On)
			{
				Task_Flag.Flag_Bit.Pow_On=!Task_Flag.Flag_Bit.Pow_On;
				POWER_ON;//����
				Play_voice(BOOT_BEGIN);
			}
			if(Task_Flag.Flag_Bit.Pow_Off)
			{
				LED_STATU_OFF;
				Task_Flag.Flag_Bit.Pow_Off=!Task_Flag.Flag_Bit.Pow_Off;
				Play_voice(SHUT_DOWN);
				delay_ms(2000);//ÿ��һ��ģ����������һ��tick
				POWER_OFF;//�ػ�
				OSTaskDel(Tick_TASK_PRIO);	//ɾ������
				OSTaskDel(SENDGGA_TASK_PRIO);//ɾ������
				OSTaskDel(RTCM_TASK_PRIO);//ɾ������
				OSTaskDel(AD_TASK_PRIO);//ɾ������
			}
		}
		else if(KEY_ST==KEY_LONG_UP)
		{
			Task_Flag.Flag_Bit.Pow_Off=!Task_Flag.Flag_Bit.Pow_Off;
		}
  	}
}
void AD_task(void *pdata)//������
{
	u16 AD_VALUE;
	while(1) 
	{
    	delay_ms(2000);//ÿ��һ��ģ����������һ��tick
    	AD_VALUE=AD_Filter(ADC1,AD_CH_POW);
		Ad_Led_statu(AD_VALUE);
  }
}

void USART2_task(void *pdata)
{
	INT8U err;
	char *des_mult=NULL;//����Ѱַ��ַ
	while(1)
	{
		OSSemPend(USART2_Semp,0,&err); //�����ź���
		if(BLUETOOTH_MESSG.receve_ok)
		{
			if(Task_Flag.Flag_Bit.Debug_Date)
			{
				UART_SendString(BLUETOOTH_USART,&BLUETOOTH_MESSG.messg_buff);//���������ݷ�������
			}
			if((strstr(BLUETOOTH_MESSG.messg_buff,"$efrtk"))!=NULL)//�յ���ȡ���˻�
			{
				if(!Get_qx_count(&BLUETOOTH_MESSG.messg_buff[0]))
				{
					if(!Task_Flag.Flag_Bit.Get_Cunt_Ok)
					{
						Task_Flag.Flag_Bit.Get_Cunt_Ok=TRUE;
						UART_SendString(BLUETOOTH_USART,"$GETACCOUNT OK\r\n");//��������
						Play_voice(GET_NET_OK);
						qxwz_setting(&config);
						//qxwz_prefs_flags_set(QXWZ_PREFS_FLAG_ENABLE_LOG);		//log���ʹ��
						//qxwz_prefs_flags_set(QXWZ_PREFS_FLAG_SOCKET_ASYN);	//log��sokectͬ�����ʹ��
  						//qxwz_prefs_flags_set(QXWZ_PREFS_FLAG_ENABLE_LOG|QXWZ_PREFS_FLAG_SOCKET_ASYN);
						qxwz_start(qxwz_rtcm_response_callback,qxwz_status_response_callback);//����rtcm 
						OSTaskResume(RTCM_TASK_PRIO);//�ָ�����
						OSTaskResume(Tick_TASK_PRIO);//�ָ�����
						Task_Flag.Flag_Bit.Send_Gpgga=TRUE;//���Է���GPGGA����
					}
				}
			}
			else if((strstr(BLUETOOTH_MESSG.messg_buff,"$NQX"))!=NULL)	//���ó���ͨ��վָ��
			{
				UART_SendString(GGA_USART,"$NQX\r\n");	//�����װ�ȥ����
			}
			else if((strstr(BLUETOOTH_MESSG.messg_buff,"$NET_INSTRUMENT"))!=NULL)	//���ó��Ʋ����
			{
				UART_SendString(GGA_USART,"$NET_INSTRUMENT\r\n");	//�����װ�ȥ����
			}
			else if((strstr(BLUETOOTH_MESSG.messg_buff,"$CONFIG"))!=NULL)	//�ƶ�վ�Ͳ���ǵ�����ָ��
			{
				UART_SendString(GGA_USART,"$CONFIG\r\n");	//�����װ�ȥ����
			}
			else if((strstr(BLUETOOTH_MESSG.messg_buff,"$CBASE"))!=NULL)	//��վ������ָ��
			{
				UART_SendString(GGA_USART,"$CBASE\r\n");	//�����װ�ȥ����
			}
			else if((strstr(BLUETOOTH_MESSG.messg_buff,"$DEBUG"))!=NULL)	//��վ������ָ��
			{
				Task_Flag.Flag_Bit.Debug_Date=TRUE;//������������ģʽ
			}
			else if((des_mult=strstr(BLUETOOTH_MESSG.messg_buff,"$NAME"))!=NULL)//�޸���������
			{	
				SendCmdViaBt(des_mult+5,5);//set bluetooth name
			}
			else if((des_mult=strstr(BLUETOOTH_MESSG.messg_buff,"$NET"))!=NULL)//�����ַ
			{
				Fram_Date_Buff[0]=BLUETOOTH_MESSG.date_lenth+2;
				memcpy(&Fram_Date_Buff[1],&BLUETOOTH_MESSG.messg_buff[0],BLUETOOTH_MESSG.date_lenth);
				Fram_Date_Buff[Fram_Date_Buff[0]]=0x20;
				
				//Fram_read(Move_Station_Net_Len_Addr,0x0001,&Fram_Date_Buff[0]);
				//Fram_read(Move_Station_Net_Addr,Fram_Date_Buff[0]-1,&Fram_Date_Buff[1]);//�����ַд��
				UART_SendString(BLUETOOTH_USART,&Fram_Date_Buff[1]);//��������
				UART_SendString(GGA_USART,des_mult);	//�����װ�ȥ����
			}
			else if((des_mult=strstr(BLUETOOTH_MESSG.messg_buff,"$UNION"))!=NULL)//��Ԫ��ַ
			{
				Fram_Date_Buff[0]=BLUETOOTH_MESSG.date_lenth+2;
				memcpy(&Fram_Date_Buff[1],&BLUETOOTH_MESSG.messg_buff[0],BLUETOOTH_MESSG.date_lenth);
				Fram_Date_Buff[Fram_Date_Buff[0]]=0x20;
				UART_SendString(GGA_USART,des_mult);	//�����װ�ȥ����
			}
			else if(strstr(BLUETOOTH_MESSG.messg_buff,"$RECOVER")!=NULL)//��λ����
			{
				UART_SendString(GGA_USART,"$RECOVER\r\n");	//�����װ�ȥ����
			}
			else if((strstr(BLUETOOTH_MESSG.messg_buff,"Name OK"))!=NULL)//���������޸ĳɹ�
			{	
				Play_voice(SET_NAME);
			}
			//UART_SendString(BLUETOOTH_USART,&BLUETOOTH_MESSG.messg_buff[0]);//�������� ������
			BLUETOOTH_MESSG.receve_ok=FALSE;
		}
	}
}

void USART3_task(void *pdata) 
{	
	INT8U err;
	char *gga_addr;
  	while(1) 
	{	
		OSSemPend(USART3_Semp,0,&err); //�����ź���
		if(GGA_MESS.receve_ok)
		{
			if(Task_Flag.Flag_Bit.Debug_Date)
			{
				UART_SendString(BLUETOOTH_USART,&GGA_MESS.messg_buff[0]);//���������ݷ�������
			}
			if((gga_addr=strstr(GGA_MESS.messg_buff,"$GPGGA"))!=NULL)
			{
				UART_SendString(BLUETOOTH_USART,&GGA_MESS.messg_buff[0]);//���������ݷ�������
				RTK_Status(&GGA_MESS.messg_buff[0],GGA_MESS.date_lenth);
				if((strstr(GGA_MESS.messg_buff,"*66")==NULL)&&
					((strstr(GGA_MESS.messg_buff,"E"))!=NULL))	//���ݴ��ڲ��ǿ�����
				{
					if(Task_Flag.Flag_Bit.Send_Gpgga)
					{
						qxwz_send_gga(gga_addr);
					}
				}
			}
			else if((strstr(GGA_MESS.messg_buff,"$CLOSENET"))!=NULL)
			{
				UART_SendString(BLUETOOTH_USART,"$CLOSENET OK\r\n");//���������ݷ�������
				Task_Flag.Flag_Bit.Get_Cunt_Ok=TRUE;//��ͨ�̶���վ����д�˻�
				Task_Flag.Flag_Bit.Send_Gpgga=FALSE;
				qxwz_stop();
				OSTaskDel(Tick_TASK_PRIO);//ɾ������
				OSTaskDel(RTCM_TASK_PRIO);//ɾ������
				Init_Sim=TRUE;
				SIM_POWER_OFF;
			}
			else if(((strstr(GGA_MESS.messg_buff,"$QXRTK"))!=NULL)
					||((strstr(GGA_MESS.messg_buff,"$EFRTK"))!=NULL))
			{
				UART_SendString(BLUETOOTH_USART,&GGA_MESS.messg_buff[0]);//�������ݷ�������
				if(!Task_Flag.Flag_Bit.Send_Rtcm_Flag)
				{
					Play_voice(SEND_Data);
					Task_Flag.Flag_Bit.Send_Rtcm_Flag=TRUE;//��ʼ��������
				}
			}
			else if((strstr(GGA_MESS.messg_buff,"$NQX OK"))!=NULL)//��ͨ�̶���վ���óɹ�
			{
				Play_voice(NNET_BASE);
				UART_SendString(BLUETOOTH_USART,"$NQX OK\r\n");//���������ݷ�������
			}
			else if((strstr(GGA_MESS.messg_buff,"$NET_INSTRUMENT OK"))!=NULL)//�Ʋ�������óɹ�
			{
				Play_voice(NET_INSTRUMENT);
				UART_SendString(BLUETOOTH_USART,"$NET_INSTRUMENT OK\r\n");//���������ݷ�������
			}
			else if((strstr(GGA_MESS.messg_buff,"$CONFIG OK"))!=NULL)//�ƶ�վ��������ģʽ
			{
				Play_voice(GET_CMD);
				Task_Flag.Flag_Bit.Move_Station=!Task_Flag.Flag_Bit.Move_Station;
				UART_SendString(BLUETOOTH_USART,"$CONFIG OK\r\n");//���������ݷ�������
			}
			else if((strstr(GGA_MESS.messg_buff,"$CBASE OK"))!=NULL)//��վ��������ģʽ
			{
				Play_voice(GET_CMD);
				UART_SendString(BLUETOOTH_USART,"$CBASE OK\r\n");//���������ݷ�������
			}
			else if((strstr(GGA_MESS.messg_buff,"configuration"))!=NULL)//��������ģʽ
			{
				qxwz_stop();
				OSTaskDel(AD_TASK_PRIO); //������ʼ����.
				OSTaskDel(RTCM_TASK_PRIO); //������ʼ����.
				OSTaskDel(Tick_TASK_PRIO); //������ʼ����.
				Play_voice(CONFIG_ING);
			}
			else if((strstr(GGA_MESS.messg_buff,"$net save OK"))!=NULL)//�����ַ���óɹ�
			{
				if(Task_Flag.Flag_Bit.Move_Station)
				{
					Fram_wirte(Move_Station_Net_Len_Addr,Fram_Date_Buff[0],&Fram_Date_Buff[0]);//�����ַд��
				}
				else
				{
					Fram_wirte(Base_Station_Net_Len_Addr,Fram_Date_Buff[0],&Fram_Date_Buff[0]);//�����ַд��
				}
				Play_voice(CONFIG_WILESS_ADDR_OK);
				UART_SendString(BLUETOOTH_USART,"$net save OK\r\n");//���������ݷ�������
			}
			else if((strstr(GGA_MESS.messg_buff,"$union save OK"))!=NULL)//��Ԫ��ַ���óɹ�
			{
				Fram_wirte(Move_Station_Union_Len_Addr,Fram_Date_Buff[0],&Fram_Date_Buff[0]);//��Ԫ��ַд��Fram ֻ���ƶ�վ�����
				
				Play_voice(CONFIG_WILESS_UNIO_OK);
				UART_SendString(BLUETOOTH_USART,"$union save OK\r\n");//���������ݷ�������
			}
			else if((strstr(GGA_MESS.messg_buff,"$recover save OK"))!=NULL)//�ָ���������
			{
				Play_voice(WILESS_RECOVER);
				UART_SendString(BLUETOOTH_USART,"$recover save OK\r\n");//���������ݷ�������
			}
			else if((strstr(GGA_MESS.messg_buff,"$"))!=NULL)//����ָ��
			{
				Play_voice(NONE_DATE);
				//UART_SendString(BLUETOOTH_USART,&GGA_MESS.messg_buff[0]);//���������ݷ�������
			}
			GGA_MESS.receve_ok=FALSE;
		}
   };
}


