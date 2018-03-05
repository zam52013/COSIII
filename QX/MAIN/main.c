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
////////////////////任务堆栈设置///////////////////////////////////
//任务堆栈
OS_STK START_TASK_STK[START_STK_SIZE];
//任务堆栈
OS_STK Tick_TASK_STK[Tick_STK_SIZE];
//任务堆栈
OS_STK SENDGGA_TASK_STK[SENDGGA_STK_SIZE];
//任务堆栈
OS_STK RTCM_TASK_STK[RTCM_STK_SIZE];
//任务堆栈
OS_STK USART1_TASK_STK[USART1_STK_SIZE];
//任务堆栈
OS_STK USART2_TASK_STK[USART2_STK_SIZE];
//任务堆栈
OS_STK USART3_TASK_STK[USART3_STK_SIZE];
//任务堆栈
OS_STK KEY_TASK_STK[KEY_STK_SIZE];
//任务堆栈
OS_STK AD_TASK_STK[AD_STK_SIZE];

//创建信号量
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
	delay_init();	   	//延时函数初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	Usart_init();
	Spi_init();
	Ble_Bluetooth_Init();
	Task_Flag.Flag_Bit.Debug_Date=TRUE;
	Sim_contro();
	LED_Init();	 	//初始化与LED连接的硬件接口
	WT588C_Init();
	All_Pow_Io_Init();
	Adc_Init();
	fm25v_init();
	OSInit(); 
	OSTaskCreate(start_task,(void *)0,(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO );//创建起始任务
	OSStart();
}
//开始任务
void start_task(void *pdata) 
{
    OS_CPU_SR cpu_sr = 0;
    pdata = pdata;
    OS_ENTER_CRITICAL();            //进入临界区(无法被中断打断)
    
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
	
	OSTaskSuspend(RTCM_TASK_PRIO); //挂起起始任务.
	OSTaskSuspend(Tick_TASK_PRIO); //挂起起始任务.
	
    OSTaskSuspend(START_TASK_PRIO); //挂起起始任务.
    OS_EXIT_CRITICAL();             //退出临界区(可以被中断打断)
}

//LED0任务,用来判指示程序是否正在运行
void Tick_task(void *pdata) 
{
	
  while(1) 
	{
    	delay_ms(1000);
		timer_tick(TIMER_FEQ);
		restart_connect();			  
   };
}
//发送GGA任务
void sendgga_task(void *pdata)
{	  
	//char *gga_addr;
	static unsigned int i=0;
	while(1)
	{
		delay_ms(100);//每隔一秒模拟接收到一次数据
		//printf("%s\r\n",&a);
		LED_STATU_ON_OFF;
		//printf("任务执行次数=%d\r\n",i++);
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
    delay_ms(1000);//每隔一秒模拟宿主触发一次tick
    curretnTime += 1000;
		qxwz_tick_cnt++;
		async_notify();
    qxwz_tick(curretnTime);
		//get_qxwz_sdk_account_info();
  }
}
void KEY_task(void *pdata)//任务函数
{
	unsigned char KEY_ST;
	WTH_2L(0x0a0140); //芯片初始化
	while(1) 
	{
    	delay_ms(10);//每隔一秒模拟宿主触发一次tick
    	KEY_ST=Key_Scan();
		if(KEY_ST==KEY_LONG_DOWN)
		{
			if(!Task_Flag.Flag_Bit.Pow_On)
			{
				Task_Flag.Flag_Bit.Pow_On=!Task_Flag.Flag_Bit.Pow_On;
				POWER_ON;//开机
				Play_voice(BOOT_BEGIN);
			}
			if(Task_Flag.Flag_Bit.Pow_Off)
			{
				LED_STATU_OFF;
				Task_Flag.Flag_Bit.Pow_Off=!Task_Flag.Flag_Bit.Pow_Off;
				Play_voice(SHUT_DOWN);
				delay_ms(2000);//每隔一秒模拟宿主触发一次tick
				POWER_OFF;//关机
				OSTaskDel(Tick_TASK_PRIO);	//删除任务
				OSTaskDel(SENDGGA_TASK_PRIO);//删除任务
				OSTaskDel(RTCM_TASK_PRIO);//删除任务
				OSTaskDel(AD_TASK_PRIO);//删除任务
			}
		}
		else if(KEY_ST==KEY_LONG_UP)
		{
			Task_Flag.Flag_Bit.Pow_Off=!Task_Flag.Flag_Bit.Pow_Off;
		}
  	}
}
void AD_task(void *pdata)//任务函数
{
	u16 AD_VALUE;
	while(1) 
	{
    	delay_ms(2000);//每隔一秒模拟宿主触发一次tick
    	AD_VALUE=AD_Filter(ADC1,AD_CH_POW);
		Ad_Led_statu(AD_VALUE);
  }
}

void USART2_task(void *pdata)
{
	INT8U err;
	char *des_mult=NULL;//设置寻址地址
	while(1)
	{
		OSSemPend(USART2_Semp,0,&err); //请求信号量
		if(BLUETOOTH_MESSG.receve_ok)
		{
			if(Task_Flag.Flag_Bit.Debug_Date)
			{
				UART_SendString(BLUETOOTH_USART,&BLUETOOTH_MESSG.messg_buff);//将其他数据发回蓝牙
			}
			if((strstr(BLUETOOTH_MESSG.messg_buff,"$efrtk"))!=NULL)//收到获取的账户
			{
				if(!Get_qx_count(&BLUETOOTH_MESSG.messg_buff[0]))
				{
					if(!Task_Flag.Flag_Bit.Get_Cunt_Ok)
					{
						Task_Flag.Flag_Bit.Get_Cunt_Ok=TRUE;
						UART_SendString(BLUETOOTH_USART,"$GETACCOUNT OK\r\n");//发给蓝牙
						Play_voice(GET_NET_OK);
						qxwz_setting(&config);
						//qxwz_prefs_flags_set(QXWZ_PREFS_FLAG_ENABLE_LOG);		//log输出使能
						//qxwz_prefs_flags_set(QXWZ_PREFS_FLAG_SOCKET_ASYN);	//log与sokect同步输出使能
  						//qxwz_prefs_flags_set(QXWZ_PREFS_FLAG_ENABLE_LOG|QXWZ_PREFS_FLAG_SOCKET_ASYN);
						qxwz_start(qxwz_rtcm_response_callback,qxwz_status_response_callback);//启动rtcm 
						OSTaskResume(RTCM_TASK_PRIO);//恢复任务
						OSTaskResume(Tick_TASK_PRIO);//恢复任务
						Task_Flag.Flag_Bit.Send_Gpgga=TRUE;//可以发送GPGGA数据
					}
				}
			}
			else if((strstr(BLUETOOTH_MESSG.messg_buff,"$NQX"))!=NULL)	//设置成普通基站指令
			{
				UART_SendString(GGA_USART,"$NQX\r\n");	//发给底板去处理
			}
			else if((strstr(BLUETOOTH_MESSG.messg_buff,"$NET_INSTRUMENT"))!=NULL)	//设置成云测绘仪
			{
				UART_SendString(GGA_USART,"$NET_INSTRUMENT\r\n");	//发给底板去处理
			}
			else if((strstr(BLUETOOTH_MESSG.messg_buff,"$CONFIG"))!=NULL)	//移动站和测绘仪的配置指令
			{
				UART_SendString(GGA_USART,"$CONFIG\r\n");	//发给底板去处理
			}
			else if((strstr(BLUETOOTH_MESSG.messg_buff,"$CBASE"))!=NULL)	//基站的配置指令
			{
				UART_SendString(GGA_USART,"$CBASE\r\n");	//发给底板去处理
			}
			else if((strstr(BLUETOOTH_MESSG.messg_buff,"$DEBUG"))!=NULL)	//基站的配置指令
			{
				Task_Flag.Flag_Bit.Debug_Date=TRUE;//开启蓝牙调试模式
			}
			else if((des_mult=strstr(BLUETOOTH_MESSG.messg_buff,"$NAME"))!=NULL)//修改蓝牙名字
			{	
				SendCmdViaBt(des_mult+5,5);//set bluetooth name
			}
			else if((des_mult=strstr(BLUETOOTH_MESSG.messg_buff,"$NET"))!=NULL)//网络地址
			{
				Fram_Date_Buff[0]=BLUETOOTH_MESSG.date_lenth+2;
				memcpy(&Fram_Date_Buff[1],&BLUETOOTH_MESSG.messg_buff[0],BLUETOOTH_MESSG.date_lenth);
				Fram_Date_Buff[Fram_Date_Buff[0]]=0x20;
				
				//Fram_read(Move_Station_Net_Len_Addr,0x0001,&Fram_Date_Buff[0]);
				//Fram_read(Move_Station_Net_Addr,Fram_Date_Buff[0]-1,&Fram_Date_Buff[1]);//网络地址写入
				UART_SendString(BLUETOOTH_USART,&Fram_Date_Buff[1]);//发给蓝牙
				UART_SendString(GGA_USART,des_mult);	//发给底板去处理
			}
			else if((des_mult=strstr(BLUETOOTH_MESSG.messg_buff,"$UNION"))!=NULL)//单元地址
			{
				Fram_Date_Buff[0]=BLUETOOTH_MESSG.date_lenth+2;
				memcpy(&Fram_Date_Buff[1],&BLUETOOTH_MESSG.messg_buff[0],BLUETOOTH_MESSG.date_lenth);
				Fram_Date_Buff[Fram_Date_Buff[0]]=0x20;
				UART_SendString(GGA_USART,des_mult);	//发给底板去处理
			}
			else if(strstr(BLUETOOTH_MESSG.messg_buff,"$RECOVER")!=NULL)//复位处理
			{
				UART_SendString(GGA_USART,"$RECOVER\r\n");	//发给底板去处理
			}
			else if((strstr(BLUETOOTH_MESSG.messg_buff,"Name OK"))!=NULL)//蓝牙名字修改成功
			{	
				Play_voice(SET_NAME);
			}
			//UART_SendString(BLUETOOTH_USART,&BLUETOOTH_MESSG.messg_buff[0]);//发给蓝牙 调试用
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
		OSSemPend(USART3_Semp,0,&err); //请求信号量
		if(GGA_MESS.receve_ok)
		{
			if(Task_Flag.Flag_Bit.Debug_Date)
			{
				UART_SendString(BLUETOOTH_USART,&GGA_MESS.messg_buff[0]);//将其他数据发回蓝牙
			}
			if((gga_addr=strstr(GGA_MESS.messg_buff,"$GPGGA"))!=NULL)
			{
				UART_SendString(BLUETOOTH_USART,&GGA_MESS.messg_buff[0]);//将其他数据发回蓝牙
				RTK_Status(&GGA_MESS.messg_buff[0],GGA_MESS.date_lenth);
				if((strstr(GGA_MESS.messg_buff,"*66")==NULL)&&
					((strstr(GGA_MESS.messg_buff,"E"))!=NULL))	//数据存在不是空数据
				{
					if(Task_Flag.Flag_Bit.Send_Gpgga)
					{
						qxwz_send_gga(gga_addr);
					}
				}
			}
			else if((strstr(GGA_MESS.messg_buff,"$CLOSENET"))!=NULL)
			{
				UART_SendString(BLUETOOTH_USART,"$CLOSENET OK\r\n");//将其他数据发回蓝牙
				Task_Flag.Flag_Bit.Get_Cunt_Ok=TRUE;//普通固定基站不能写账户
				Task_Flag.Flag_Bit.Send_Gpgga=FALSE;
				qxwz_stop();
				OSTaskDel(Tick_TASK_PRIO);//删除任务
				OSTaskDel(RTCM_TASK_PRIO);//删除任务
				Init_Sim=TRUE;
				SIM_POWER_OFF;
			}
			else if(((strstr(GGA_MESS.messg_buff,"$QXRTK"))!=NULL)
					||((strstr(GGA_MESS.messg_buff,"$EFRTK"))!=NULL))
			{
				UART_SendString(BLUETOOTH_USART,&GGA_MESS.messg_buff[0]);//将其数据发回蓝牙
				if(!Task_Flag.Flag_Bit.Send_Rtcm_Flag)
				{
					Play_voice(SEND_Data);
					Task_Flag.Flag_Bit.Send_Rtcm_Flag=TRUE;//开始发送数据
				}
			}
			else if((strstr(GGA_MESS.messg_buff,"$NQX OK"))!=NULL)//普通固定基站设置成功
			{
				Play_voice(NNET_BASE);
				UART_SendString(BLUETOOTH_USART,"$NQX OK\r\n");//将其他数据发回蓝牙
			}
			else if((strstr(GGA_MESS.messg_buff,"$NET_INSTRUMENT OK"))!=NULL)//云测绘仪设置成功
			{
				Play_voice(NET_INSTRUMENT);
				UART_SendString(BLUETOOTH_USART,"$NET_INSTRUMENT OK\r\n");//将其他数据发回蓝牙
			}
			else if((strstr(GGA_MESS.messg_buff,"$CONFIG OK"))!=NULL)//移动站进入配置模式
			{
				Play_voice(GET_CMD);
				Task_Flag.Flag_Bit.Move_Station=!Task_Flag.Flag_Bit.Move_Station;
				UART_SendString(BLUETOOTH_USART,"$CONFIG OK\r\n");//将其他数据发回蓝牙
			}
			else if((strstr(GGA_MESS.messg_buff,"$CBASE OK"))!=NULL)//基站进入配置模式
			{
				Play_voice(GET_CMD);
				UART_SendString(BLUETOOTH_USART,"$CBASE OK\r\n");//将其他数据发回蓝牙
			}
			else if((strstr(GGA_MESS.messg_buff,"configuration"))!=NULL)//进入配置模式
			{
				qxwz_stop();
				OSTaskDel(AD_TASK_PRIO); //挂起起始任务.
				OSTaskDel(RTCM_TASK_PRIO); //挂起起始任务.
				OSTaskDel(Tick_TASK_PRIO); //挂起起始任务.
				Play_voice(CONFIG_ING);
			}
			else if((strstr(GGA_MESS.messg_buff,"$net save OK"))!=NULL)//网络地址设置成功
			{
				if(Task_Flag.Flag_Bit.Move_Station)
				{
					Fram_wirte(Move_Station_Net_Len_Addr,Fram_Date_Buff[0],&Fram_Date_Buff[0]);//网络地址写入
				}
				else
				{
					Fram_wirte(Base_Station_Net_Len_Addr,Fram_Date_Buff[0],&Fram_Date_Buff[0]);//网络地址写入
				}
				Play_voice(CONFIG_WILESS_ADDR_OK);
				UART_SendString(BLUETOOTH_USART,"$net save OK\r\n");//将其他数据发回蓝牙
			}
			else if((strstr(GGA_MESS.messg_buff,"$union save OK"))!=NULL)//单元地址设置成功
			{
				Fram_wirte(Move_Station_Union_Len_Addr,Fram_Date_Buff[0],&Fram_Date_Buff[0]);//单元地址写入Fram 只有移动站有这个
				
				Play_voice(CONFIG_WILESS_UNIO_OK);
				UART_SendString(BLUETOOTH_USART,"$union save OK\r\n");//将其他数据发回蓝牙
			}
			else if((strstr(GGA_MESS.messg_buff,"$recover save OK"))!=NULL)//恢复出厂设置
			{
				Play_voice(WILESS_RECOVER);
				UART_SendString(BLUETOOTH_USART,"$recover save OK\r\n");//将其他数据发回蓝牙
			}
			else if((strstr(GGA_MESS.messg_buff,"$"))!=NULL)//其他指令
			{
				Play_voice(NONE_DATE);
				//UART_SendString(BLUETOOTH_USART,&GGA_MESS.messg_buff[0]);//将其他数据发回蓝牙
			}
			GGA_MESS.receve_ok=FALSE;
		}
   };
}


