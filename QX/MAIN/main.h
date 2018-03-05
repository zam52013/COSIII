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

/////////////////////////UCOSII��������///////////////////////////////////
//������
#define START_TASK_PRIO                 5 //�����������ȼ�
#define START_STK_SIZE                  64  //���������ջ��С
void start_task(void *pdata);//������

//LED0����
#define Tick_TASK_PRIO                   10//�����������ȼ�
#define Tick_STK_SIZE                    512//���������ջ��С
void Tick_task(void *pdata);//������

//SENDGGA����
#define SENDGGA_TASK_PRIO                  15 //�����������ȼ�
#define SENDGGA_STK_SIZE                   512//���������ջ��С
void sendgga_task(void *pdata);//������

//RTCM����
#define RTCM_TASK_PRIO                  6//�����������ȼ�
#define RTCM_STK_SIZE                   512//���������ջ��С
void rtcm_task(void *pdata);//������

//����1���ݴ�������
#define USART1_TASK_PRIO                  7//�����������ȼ�
#define USART1_STK_SIZE                   512//���������ջ��С
void USART1_task(void *pdata);//������

//����2���ݴ�������
#define USART2_TASK_PRIO                  8//�����������ȼ�
#define USART2_STK_SIZE                   512//���������ջ��С
void USART2_task(void *pdata);//������

//����3���ݴ�������
#define USART3_TASK_PRIO                  9//�����������ȼ�
#define USART3_STK_SIZE                   512//���������ջ��С
void USART3_task(void *pdata);//������

//��������
#define KEY_TASK_PRIO                  12//�����������ȼ�
#define KEY_STK_SIZE                   64//���������ջ��С
void KEY_task(void *pdata);//������

//��������
#define AD_TASK_PRIO                  13//�����������ȼ�
#define AD_STK_SIZE                   64//���������ջ��С
void AD_task(void *pdata);//������

//////////////////////////////////////////////////////////////////////////


/*************�ź���****************************************/
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
