#ifndef __MAIN_H
#define __MAIN_H


#include "ucos_ii.h"
#include "qxwz_rtcm.h"
#include "usart.h"	

#define TIMER_FEQ    1

/////////////////////////UCOSII��������///////////////////////////////////
//������
#define START_TASK_PRIO                 10 //�����������ȼ�
#define START_STK_SIZE                  64  //���������ջ��С
void start_task(void *pdata);//������

//LED0����
#define LED0_TASK_PRIO                   5//�����������ȼ�
#define LED0_STK_SIZE                    512//���������ջ��С
void led0_task(void *pdata);//������

//SENDGGA����
#define SENDGGA_TASK_PRIO                  8 //�����������ȼ�
#define SENDGGA_STK_SIZE                   512//���������ջ��С
void sendgga_task(void *pdata);//������

//RTCM����
#define RTCM_TASK_PRIO                  6//�����������ȼ�
#define RTCM_STK_SIZE                   2048//���������ջ��С
void rtcm_task(void *pdata);//������

//////////////////////////////////////////////////////////////////////////


void restart_connect(void);
void async_notify(void);
void timer_tick(uint16_t tick_feq);
void get_qxwz_sdk_status(qxwz_rtcm_status_code status);

#endif
