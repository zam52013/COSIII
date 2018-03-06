#ifndef _kernel_h_
#define _kernel_h_
#include "os.h"

void KernelMain(void);
static void TaskStart(void);
static void Task1(void *p_arg);
static void Task2(void *p_arg);
static void Task3(void *p_arg);
static void sendgga_task(void *pdata);
static void rtcm_task(void *pdata);
static void UART_task(void *pdata);


#endif


