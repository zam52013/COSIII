/*
 **************************************************************************
 * @file    socket.c
 * @author  Liwa Lin
 * @version V1.0.1
 * @date    Sep 8, 2016
 * @version V1.0.0
 * @date    Sep 1, 2016
 **************************************************************************
 * @attetion  Based on GSM Module SIM7600C
 *
 * Copyright(c) 2016 QXWZ Corporation.  All rights reserved.
 */

#include "socket.h"
#include "delay.h"
#include "usart.h"
#include "main.h"
#include "prefs.h"
#include "ef_qx.h"
#include "sim7600.h"
#include "wt5883.h"

#undef QXLOG
#define QXLOG printf


typedef  uint8_t  BOOL;
#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

//GSM»Ø¸´ERROR
#define GSM_ACK_ERROR             1
//GSMÖØÆô
#define GSM_ACK_IIII              2
//GSM TCPÁ¬½ÓÊ§°Ü
#define GSM_ACK_CONNECT_FAIL      3
//GSM Ô¶¶Ë¹Ø±ÕÁ¬½Ó
#define GSM_ACK_CLOSED            4
//µÈ´ıGSMÄ£¿é»Ø¸´³¬Ê±
#define GSM_ACK_TIMEOUT           5
//ÒÑ½¨Á¢Á¬½Ó
#define GSM_ACK_ALREAY_CONNECT    6
//Ä£¿é²»ÔÊĞí²Ù×÷
#define GSM_ACK_CME_ERROR         7

//socket Ä¬ÈÏÍ¨µÀºÅ
#define SOC_DEFUALT_CHANNEL       1   
//´®¿ÚÊı¾İ½ÓÊÕ»º´æÇø
#define BUF_MAX                   40 
//ATÖ¸ÁîµÈ´ı»Ø¸´³¬Ê±Ê±¼ä£¬µ¥Î»Ãë
#define AT_CMD_TIMEOUT    		   	5

#define RECV_DATA_BUF_MAX         2048

#define WAIT_CFG_TRY_SECOND       20
//Í³¼ÆÊ¹ÓÃ
uint8_t recv_IIII_cnt = 0;
uint8_t recv_CLOSED_cnt = 0;
uint32_t timer_cnt = 0;
int32_t uart_irq_time_need = 0;


/** ----------------------------------------- INTERNAL ------------------------------------------*/

//ÊÇ·ñÒÑ¾­³õÊ¼»¯GSMÄ£¿é
static BOOL s_is_init = FALSE;
//ÖØÆôÄ£¿é±êÖ¾
static BOOL s_restart_gsm_flag = FALSE;

//¿ªÊ¼½ÓÊÕGPRS TCPÓ¦ÓÃ²ãÊı¾İ
static BOOL s_begin_recive = FALSE;

static uint8_t s_first_index = 0;
static uint16_t s_body_len = 0;
//ÒÑ¾­½ÓÊÕµ½µÄÍøÂçÓ¦ÓÃ²ãµÄÊı¾İ³¤¶È
static uint16_t s_has_recived_len = 0;

//static char body_len_buf[5] = {0};//»º´æbody³¤¶È
static char uart_exception_buf[10] = {0};
static char uart_buf[BUF_MAX] = {0};
static char recived_buf[RECV_DATA_BUF_MAX] = {0};//½ÓÊÕÊı¾İµÄ»º´æ


//¶¨Ê±Æ÷Ïà¹Ø
static BOOL s_timer_start_flag = FALSE;	
static BOOL s_at_ack_timeout_flag = FALSE; 
static uint8_t s_at_ack_wait_time = 0;
static uint16_t s_tick_cnt = 0;

//qxwz_sdk ×´Ì¬
static uint32_t qxwz_sdk_ntrip_status = 0;

static int8_t qxwz_soc_init(void);
static int8_t send_at_command(char *b,char *a,uint8_t wait_time); 
static uint8_t find_string(char *str);
static void clear_buf(void);

static int8_t set_ate0(void);
static int8_t init_gsm_gprs(void);
static void get_gsm_status(void);
static uint8_t gsm_ack_exception(void);
static void start_at_ack_timer(uint8_t second);


//socket·¢Éú´íÎóµÄÇé¿öÏÂµ÷ÓÃ

static void query_uart_buf(uint8_t recv);
static void uart_recv_exception(uint8_t recv);

//Òì²½socket
static unsigned char  s_connect_ok_flag = FALSE;
static unsigned char  s_connect_fail_flag = FALSE;

static unsigned char s_send_ok_flag = FALSE;
static unsigned char  s_send_fail_flag = FALSE;
static unsigned char s_close_ok_flag = FALSE;
static unsigned char s_close_fail_flag = FALSE;


/**-------------------------Ä£Äâsocket create connect send  close---------------------------------*/
/*******************************************************************************
º¯ Êı Ãû£ºvoid qxwz_soc_create
¹¦ÄÜÃèÊö£ºÍøÂç·şÎñÆ÷³õÊ¼»¯
Èë¿Ú²ÎÊı£ºsoc:Í¨µÀ address:·şÎñÆ÷µØÖ·
·µ»Ø²ÎÊı£º0 ³É¹¦
´´½¨Ê±¼ä: 2017-11-02 by zam
********************************************************************************/
qxwz_soc qxwz_soc_create(void)
{
	qxwz_soc  socket_fd = -1;
	
	socket_fd = qxwz_soc_init();
	if(socket_fd >= 0)
	{
		QXLOG("socket connect successful=%d \r\n",socket_fd);
		return socket_fd;
	}
	else
	{
		QXLOG(" socket connect erro\r\n");
		return -1;
	}
}

/*******************************************************************************
º¯ Êı Ãû£ºvoid qxwz_soc_connect
¹¦ÄÜÃèÊö£ºÁ¬½ÓÍøÂç·şÎñÆ÷
Èë¿Ú²ÎÊı£ºsoc:Í¨µÀ address:·şÎñÆ÷µØÖ·
·µ»Ø²ÎÊı£º0 ³É¹¦
´´½¨Ê±¼ä: 2017-11-02 by zam
********************************************************************************/
int8_t qxwz_soc_connect(qxwz_soc soc,qxwz_soc_address address)
{
	int8_t ret = 0;
	char connect_buf[200];
	memset(connect_buf,0,sizeof(connect_buf));
	sprintf(connect_buf,"AT+CIPOPEN=0,\"TCP\",\"%s\",%d",address.hostName,address.port);
	QXLOG("connect servece= %s\r\n",connect_buf);
	if((ret = send_at_command(connect_buf,"OPEN",3*AT_CMD_TIMEOUT)) != 0)
	{
		QXLOG("connect erro=%d \r\n",ret);
		if(qxwz_prefs_flags_get() & QXWZ_PREFS_FLAG_SOCKET_ASYN)
		{
			s_connect_fail_flag = TRUE;
		}
		return ret;
	}
	return 0;
}
/*******************************************************************************
º¯ Êı Ãû£ºvoid qxwz_soc_send
¹¦ÄÜÃèÊö£º¹Ø±ÕÍøÂçÄ£¿é·¢ËÍÊı¾İ
Èë¿Ú²ÎÊı£ºsoc:Í¨µÀ send_buffer:´ı·¢ËÍÊı¾İ length:Êı¾İ³¤¶È
·µ»Ø²ÎÊı£ºÊı¾İ·¢ËÍ³¤¶È
´´½¨Ê±¼ä: 2017-11-02 by zam
********************************************************************************/
qxwz_ssize_t qxwz_soc_send(qxwz_soc soc,char *send_buffer,size_t length)
{
	int8_t ret = 0;
	char sendbuf[1024];
	memset(sendbuf,0,sizeof(sendbuf));
	ret = send_at_command("AT+CIPSEND=0,",">",3*AT_CMD_TIMEOUT);
	if(ret != 0)
	{
		QXLOG("can not connect socker!\r\n");//·¢ËÍÊı¾İ
		qxwz_soc_error(SOC_DEFUALT_CHANNEL);
		return -1;	
	}
	QXLOG("send date......\r\n");//·¢ËÍÊı¾İ
	sprintf(sendbuf,"%s\32\0",send_buffer);
	QXLOG("%s\32\0",send_buffer);
	ret = send_at_command(sendbuf,"OK",3*AT_CMD_TIMEOUT);  //»Ø¸´OK£¬Ê±¼ä¿ÉÒÔµ÷
	if(ret != 0)
	{
		QXLOG("send erro.....\r\n");//·¢ËÍÊı¾İ
		if(qxwz_prefs_flags_get() & QXWZ_PREFS_FLAG_SOCKET_ASYN)
		{
			s_send_fail_flag = TRUE;
		}
		qxwz_soc_error(SOC_DEFUALT_CHANNEL);
		return -1;
	}
	return length;
}

/*******************************************************************************
º¯ Êı Ãû£ºvoid qxwz_soc_close
¹¦ÄÜÃèÊö£º¹Ø±ÕÍøÂçÄ£¿éÊı¾İ
Èë¿Ú²ÎÊı£º
·µ»Ø²ÎÊı£º
´´½¨Ê±¼ä: 2017-11-02 by zam
********************************************************************************/
void qxwz_soc_close(qxwz_soc soc)
{
	uint8_t local_soc = soc;
	int8_t ret = 0;	
	QXLOG("select close socket=%d.....\r\n",local_soc);//Ö÷¶¯¹Ø±ÕÁ¬½Ó
	ret = send_at_command("AT+CIPCLOSE=0","CLOSE",3*AT_CMD_TIMEOUT);	//¹Ø±ÕÁ¬½Ó
	if(ret != 0)
	{
		QXLOG("close socket erro.....\r\n");//·¢ËÍÊı¾İ
		if(qxwz_prefs_flags_get() & QXWZ_PREFS_FLAG_SOCKET_ASYN)
		{
			s_close_fail_flag = TRUE;
		}
	}
}

/*******************************************************************************
º¯ Êı Ãû£ºvoid qxwz_soc_init
¹¦ÄÜÃèÊö£ºÍøÂçÄ£¿é³õÊ¼»¯
Èë¿Ú²ÎÊı£º
·µ»Ø²ÎÊı£º
´´½¨Ê±¼ä: 2017-11-02 by zam
********************************************************************************/
qxwz_soc qxwz_soc_init(void)
{
	int8_t ret = 0;
	if(Init_Sim)
	{
		return 1;
	}
	if(s_is_init == FALSE)
	{
		ret = init_gsm_gprs();
		if(ret == 0)
		{
			s_is_init = TRUE;
		}else
		{
			QXLOG("init model erro code=%d\r\n",ret);
			return -1;
		}
	}
	//GSMµ¥Í¨µÀÄ£Ê½Ä¬ÈÏÊ¹ÓÃÍ¨µÀ1
	return 1;	
	
}
/**------------------------------------------ÖĞ¶Ï·şÎñ³ÌĞòÏà¹Ø--------------------------------------------*/
/*******************************************************************************
º¯ Êı Ãû£ºvoid USART1_IRQHandler
¹¦ÄÜÃèÊö£º®¿Ú2ÖĞ¶Ï·şÎñ³ÌĞò,½ÓÊÕGSMÄ£¿éÓ¦´ğµÄÊı¾İ
Èë¿Ú²ÎÊı£º
·µ»Ø²ÎÊı£º
´´½¨Ê±¼ä: 2017-11-02 by zam
********************************************************************************/
void USART1_IRQHandler(void)                	
{
	uint8_t res = 0;
	OSIntEnter();
	if(USART_GetITStatus(GSM_USART, USART_IT_RXNE) != RESET) 
	{
		res = USART_ReceiveData(GSM_USART);
		uart_recv_exception(res);
		if(s_begin_recive == TRUE)
		{
			recived_buf[s_has_recived_len] = res;
			s_has_recived_len++;
			if(s_has_recived_len >= s_body_len)
			{
				s_begin_recive = FALSE;
				recived_buf[s_has_recived_len+1] = 0;
				qxwz_on_soc_data_received(SOC_DEFUALT_CHANNEL,recived_buf,s_has_recived_len);
			}
		}
		else
		{
			uart_buf[s_first_index] = res;  	  //½«½ÓÊÕµ½µÄ×Ö·û´®´æµ½»º´æÖĞ 
			if(!Task_Flag.Flag_Bit.Debug_Date)
			{
				DEBUG_UART->DR = res;//½«½ÓÊÕµ½µÄÊı¾İ×ª·¢µ½µ÷ÊÔ´®¿Ú
			}
			else
			{
				BLUETOOTH_USART->DR = res;//½«½ÓÊÕµ½µÄÊı¾İ×ª·¢µ½µ÷ÊÔ´®¿Ú
			}
			s_first_index++;                		//»º´æÖ¸ÕëÏòºóÒÆ¶¯
			if(s_first_index >= (BUF_MAX-1))
			{
				s_first_index = 0;
			}
			query_uart_buf(res);
		}
	}
	OSIntExit();  
}

/*******************************************************************************
º¯ Êı Ãû£ºvoid uart_recv_exception
¹¦ÄÜÃèÊö£º²éÑ¯´®¿ÚÃüÁî½ÓÊÕ»º´æÇø
Èë¿Ú²ÎÊı£º@param  recv    ´®¿Ú½ÓÊÕµ½µÄ×Ö·û
·µ»Ø²ÎÊı£º
´´½¨Ê±¼ä: 2017-11-02 by zam
********************************************************************************/
static void query_uart_buf(uint8_t recv)
{
	char *qx_cnt;
	if((recv =='\n'))//ÓÃÀ´ÅĞ¶ÏÊÇ·ñÓĞÊı¾İ·µ»Ø  gsmÄ£¿é·µ»ØµÄÊı¾İ
	{
		if((qx_cnt=strstr(uart_buf,"+IPD"))!=NULL)
		{
			s_begin_recive = 1;
			qx_cnt+=4;
			s_body_len =atoi(qx_cnt);//¼ÆËã½«ÒªÊÕµ½µÄÊı¾İ
			if(s_body_len > sizeof(recived_buf)-1)
			{
				s_body_len	= sizeof(recived_buf)-1;
			}
			s_has_recived_len = 0;
			clear_buf();
		}
		else if((qx_cnt=strstr(uart_buf,"SIM not"))!=NULL)
		{
			Play_voice(NO_SIM);
			QXLOG("$NO SIM\r\n");//½«ÆäËûÊı¾İ·¢»Øµ½µ×°åÈ¥
			clear_buf();
		}
		else if((qx_cnt=strstr(uart_buf,"ERROR"))!=NULL)
		{
			clear_buf();
		}
	}
	if(qxwz_prefs_flags_get() & QXWZ_PREFS_FLAG_SOCKET_ASYN)//Ê¹ÄÜÒì²½SOCKET£¬²ÅĞèÒª¼ì²âÍê³ÉÇé¿ö
	{
		if(recv == 'K')//ÅĞ¶ÏÒ»Ğ©´øÓĞOKµÄÕı³£×´Ì¬
		{
			if((strcmp(uart_buf,"CLOSE"))!=NULL)
			{
				s_close_ok_flag = TRUE;
			}
			else if((strcmp(uart_buf,"CONNECT"))!=NULL)
			{
				s_connect_ok_flag = TRUE;
			}
			else if((strcmp(uart_buf,"SEND"))!=NULL)
			{
				s_send_ok_flag = TRUE;
			}
		}	
	}
	
}
/*******************************************************************************
º¯ Êı Ãû£ºvoid uart_recv_exception
¹¦ÄÜÃèÊö£ºÅĞ¶Ï½ÓÊÕ´íÎóÊı¾İ
Èë¿Ú²ÎÊı£º
·µ»Ø²ÎÊı£º
´´½¨Ê±¼ä: 2017-11-02 by zam
********************************************************************************/
void uart_recv_exception(uint8_t recv)
{
	static BOOL s_begin_exception_recv_flag = FALSE;
	static uint8_t s_index = 0;
	if(!s_begin_exception_recv_flag)
	{
		if((recv == 'I')||(recv == 'C'))
		{
			s_begin_exception_recv_flag = TRUE;
			s_index = 0;
			memset(uart_exception_buf,0,sizeof(uart_exception_buf));
			uart_exception_buf[s_index++] = recv;//¼ÇÂ¼Òì³£Ê××Ö·û
		}
	}
	else
	{
		uart_exception_buf[s_index++] = recv;
		if(s_index > sizeof(uart_exception_buf))
		{
			s_index = 0;
			s_begin_exception_recv_flag = FALSE;
		}
		if(!strcmp(uart_exception_buf,"IIII"))
		{
			s_begin_exception_recv_flag = FALSE;//½áÊø½ÓÊÕ
			qxwz_soc_error(SOC_DEFUALT_CHANNEL);
			s_is_init = FALSE;//ÖØĞÂ³õÊ¼»¯
		}
		else if(!strcmp(uart_exception_buf,"CLOSE"))
		{
			s_begin_exception_recv_flag = FALSE;//½áÊø½ÓÊÕ
			qxwz_soc_error(SOC_DEFUALT_CHANNEL);
		}
	}
}

/*******************************************************************************
º¯ Êı Ãû£ºvoid TIM3_IRQHandler
¹¦ÄÜÃèÊö£º¶¨Ê±Æ÷3ÖĞ¶Ï·şÎñ³ÌĞò
Èë¿Ú²ÎÊı£º
·µ»Ø²ÎÊı£º
´´½¨Ê±¼ä: 2017-11-02 by zam
********************************************************************************/
void TIM3_IRQHandler(void)   
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) 
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  
		timer_tick(TIMER_FEQ);
	}
}

/*******************************************************************************
º¯ Êı Ãû£ºvoid timer_tick
¹¦ÄÜÃèÊö£ºÔÚ¶¨Ê±ÖĞ¶ÏÖĞ±»µ÷ÓÃ
Èë¿Ú²ÎÊı£º	@param  tick_feq              1ÃëÄÚµ÷ÓÃÆµÂÊ,µ¥Î»(HZ)
·µ»Ø²ÎÊı£º
´´½¨Ê±¼ä: 2017-11-02 by zam
********************************************************************************/
void timer_tick(uint16_t tick_feq)
{
	if(s_timer_start_flag)
	{
		if(s_tick_cnt >= s_at_ack_wait_time*tick_feq)
		{
				s_at_ack_timeout_flag = TRUE;
		}	
		s_tick_cnt++;
	}
	else
	{
		s_tick_cnt = 0;
	}
}

/**-------------------------------------ATÖ¸ÁîÏà¹Ø------------------------------------------------*/
/*******************************************************************************
º¯ Êı Ãû£ºvoid send_at_command
¹¦ÄÜÃèÊö£ºÆôÓÃ¶¨Ê±Æ÷ 
Èë¿Ú²ÎÊı£º	@param  send_str              ´ı·¢ËÍµÄATÖ¸Áî	 @param  excepted_recv_str     ÆÚ´ı½ÓÊÕµÄÓ¦´ğ
·µ»Ø²ÎÊı£º  @param  wait_time             ³¬Ê±Ê±¼ä,µ¥Î»(S)
´´½¨Ê±¼ä: 2017-11-02 by zam
********************************************************************************/
static int8_t send_at_command(char *send_str,char *excepted_recv_str,uint8_t wait_time)         
{
	uint8_t ret = 0;									
	if(send_str == NULL)
	{
		return -1;
	}
	clear_buf(); //Çå´®¿Ú½ÓÊÕ»º´æÇø
	s_timer_start_flag = FALSE;
	for (; *send_str!='\0';send_str++)//·¢ËÍATÃüÁî
	{
		while(USART_GetFlagStatus(GSM_USART, USART_FLAG_TC) == RESET);
		USART_SendData(GSM_USART,*send_str);
	}
	UART_SendString(GSM_USART,"\r\n");
	if((excepted_recv_str != NULL)&&(wait_time > 0))
	{
		start_at_ack_timer(wait_time);//Æô¶¯³¬Ê±¿ØÖÆ¶¨Ê±Æ÷
	}
	else
	{
		return 0;//Ö±½Ó·µ»Ø
	}
	while(1)                    
	{
		if(!find_string(excepted_recv_str))
		{
			ret = gsm_ack_exception();	//Ã»ÊÕµ½ÆÚ´ıÊÕµ½µÄGSM Ó¦´ğ	
			if(ret != 0)
			{
				break;
			}
    	}
		else
    	{
			//find the excepted string
			break;   //Ìø³öÑ­»·
		}
		if(s_at_ack_timeout_flag)//timeout
		{
			ret = GSM_ACK_TIMEOUT;
			break;
		}
	}//while(1)
	
	if(ret == GSM_ACK_ALREAY_CONNECT)
	{
		ret = 0;
	}
	clear_buf(); 
	s_timer_start_flag = FALSE;  //Í£Ö¹¼ÆÊ±Æ÷
	s_at_ack_timeout_flag = FALSE;
	return ret;
}
/*******************************************************************************
º¯ Êı Ãû£ºvoid start_at_ack_timer(void)
¹¦ÄÜÃèÊö£º ÅĞ¶Ï»º´æÖĞÊÇ·ñº¬ÓĞÖ¸¶¨µÄ×Ö·û´®
Èë¿Ú²ÎÊı£º	@param  str              ´ı·¢ËÍµÄATÖ¸Áî	
·µ»Ø²ÎÊı£º @return 1 ÕÒµ½Ö¸¶¨×Ö·û£¬0 Î´ÕÒµ½Ö¸¶¨×Ö·û 
´´½¨Ê±¼ä: 2017-11-02 by zam
********************************************************************************/
static uint8_t find_string(char *str)
{ 
	if(NULL == str)
	{
		return 0;
	}
  	if(strstr(uart_buf,str) != NULL)
	  return 1;
	else
		return 0;
}
/*******************************************************************************
º¯ Êı Ãû£ºvoid start_at_ack_timer(void)
¹¦ÄÜÃèÊö£ºÆôÓÃ¶¨Ê±Æ÷ 
Èë¿Ú²ÎÊı£º		second  			  ¶¨Ê±Ê±¼ä,µ¥Î»(S)		
·µ»Ø²ÎÊı£º 
´´½¨Ê±¼ä: 2017-11-02 by zam
********************************************************************************/
static void start_at_ack_timer(uint8_t second)
{
	s_at_ack_wait_time = second; 
	s_timer_start_flag = TRUE;     
}
/*******************************************************************************
º¯ Êı Ãû£ºvoid clear_buf(void)
¹¦ÄÜÃèÊö£ºÇå³ı´®¿Ú½ÓÊÕ»º´æÇø,²¢ÖÃ½ÓÊÕÆ«ÒÆÁ¿Îª0		
Èë¿Ú²ÎÊı£º							
·µ»Ø²ÎÊı£º 
´´½¨Ê±¼ä: 2017-11-02 by zam
********************************************************************************/
static void clear_buf(void)
{
	//uint16_t k;
	//for(k = 0;k < BUF_MAX;k++) 
	//{
	//	uart_buf[k] = 0x00;
	//}
	memset(uart_buf, 0, BUF_MAX);
  	s_first_index = 0;              //½ÓÊÕ×Ö·û´®µÄÆğÊ¼´æ´¢Î»ÖÃ
}

/*******************************************************************************
º¯ Êı Ãû£ºvoid wait_creg(void)
¹¦ÄÜÃèÊö£ºµÈ´ıÄ£¿é×¢²á³É¹¦				
Èë¿Ú²ÎÊı£º							
·µ»Ø²ÎÊı£º @return 0  ok   else  errro
´´½¨Ê±¼ä: 2017-11-02 by zam
********************************************************************************/
int8_t wait_creg(void)
{
	uint8_t i = 0;
	uint8_t k;
	BOOL first_command = TRUE;
	clear_buf();
	QXLOG("reging.....");
  	while(i == 0)        			
	{
		clear_buf();  
		UART_SendString(GSM_USART,"AT+CREG?");
		UART_SendString(GSM_USART,"\r\n");
		delay_ms(3000);  	
		if(first_command)
		{
			start_at_ack_timer(WAIT_CFG_TRY_SECOND);
			first_command = FALSE;
		}
	  	for(k = 0;k < BUF_MAX;k++) 
		{
			if(uart_buf[k] == ':')
			{
				if((uart_buf[k+4] == '1')||(uart_buf[k+4] == '5'))
				{
					i = 1;
					QXLOG("\r\n");
					return  0;
				}
			}
		}
		if(s_at_ack_timeout_flag)
		{
			s_restart_gsm_flag = TRUE;
			s_timer_start_flag = FALSE;//Í£Ö¹¼ÆÊ±Æ÷
			s_at_ack_timeout_flag = FALSE;
			return  GSM_ACK_TIMEOUT;
		}
	}//while(i == 0)   
	return 0;
}
	
/*******************************************************************************
º¯ Êı Ãû£ºvoid set_ate0(void)
¹¦ÄÜÃèÊö£º¹Øµô»ØÏÔ					
Èë¿Ú²ÎÊı£º							
·µ»Ø²ÎÊı£º
´´½¨Ê±¼ä: 2017-11-02 by zam
********************************************************************************/
static int8_t set_ate0(void)
{
	return (send_at_command("ATE0","OK",AT_CMD_TIMEOUT));								//È¡Ïû»ØÏÔ		
}
/*******************************************************************************
º¯ Êı Ãû£ºint8_t init_gsm_gprs(void)
¹¦ÄÜÃèÊö£º³õÊ¼»¯tcpÍøÂç					
Èë¿Ú²ÎÊı£º							
·µ»Ø²ÎÊı£º
´´½¨Ê±¼ä: 2017-11-02 by zam
********************************************************************************/
static int8_t init_gsm_gprs(void)
{
	int8_t ret = 0;
	delay_ms(3000);  
	ret = send_at_command("AT+CICCID","SIM not",3);
	ret = wait_creg();
	if(ret == 0)
	{
		QXLOG("reg successful.....\r\n");
	}
	else
	{
		QXLOG("reg erro.....\r\n");
		Play_voice(NET_ERRO);//×¢²á´íÎó
		return -1;
	}
	if((ret = set_ate0()) != 0)//¹Ø±Õ»ØÏÔ
	{
		return ret;
	}
//	send_at_command("AT+CREG=1","OK",3*AT_CMD_TIMEOUT);//¿ªÆôÏÔÊ¾ÍøÂç×¢²áµÈĞÅÏ¢
	if((ret = send_at_command("AT+CGREG=1","OK",3*AT_CMD_TIMEOUT)) != 0) //ÉèÖÃGPRSÒÆ¶¯Ì¨Àà±ğÎªA,Ö§³Ö°ü½»»»ºÍÊı¾İ½»»» 
	{
		QXLOG("net reg erro!\r\n");
		return ret;
	}		
//	if((ret = send_at_command("AT+CGCLASS=\"A\"","OK",3*AT_CMD_TIMEOUT)) != 0) //ÉèÖÃGPRSÒÆ¶¯Ì¨Àà±ğÎªA,Ö§³Ö°ü½»»»ºÍÊı¾İ½»»» 
//	{
//		QXLOG("AÀàÒÆ¶¯Ì¨ÉèÖÃ´íÎó!\r\n");
//		return ret;
//	}		
//	if((ret = send_at_command("AT+CGATT=1","OK",3*AT_CMD_TIMEOUT)) != 0)//¸½×ÅGPRSÒµÎñ 
//	{
//		QXLOG("GPRSÊı¾İ´íÎó!\r\n");
//		return ret;
//	}
//	if((ret =send_at_command("AT+CGACT=1","OK",AT_CMD_TIMEOUT)) != 0)
//	{
//		QXLOG("PDPÎ´¼¤»î!\r\n");
//		return ret;
//	}
	if((ret =send_at_command("AT+CGDCONT=1,\"IP\",\"CMNET\"","OK",3*AT_CMD_TIMEOUT)) != 0)
	{
		//ÉèÖÃPDPÉÏÏÂÎÄ,»¥ÁªÍø½ÓĞ­Òé,½ÓÈëµãµÈĞÅÏ¢
		QXLOG("set ip erro!\r\n");
		return ret;
	}
	if((ret = send_at_command("AT+CSOCKSETPN=1","OK",3*AT_CMD_TIMEOUT)) != 0) 
	{
		QXLOG("set CSOCKSETPN erro!\r\n");
		return ret;
	}
	if((ret = send_at_command("AT+CIPMODE=0","OK",3*AT_CMD_TIMEOUT)) != 0) 
	{
		//¸½×ÅGPRSÒµÎñ
		QXLOG("set model erro!\r\n");
		return ret;
	}
	if((ret = send_at_command("AT+NETOPEN","OK",3*AT_CMD_TIMEOUT)) != 0) 
	{
		QXLOG("net can not open!\r\n");
		return ret;
	}
	if((ret = send_at_command("AT+CIPHEAD=1","OK",2*AT_CMD_TIMEOUT)) != 0) 
	{
		//ÉèÖÃ½ÓÊÕÊı¾İÏÔÊ¾IPÍ·(·½±ãÅĞ¶ÏÊı¾İÀ´Ô´,½öÔÚµ¥Â·Á¬½ÓÓĞĞ§)
		QXLOG("set ip head erro!\r\n");
		return ret;
	}
	return ret;
}
/*******************************************************************************
º¯ Êı Ãû£ºint8_t gsm_ack_exception(void)
¹¦ÄÜÃèÊö£ºÒì³£Êı¾İ½ÓÊÕ				
Èë¿Ú²ÎÊı£º							
·µ»Ø²ÎÊı£º
´´½¨Ê±¼ä: 2017-11-02 by zam
********************************************************************************/
static uint8_t gsm_ack_exception(void)
{
	uint8_t ret = 0;
	if(find_string("+CME ERROR"))
	{
		if(qxwz_sdk_ntrip_status == QXWZ_STATUS_NTRIP_CONNECTED)
		{
			//ËµÃ÷Ô¶¶Ë·şÎñÆ÷ÒÑ¶Ï¿ª
			qxwz_soc_error(SOC_DEFUALT_CHANNEL);
		}
		ret = GSM_ACK_CME_ERROR;//GSM:²Ù×÷²»ÔÊĞí
	}
	else if(find_string("CONNECT FAIL"))
	{
		s_restart_gsm_flag = TRUE;
		s_connect_fail_flag = TRUE;
		ret = GSM_ACK_CONNECT_FAIL;
	}
	else if(find_string("IIII"))
	{
		ret = GSM_ACK_IIII;//Ä£¿é×Ô¼ºÖØÆô
	}
	else if(find_string("ALREAY CONNECT"))
	{
		ret = GSM_ACK_ALREAY_CONNECT;//ÒÑ½¨Á¢Á¬½Ó
	}
	else if(find_string("ERROR"))
	{//Ö¸Áîµ÷ÓÃ´íÎó
		ret = GSM_ACK_ERROR;  //error£¬Ö¸Áî¸ñÊ½´íÎó
	}
	if(ret != 0)
	{
		QXLOG("GSM init erro=%d....\r\n",ret);
	}
	return ret;
}

/*******************************************************************************
º¯ Êı Ãû£ºint8_t get_gsm_status(void)
¹¦ÄÜÃèÊö£º»ñÈ¡Ä£¿é¹¤×÷×´Ì¬		
Èë¿Ú²ÎÊı£º							
·µ»Ø²ÎÊı£º
´´½¨Ê±¼ä: 2017-11-02 by zam
********************************************************************************/
static void get_gsm_status(void)
{
	send_at_command("AT+CIPSTATUS","OK",AT_CMD_TIMEOUT);	//»ñÈ¡Á¬½Ó×´Ì¬
}
/*******************************************************************************
º¯ Êı Ãû£ºvoid enable_gsm_report_error(void)
¹¦ÄÜÃèÊö£ºÊ¹ÄÜÄ£¿éÉÏ±¨´íÎóĞÅÏ¢	
Èë¿Ú²ÎÊı£º							
·µ»Ø²ÎÊı£º
´´½¨Ê±¼ä: 2017-11-02 by zam
********************************************************************************/
void enable_gsm_report_error(void)
{
	send_at_command("AT+CMEE=?","OK",AT_CMD_TIMEOUT);	
	send_at_command("AT+CMEE=2","OK",AT_CMD_TIMEOUT);	
}

/*******************************************************************************
º¯ Êı Ãû£ºvoid restart_connect(void)
¹¦ÄÜÃèÊö£ºGSMÖØÆô,ÔÚled_taskµ÷ÓÃ,ÂÖÑ¯
Èë¿Ú²ÎÊı£º							
·µ»Ø²ÎÊı£º
´´½¨Ê±¼ä: 2017-11-02 by zam
********************************************************************************/
void restart_connect(void)
{
	if(s_restart_gsm_flag)
	{
		//send_at_command("AT+CFUN=0","OK",2*AT_CMD_TIMEOUT);	//½øÈë×îĞ¡¹¦ÄÜÄ£Ê½
		//delay_ms(1000);
		//send_at_command("AT+CFUN=1","OK",3*AT_CMD_TIMEOUT);	//½øÈëÈ«Ãæ¹¦ÄÜÄ£Ê½
		s_restart_gsm_flag = FALSE;
		s_is_init = FALSE;
	}
}
/*******************************************************************************
º¯ Êı Ãû£ºvoid get_qxwz_sdk_status(void)
¹¦ÄÜÃèÊö£º»ñÈ¡SDK×´Ì¬
Èë¿Ú²ÎÊı£º							
·µ»Ø²ÎÊı£º
´´½¨Ê±¼ä: 2017-11-02 by zam
********************************************************************************/
void get_qxwz_sdk_status(qxwz_rtcm_status_code status)
{
	qxwz_sdk_ntrip_status = status;
}

/**-------------------------------------Òì²½socket listÍ¨Öª»úÖÆ---------------------------------------*/
/*******************************************************************************
º¯ Êı Ãû£ºvoid async_notify(void)
¹¦ÄÜÃèÊö£ºÒì²½socket listÍ¨Öª»úÖÆ
Èë¿Ú²ÎÊı£º							
·µ»Ø²ÎÊı£º
´´½¨Ê±¼ä: 2017-11-02 by zam
********************************************************************************/
void async_notify(void)
{
	if(qxwz_prefs_flags_get() & QXWZ_PREFS_FLAG_SOCKET_ASYN)
	{
		//connect
		if(s_connect_ok_flag)
		{
			qxwz_soc_connect_complete(SOC_DEFUALT_CHANNEL,0);
			s_connect_ok_flag = FALSE;
		}
		else if(s_connect_fail_flag)
		{
			qxwz_soc_connect_complete(SOC_DEFUALT_CHANNEL,2);
			s_connect_fail_flag = FALSE;
		}
		//send
		if(s_send_ok_flag)
		{
			qxwz_soc_send_complete(SOC_DEFUALT_CHANNEL,0);
			s_send_ok_flag = FALSE;
		}
		else if(s_send_fail_flag)
		{
			qxwz_soc_send_complete(SOC_DEFUALT_CHANNEL,-1);
			s_send_fail_flag = FALSE;
		}
		//close
		if(s_close_ok_flag)
		{
			qxwz_soc_close_complete(SOC_DEFUALT_CHANNEL,0);
			s_close_ok_flag = FALSE;
		}
		else if(s_close_fail_flag)
		{
			qxwz_soc_close_complete(SOC_DEFUALT_CHANNEL,2);
			s_close_fail_flag = FALSE;
		}
	}
	
}

