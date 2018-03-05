#ifndef __MAIN_H
#define __MAIN_H


#include "ucos_ii.h"
#include "qxwz_rtcm.h"
#include "usart.h"	

#define TIMER_FEQ    1

/////////////////////////UCOSII任务设置///////////////////////////////////
//主任务
#define START_TASK_PRIO                 10 //设置任务优先级
#define START_STK_SIZE                  64  //设置任务堆栈大小
void start_task(void *pdata);//任务函数

//LED0任务
#define LED0_TASK_PRIO                   5//设置任务优先级
#define LED0_STK_SIZE                    512//设置任务堆栈大小
void led0_task(void *pdata);//任务函数

//SENDGGA任务
#define SENDGGA_TASK_PRIO                  8 //设置任务优先级
#define SENDGGA_STK_SIZE                   512//设置任务堆栈大小
void sendgga_task(void *pdata);//任务函数

//RTCM任务
#define RTCM_TASK_PRIO                  6//设置任务优先级
#define RTCM_STK_SIZE                   2048//设置任务堆栈大小
void rtcm_task(void *pdata);//任务函数

//////////////////////////////////////////////////////////////////////////


void restart_connect(void);
void async_notify(void);
void timer_tick(uint16_t tick_feq);
void get_qxwz_sdk_status(qxwz_rtcm_status_code status);

#endif
