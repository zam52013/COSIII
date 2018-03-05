/*-------------------------------------------------------------------------

							软件主体

    
-------------------------------------------------------------------------*/

#include "App.h"
#include "include.h"


static OS_TCB taskStartTCB;
static CPU_STK taskStartStk[STARTUP_TASK_STK_SIZE]; 		//启动任务的程序空间

static OS_TCB InitTCB;
static CPU_STK Init_stk[Init_TASK_STK_SIZE];

static OS_TCB task2TCB;
static CPU_STK task2_stk[TASK2_STK_SIZE];

static OS_TCB task3TCB;
static CPU_STK task3_stk[TASK3_STK_SIZE];

static OS_TCB task_sendgga_TCB;
static CPU_STK task_sendgga_stk[TASK_SENDGGA_STK_SIZE];

static OS_TCB task_rtcm_TCB;
static CPU_STK task_rtcm_stk[TASK_RTCM_STK_SIZE];

static OS_TCB task_uart_TCB;
static CPU_STK task_uart_stk[TASK_UART_STK_SIZE];

static volatile OS_SEM taskSem;



/*******************************************************************************
* Function Name :void TaskStart(void)
* Description   :任务启动
* Input         :
* Output        :
* Other         :
* Date          :2012.04.18  11:48:23
*******************************************************************************/
static void TaskStart(void)
{
    OS_ERR 		err;   
    OSTaskCreate(   (OS_TCB     *)&task2TCB,
                    (CPU_CHAR   *)"Task2",
                    (OS_TASK_PTR)Task2,
                    (void       *)0,
                    (OS_PRIO    ) TASK2_PRIO,
                    (CPU_STK    *)&task2_stk[0],
                    (CPU_STK_SIZE)TASK2_STK_SIZE / 10,
                    (CPU_STK_SIZE)TASK2_STK_SIZE,
                    (OS_MSG_QTY )0,
                    (OS_TICK    )0,
                    (void       *)0,
                    (OS_OPT     )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), 
                    (OS_ERR     *)&err);	
    
    
    OSTaskCreate(   (OS_TCB     *)&task3TCB,
                    (CPU_CHAR   *)"Task3",
                    (OS_TASK_PTR)Task3,
                    (void       *)0,
                    (OS_PRIO    )TASK3_PRIO,
                    (CPU_STK    *)&task3_stk[0],
                    (CPU_STK_SIZE)TASK3_STK_SIZE / 10,
                    (CPU_STK_SIZE)TASK3_STK_SIZE,
                    (OS_MSG_QTY )0,
                    (OS_TICK    )0,
                    (void       *)0,
                    (OS_OPT     )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), 
                    (OS_ERR     *)&err);
	OSTaskCreate(   (OS_TCB     *)&task_sendgga_TCB,
                    (CPU_CHAR   *)"sendgga_task",
                    (OS_TASK_PTR)sendgga_task,
                    (void       *)0,
                    (OS_PRIO    )TASK_SENDGGA_PRIO,
                    (CPU_STK    *)&task_sendgga_stk[0],
                    (CPU_STK_SIZE)TASK_SENDGGA_STK_SIZE / 10,
                    (CPU_STK_SIZE)TASK_SENDGGA_STK_SIZE,
                    (OS_MSG_QTY )0,
                    (OS_TICK    )0,
                    (void       *)0,
                    (OS_OPT     )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), 
                    (OS_ERR     *)&err);
    
    OSTaskCreate(   (OS_TCB     *)&task_rtcm_TCB,
                    (CPU_CHAR   *)"rtcm_task",
                    (OS_TASK_PTR)rtcm_task,
                    (void       *)0,
                    (OS_PRIO    )TASK_RTCM_PRIO,
                    (CPU_STK    *)&task_rtcm_stk[0],
                    (CPU_STK_SIZE)TASK_RTCM_STK_SIZE / 10,
                    (CPU_STK_SIZE)TASK_RTCM_STK_SIZE,
                    (OS_MSG_QTY )0,
                    (OS_TICK    )0,
                    (void       *)0,
                    (OS_OPT     )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), 
                    (OS_ERR     *)&err);
	OSTaskCreate(   (OS_TCB     *)&task_uart_TCB,
                    (CPU_CHAR   *)"UART_task",
                    (OS_TASK_PTR)UART_task,
                    (void       *)0,
                    (OS_PRIO    )TASK_UART_PRIO,
                    (CPU_STK    *)&task_uart_stk[0],
                    (CPU_STK_SIZE)TASK_UART_STK_SIZE / 10,
                    (CPU_STK_SIZE)TASK_UART_STK_SIZE,
                    (OS_MSG_QTY )0,
                    (OS_TICK    )0,
                    (void       *)0,
                    (OS_OPT     )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), 
                    (OS_ERR     *)&err);
	
    OSSemCreate(    (OS_SEM     *)&taskSem, 	//	创建信号量
                    (CPU_CHAR   *)"taskSem", 
                    (OS_SEM_CTR )0, 
                    (OS_ERR     *)err);
    
    OSTaskDel(      (OS_TCB     *)&taskStartTCB, 
                    (OS_ERR     *)&err);
}

static void Init_Task(void *p_arg)
{
  OS_ERR err;
	Usart_init();
	Spi_init();
	IIC_Init();
	
	Led_Init();
//	Wifi_Init();
	DHT11_Init();
	SD_Init();
	SD_DeInit();
	 //Sim_contro();

	//Set_System();
	//USB_Interrupts_Config();
  	//Set_USBClock();
  	//USB_Init();  

  
	LCD_Init();
  	LCD_Clear(BLACK);    //清屏
	gui_circle(128/2,128/2,WHITE,128/2-2, 0); //画圆
	gui_circle(128/2,128/2,WHITE,128/2-3, 0);//画圆

	Gui_StrCenter(0,10,BLUE,YELLOW,"Mcudev",16,1);//居中显示
	Gui_StrCenter(0,28,RED,BLUE,"Temp:32℃",16,1);//居中显示
	Gui_StrCenter(0,46,YELLOW,BLUE,"Hum:20%",16,1);//居中显示
	Gui_StrCenter(0,64,WHITE,BLUE,"VOL:3.2V",16,1);//居中显示
	Gui_StrCenter(0,82,GREEN,BLUE,"I:1.25mA",16,1);//居中显示
	Gui_StrCenter(0,100,RED,BLUE,"12:20AM",16,1);//居中显示
	
	//OSTaskSuspend((OS_TCB * )InitTCB,&err); //挂起任务
	//OSTaskResume((OS_TCB * )InitTCB,&err);//恢复任务
	OSTaskDel((OS_TCB * )&InitTCB,&err);//初始化完成后删除任务
}

static void Task2(void *p_arg)
{
    OS_ERR err;
    while (1)
    {
        led_on(LED_2);
				 OSTimeDly(  (OS_TICK    )300, 
                    (OS_OPT     )OS_OPT_TIME_DLY, 
                    (OS_ERR     *)&err);
			
//        OSSemPend(  (OS_SEM     *)&taskSem, //挂起任务时间
//                    (OS_TICK    )10000, 
//                    (OS_OPT     )OS_OPT_PEND_BLOCKING, 
//                    (CPU_TS     *)0, 
//                    (OS_ERR     *)&err);
        
        led_off(LED_2);
		OSTimeDly(  (OS_TICK    )500, 
                    (OS_OPT     )OS_OPT_TIME_DLY, 
                    (OS_ERR     *)&err);
    }
}

static void Task3(void *p_arg)
{
    OS_ERR err;
    while (1)
    {
    	OSSemPend((OS_SEM     *)&taskSem,0,OS_OPT_PEND_BLOCKING,0,&err);
			led_on_off(LED_3);
//        OSTimeDly(  (OS_TICK    )100, 
//                    (OS_OPT     )OS_OPT_TIME_DLY, 
//                    (OS_ERR     *)&err);
//        OSTimeDly(  (OS_TICK    )600, 
//                    (OS_OPT     )OS_OPT_TIME_DLY, 
//                    (OS_ERR     *)&err);
    }

}

//发送GGA任务#define 
void sendgga_task(void *pdata)
{	  
	OS_ERR err;
	while(1)
	{
                                                                                                                                                                                                                                                                                                                            		led_on_off(LED_1);
		//Wifi_Send("OK");
		OSTimeDly(  (OS_TICK    )500, 
                    (OS_OPT     )OS_OPT_TIME_DLY, 
                    (OS_ERR     *)&err);
//		
//        OSTimeDly(  (OS_TICK    )400, 
//                    (OS_OPT     )OS_OPT_TIME_DLY, 
//                    (OS_ERR     *)&err);
//        led_off(LED_1);
//        OSTimeDly(  (OS_TICK    )500, 
//                    (OS_OPT     )OS_OPT_TIME_DLY, 
//                    (OS_ERR     *)&err);
        OSSemPost(  (OS_SEM     *)&taskSem, //发送信号量
                    (OS_OPT     )OS_OPT_POST_1, 
                    (OS_ERR     *)&err);
		
	};
}
void rtcm_task(void *pdata) 
{
	OS_ERR err;
    while(1) 
	{
        //每隔一秒模拟宿主触发一次tick
		led_on_off(LED_4);
//		DHT11_Read_Data();
//		#ifdef DEBUG_MEG
//			__DBG__("温度:%d\r\n",temperature);
//		#endif 
//		#ifdef DEBUG_MEG
//			__DBG__("湿度:%d\r\n",humidity);
//		#endif 
//		
        OSTimeDly(  (OS_TICK    )3000, 
                    (OS_OPT     )OS_OPT_TIME_DLY, 
                    (OS_ERR     *)&err);
   
    }
}
void UART_task(void *pdata) 
{
		OS_ERR err;
    while(1)   
		{
			led_on_off(LED_5);
			Send_Led_Rgb(REGLED_COLOR_WHITE,1);
			OSTimeDly(  (OS_TICK    )500, 
                    (OS_OPT     )OS_OPT_TIME_DLY, 
                    (OS_ERR     *)&err);
     }
}

void KernelMain(void)
{
	
    OS_ERR err;
		CPU_SR_ALLOC();
  	CPU_Init();
		OSInit((OS_ERR	    *)&err);
		OS_CRITICAL_ENTER();
		/************创建最高优先级的初始化程序****************/
		OSTaskCreate(   (OS_TCB     *)&InitTCB,
                    (CPU_CHAR   *)"Init_Task",
                    (OS_TASK_PTR)Init_Task,
                    (void       *)0,
                    (OS_PRIO    )InitTask_PRIO,
                    (CPU_STK    *)&Init_stk[0],
                    (CPU_STK_SIZE)Init_TASK_STK_SIZE / 10,
                    (CPU_STK_SIZE)Init_TASK_STK_SIZE,
                    (OS_MSG_QTY )0,
                    (OS_TICK    )0,
                    (void       *)0,
                    (OS_OPT     )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), 
                    (OS_ERR     *)&err);
	/************创建其他程序入口****************/
    OSTaskCreate(   (OS_TCB     *)&taskStartTCB,
                    (CPU_CHAR   *)"Task Start",
                    (OS_TASK_PTR)TaskStart,
                    (void       *)0,
                    (OS_PRIO    ) STARTUP_TASK_PRIO,
                    (CPU_STK    *)&taskStartStk[0],
                    (CPU_STK_SIZE)STARTUP_TASK_STK_SIZE / 10,
                    (CPU_STK_SIZE)STARTUP_TASK_STK_SIZE,
                    (OS_MSG_QTY )0,
                    (OS_TICK    )0,
                    (void       *)0,
                    (OS_OPT     )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), 
                    (OS_ERR     *)&err);    
	OS_CRITICAL_EXIT();
    OSStart(        (OS_ERR	    *)&err);
}





