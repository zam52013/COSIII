#ifndef __MAIN_H
#define __MAIN_H


#include "ucos_ii.h"
#include "qxwz_rtcm.h"
#include "usart.h"	
#include "bluetooth.h"
#include "wt5883.h"
#include "key_pow.h"
#include "ad.h"
#include "spi.h"
#include "fm25v01.h"
#include "sim7600.h"
#include "ef_qx.h"
#include "led.h"
#include "delay.h"

#include "includes.h"
#include "qxwz_rtcm.h"
#include "socket.h"
#include "prefs.h"

#define TIMER_FEQ    1

/////////////////////////UCOSII任务设置///////////////////////////////////
//主任务
#define START_TASK_PRIO                 5 //设置任务优先级
#define START_STK_SIZE                  64  //设置任务堆栈大小
void start_task(void *pdata);//任务函数

//LED0任务
#define Tick_TASK_PRIO                   10//设置任务优先级
#define Tick_STK_SIZE                    512//设置任务堆栈大小
void Tick_task(void *pdata);//任务函数

//SENDGGA任务
#define SENDGGA_TASK_PRIO                  15 //设置任务优先级
#define SENDGGA_STK_SIZE                   512//设置任务堆栈大小
void sendgga_task(void *pdata);//任务函数

//RTCM任务
#define RTCM_TASK_PRIO                  6//设置任务优先级
#define RTCM_STK_SIZE                   512//设置任务堆栈大小
void rtcm_task(void *pdata);//任务函数

//串口1数据处理任务
#define USART1_TASK_PRIO                  7//设置任务优先级
#define USART1_STK_SIZE                   512//设置任务堆栈大小
void USART1_task(void *pdata);//任务函数

//串口2数据处理任务
#define USART2_TASK_PRIO                  8//设置任务优先级
#define USART2_STK_SIZE                   512//设置任务堆栈大小
void USART2_task(void *pdata);//任务函数

//串口3数据处理任务
#define USART3_TASK_PRIO                  9//设置任务优先级
#define USART3_STK_SIZE                   512//设置任务堆栈大小
void USART3_task(void *pdata);//任务函数

//按键任务
#define KEY_TASK_PRIO                  12//设置任务优先级
#define KEY_STK_SIZE                   64//设置任务堆栈大小
void KEY_task(void *pdata);//任务函数

//按键任务
#define AD_TASK_PRIO                  13//设置任务优先级
#define AD_STK_SIZE                   64//设置任务堆栈大小
void AD_task(void *pdata);//任务函数

//////////////////////////////////////////////////////////////////////////


/*************信号量****************************************/
extern OS_EVENT *USART1_Semp;
extern OS_EVENT *USART2_Semp;
extern OS_EVENT *USART3_Semp;
/*****************************************************/

typedef union
{
	unsigned int task_flag_all;
	struct
	{
		unsigned Send_Rtcm_Flag;	/*if true   start send rtcm date*/
		unsigned Get_Cunt_Ok;		/*if true  get qianxun cunt ok*/
		unsigned Pow_On;			/*if true  power on*/
		unsigned Pow_Off;			/*if true  power off*/
		unsigned Debug_Date;		/*if true  debug to bluetooth*/
		unsigned Move_Station;		/*if true   that is move_Staion*/
		unsigned Send_Gpgga;		/*if true   net can send gpgga*/
	}Flag_Bit;
}task_flag;


extern task_flag Task_Flag;

void restart_connect(void);
void async_notify(void);
void timer_tick(uint16_t tick_feq);
void get_qxwz_sdk_status(qxwz_rtcm_status_code status);

#endif
