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

//GSM�ظ�ERROR
#define GSM_ACK_ERROR             1
//GSM����
#define GSM_ACK_IIII              2
//GSM TCP����ʧ��
#define GSM_ACK_CONNECT_FAIL      3
//GSM Զ�˹ر�����
#define GSM_ACK_CLOSED            4
//�ȴ�GSMģ��ظ���ʱ
#define GSM_ACK_TIMEOUT           5
//�ѽ�������
#define GSM_ACK_ALREAY_CONNECT    6
//ģ�鲻�������
#define GSM_ACK_CME_ERROR         7

//socket Ĭ��ͨ����
#define SOC_DEFUALT_CHANNEL       1   
//�������ݽ��ջ�����
#define BUF_MAX                   40 
//ATָ��ȴ��ظ���ʱʱ�䣬��λ��
#define AT_CMD_TIMEOUT    		   	5

#define RECV_DATA_BUF_MAX         2048

#define WAIT_CFG_TRY_SECOND       20
//ͳ��ʹ��
uint8_t recv_IIII_cnt = 0;
uint8_t recv_CLOSED_cnt = 0;
uint32_t timer_cnt = 0;
int32_t uart_irq_time_need = 0;


/** ----------------------------------------- INTERNAL ------------------------------------------*/

//�Ƿ��Ѿ���ʼ��GSMģ��
static BOOL s_is_init = FALSE;
//����ģ���־
static BOOL s_restart_gsm_flag = FALSE;

//��ʼ����GPRS TCPӦ�ò�����
static BOOL s_begin_recive = FALSE;

static uint8_t s_first_index = 0;
static uint16_t s_body_len = 0;
//�Ѿ����յ�������Ӧ�ò�����ݳ���
static uint16_t s_has_recived_len = 0;

//static char body_len_buf[5] = {0};//����body����
static char uart_exception_buf[10] = {0};
static char uart_buf[BUF_MAX] = {0};
static char recived_buf[RECV_DATA_BUF_MAX] = {0};//�������ݵĻ���


//��ʱ�����
static BOOL s_timer_start_flag = FALSE;	
static BOOL s_at_ack_timeout_flag = FALSE; 
static uint8_t s_at_ack_wait_time = 0;
static uint16_t s_tick_cnt = 0;

//qxwz_sdk ״̬
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


//socket�������������µ���

static void query_uart_buf(uint8_t recv);
static void uart_recv_exception(uint8_t recv);

//�첽socket
static unsigned char  s_connect_ok_flag = FALSE;
static unsigned char  s_connect_fail_flag = FALSE;

static unsigned char s_send_ok_flag = FALSE;
static unsigned char  s_send_fail_flag = FALSE;
static unsigned char s_close_ok_flag = FALSE;
static unsigned char s_close_fail_flag = FALSE;


/**-------------------------ģ��socket create connect send  close---------------------------------*/
/*******************************************************************************
�� �� ����void qxwz_soc_create
���������������������ʼ��
��ڲ�����soc:ͨ�� address:��������ַ
���ز�����0 �ɹ�
����ʱ��: 2017-11-02 by zam
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
�� �� ����void qxwz_soc_connect
�����������������������
��ڲ�����soc:ͨ�� address:��������ַ
���ز�����0 �ɹ�
����ʱ��: 2017-11-02 by zam
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
�� �� ����void qxwz_soc_send
�����������ر�����ģ�鷢������
��ڲ�����soc:ͨ�� send_buffer:���������� length:���ݳ���
���ز��������ݷ��ͳ���
����ʱ��: 2017-11-02 by zam
********************************************************************************/
qxwz_ssize_t qxwz_soc_send(qxwz_soc soc,char *send_buffer,size_t length)
{
	int8_t ret = 0;
	char sendbuf[1024];
	memset(sendbuf,0,sizeof(sendbuf));
	ret = send_at_command("AT+CIPSEND=0,",">",3*AT_CMD_TIMEOUT);
	if(ret != 0)
	{
		QXLOG("can not connect socker!\r\n");//��������
		qxwz_soc_error(SOC_DEFUALT_CHANNEL);
		return -1;	
	}
	QXLOG("send date......\r\n");//��������
	sprintf(sendbuf,"%s\32\0",send_buffer);
	QXLOG("%s\32\0",send_buffer);
	ret = send_at_command(sendbuf,"OK",3*AT_CMD_TIMEOUT);  //�ظ�OK��ʱ����Ե�
	if(ret != 0)
	{
		QXLOG("send erro.....\r\n");//��������
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
�� �� ����void qxwz_soc_close
�����������ر�����ģ������
��ڲ�����
���ز�����
����ʱ��: 2017-11-02 by zam
********************************************************************************/
void qxwz_soc_close(qxwz_soc soc)
{
	uint8_t local_soc = soc;
	int8_t ret = 0;	
	QXLOG("select close socket=%d.....\r\n",local_soc);//�����ر�����
	ret = send_at_command("AT+CIPCLOSE=0","CLOSE",3*AT_CMD_TIMEOUT);	//�ر�����
	if(ret != 0)
	{
		QXLOG("close socket erro.....\r\n");//��������
		if(qxwz_prefs_flags_get() & QXWZ_PREFS_FLAG_SOCKET_ASYN)
		{
			s_close_fail_flag = TRUE;
		}
	}
}

/*******************************************************************************
�� �� ����void qxwz_soc_init
��������������ģ���ʼ��
��ڲ�����
���ز�����
����ʱ��: 2017-11-02 by zam
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
	//GSM��ͨ��ģʽĬ��ʹ��ͨ��1
	return 1;	
	
}
/**------------------------------------------�жϷ���������--------------------------------------------*/
/*******************************************************************************
�� �� ����void USART1_IRQHandler
�������������2�жϷ������,����GSMģ��Ӧ�������
��ڲ�����
���ز�����
����ʱ��: 2017-11-02 by zam
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
			uart_buf[s_first_index] = res;  	  //�����յ����ַ����浽������ 
			if(!Task_Flag.Flag_Bit.Debug_Date)
			{
				DEBUG_UART->DR = res;//�����յ�������ת�������Դ���
			}
			else
			{
				BLUETOOTH_USART->DR = res;//�����յ�������ת�������Դ���
			}
			s_first_index++;                		//����ָ������ƶ�
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
�� �� ����void uart_recv_exception
������������ѯ����������ջ�����
��ڲ�����@param  recv    ���ڽ��յ����ַ�
���ز�����
����ʱ��: 2017-11-02 by zam
********************************************************************************/
static void query_uart_buf(uint8_t recv)
{
	char *qx_cnt;
	if((recv =='\n'))//�����ж��Ƿ������ݷ���  gsmģ�鷵�ص�����
	{
		if((qx_cnt=strstr(uart_buf,"+IPD"))!=NULL)
		{
			s_begin_recive = 1;
			qx_cnt+=4;
			s_body_len =atoi(qx_cnt);//���㽫Ҫ�յ�������
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
			QXLOG("$NO SIM\r\n");//���������ݷ��ص��װ�ȥ
			clear_buf();
		}
		else if((qx_cnt=strstr(uart_buf,"ERROR"))!=NULL)
		{
			clear_buf();
		}
	}
	if(qxwz_prefs_flags_get() & QXWZ_PREFS_FLAG_SOCKET_ASYN)//ʹ���첽SOCKET������Ҫ���������
	{
		if(recv == 'K')//�ж�һЩ����OK������״̬
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
�� �� ����void uart_recv_exception
�����������жϽ��մ�������
��ڲ�����
���ز�����
����ʱ��: 2017-11-02 by zam
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
			uart_exception_buf[s_index++] = recv;//��¼�쳣���ַ�
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
			s_begin_exception_recv_flag = FALSE;//��������
			qxwz_soc_error(SOC_DEFUALT_CHANNEL);
			s_is_init = FALSE;//���³�ʼ��
		}
		else if(!strcmp(uart_exception_buf,"CLOSE"))
		{
			s_begin_exception_recv_flag = FALSE;//��������
			qxwz_soc_error(SOC_DEFUALT_CHANNEL);
		}
	}
}

/*******************************************************************************
�� �� ����void TIM3_IRQHandler
������������ʱ��3�жϷ������
��ڲ�����
���ز�����
����ʱ��: 2017-11-02 by zam
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
�� �� ����void timer_tick
�����������ڶ�ʱ�ж��б�����
��ڲ�����	@param  tick_feq              1���ڵ���Ƶ��,��λ(HZ)
���ز�����
����ʱ��: 2017-11-02 by zam
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

/**-------------------------------------ATָ�����------------------------------------------------*/
/*******************************************************************************
�� �� ����void send_at_command
�������������ö�ʱ�� 
��ڲ�����	@param  send_str              �����͵�ATָ��	 @param  excepted_recv_str     �ڴ����յ�Ӧ��
���ز�����  @param  wait_time             ��ʱʱ��,��λ(S)
����ʱ��: 2017-11-02 by zam
********************************************************************************/
static int8_t send_at_command(char *send_str,char *excepted_recv_str,uint8_t wait_time)         
{
	uint8_t ret = 0;									
	if(send_str == NULL)
	{
		return -1;
	}
	clear_buf(); //�崮�ڽ��ջ�����
	s_timer_start_flag = FALSE;
	for (; *send_str!='\0';send_str++)//����AT����
	{
		while(USART_GetFlagStatus(GSM_USART, USART_FLAG_TC) == RESET);
		USART_SendData(GSM_USART,*send_str);
	}
	UART_SendString(GSM_USART,"\r\n");
	if((excepted_recv_str != NULL)&&(wait_time > 0))
	{
		start_at_ack_timer(wait_time);//������ʱ���ƶ�ʱ��
	}
	else
	{
		return 0;//ֱ�ӷ���
	}
	while(1)                    
	{
		if(!find_string(excepted_recv_str))
		{
			ret = gsm_ack_exception();	//û�յ��ڴ��յ���GSM Ӧ��	
			if(ret != 0)
			{
				break;
			}
    	}
		else
    	{
			//find the excepted string
			break;   //����ѭ��
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
	s_timer_start_flag = FALSE;  //ֹͣ��ʱ��
	s_at_ack_timeout_flag = FALSE;
	return ret;
}
/*******************************************************************************
�� �� ����void start_at_ack_timer(void)
���������� �жϻ������Ƿ���ָ�����ַ���
��ڲ�����	@param  str              �����͵�ATָ��	
���ز����� @return 1 �ҵ�ָ���ַ���0 δ�ҵ�ָ���ַ� 
����ʱ��: 2017-11-02 by zam
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
�� �� ����void start_at_ack_timer(void)
�������������ö�ʱ�� 
��ڲ�����		second  			  ��ʱʱ��,��λ(S)		
���ز����� 
����ʱ��: 2017-11-02 by zam
********************************************************************************/
static void start_at_ack_timer(uint8_t second)
{
	s_at_ack_wait_time = second; 
	s_timer_start_flag = TRUE;     
}
/*******************************************************************************
�� �� ����void clear_buf(void)
����������������ڽ��ջ�����,���ý���ƫ����Ϊ0		
��ڲ�����							
���ز����� 
����ʱ��: 2017-11-02 by zam
********************************************************************************/
static void clear_buf(void)
{
	//uint16_t k;
	//for(k = 0;k < BUF_MAX;k++) 
	//{
	//	uart_buf[k] = 0x00;
	//}
	memset(uart_buf, 0, BUF_MAX);
  	s_first_index = 0;              //�����ַ�������ʼ�洢λ��
}

/*******************************************************************************
�� �� ����void wait_creg(void)
�����������ȴ�ģ��ע��ɹ�				
��ڲ�����							
���ز����� @return 0  ok   else  errro
����ʱ��: 2017-11-02 by zam
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
			s_timer_start_flag = FALSE;//ֹͣ��ʱ��
			s_at_ack_timeout_flag = FALSE;
			return  GSM_ACK_TIMEOUT;
		}
	}//while(i == 0)   
	return 0;
}
	
/*******************************************************************************
�� �� ����void set_ate0(void)
�����������ص�����					
��ڲ�����							
���ز�����
����ʱ��: 2017-11-02 by zam
********************************************************************************/
static int8_t set_ate0(void)
{
	return (send_at_command("ATE0","OK",AT_CMD_TIMEOUT));								//ȡ������		
}
/*******************************************************************************
�� �� ����int8_t init_gsm_gprs(void)
������������ʼ��tcp����					
��ڲ�����							
���ز�����
����ʱ��: 2017-11-02 by zam
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
		Play_voice(NET_ERRO);//ע�����
		return -1;
	}
	if((ret = set_ate0()) != 0)//�رջ���
	{
		return ret;
	}
//	send_at_command("AT+CREG=1","OK",3*AT_CMD_TIMEOUT);//������ʾ����ע�����Ϣ
	if((ret = send_at_command("AT+CGREG=1","OK",3*AT_CMD_TIMEOUT)) != 0) //����GPRS�ƶ�̨���ΪA,֧�ְ����������ݽ��� 
	{
		QXLOG("net reg erro!\r\n");
		return ret;
	}		
//	if((ret = send_at_command("AT+CGCLASS=\"A\"","OK",3*AT_CMD_TIMEOUT)) != 0) //����GPRS�ƶ�̨���ΪA,֧�ְ����������ݽ��� 
//	{
//		QXLOG("A���ƶ�̨���ô���!\r\n");
//		return ret;
//	}		
//	if((ret = send_at_command("AT+CGATT=1","OK",3*AT_CMD_TIMEOUT)) != 0)//����GPRSҵ�� 
//	{
//		QXLOG("GPRS���ݴ���!\r\n");
//		return ret;
//	}
//	if((ret =send_at_command("AT+CGACT=1","OK",AT_CMD_TIMEOUT)) != 0)
//	{
//		QXLOG("PDPδ����!\r\n");
//		return ret;
//	}
	if((ret =send_at_command("AT+CGDCONT=1,\"IP\",\"CMNET\"","OK",3*AT_CMD_TIMEOUT)) != 0)
	{
		//����PDP������,��������Э��,��������Ϣ
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
		//����GPRSҵ��
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
		//���ý���������ʾIPͷ(�����ж�������Դ,���ڵ�·������Ч)
		QXLOG("set ip head erro!\r\n");
		return ret;
	}
	return ret;
}
/*******************************************************************************
�� �� ����int8_t gsm_ack_exception(void)
�����������쳣���ݽ���				
��ڲ�����							
���ز�����
����ʱ��: 2017-11-02 by zam
********************************************************************************/
static uint8_t gsm_ack_exception(void)
{
	uint8_t ret = 0;
	if(find_string("+CME ERROR"))
	{
		if(qxwz_sdk_ntrip_status == QXWZ_STATUS_NTRIP_CONNECTED)
		{
			//˵��Զ�˷������ѶϿ�
			qxwz_soc_error(SOC_DEFUALT_CHANNEL);
		}
		ret = GSM_ACK_CME_ERROR;//GSM:����������
	}
	else if(find_string("CONNECT FAIL"))
	{
		s_restart_gsm_flag = TRUE;
		s_connect_fail_flag = TRUE;
		ret = GSM_ACK_CONNECT_FAIL;
	}
	else if(find_string("IIII"))
	{
		ret = GSM_ACK_IIII;//ģ���Լ�����
	}
	else if(find_string("ALREAY CONNECT"))
	{
		ret = GSM_ACK_ALREAY_CONNECT;//�ѽ�������
	}
	else if(find_string("ERROR"))
	{//ָ����ô���
		ret = GSM_ACK_ERROR;  //error��ָ���ʽ����
	}
	if(ret != 0)
	{
		QXLOG("GSM init erro=%d....\r\n",ret);
	}
	return ret;
}

/*******************************************************************************
�� �� ����int8_t get_gsm_status(void)
������������ȡģ�鹤��״̬		
��ڲ�����							
���ز�����
����ʱ��: 2017-11-02 by zam
********************************************************************************/
static void get_gsm_status(void)
{
	send_at_command("AT+CIPSTATUS","OK",AT_CMD_TIMEOUT);	//��ȡ����״̬
}
/*******************************************************************************
�� �� ����void enable_gsm_report_error(void)
����������ʹ��ģ���ϱ�������Ϣ	
��ڲ�����							
���ز�����
����ʱ��: 2017-11-02 by zam
********************************************************************************/
void enable_gsm_report_error(void)
{
	send_at_command("AT+CMEE=?","OK",AT_CMD_TIMEOUT);	
	send_at_command("AT+CMEE=2","OK",AT_CMD_TIMEOUT);	
}

/*******************************************************************************
�� �� ����void restart_connect(void)
����������GSM����,��led_task����,��ѯ
��ڲ�����							
���ز�����
����ʱ��: 2017-11-02 by zam
********************************************************************************/
void restart_connect(void)
{
	if(s_restart_gsm_flag)
	{
		//send_at_command("AT+CFUN=0","OK",2*AT_CMD_TIMEOUT);	//������С����ģʽ
		//delay_ms(1000);
		//send_at_command("AT+CFUN=1","OK",3*AT_CMD_TIMEOUT);	//����ȫ�湦��ģʽ
		s_restart_gsm_flag = FALSE;
		s_is_init = FALSE;
	}
}
/*******************************************************************************
�� �� ����void get_qxwz_sdk_status(void)
������������ȡSDK״̬
��ڲ�����							
���ز�����
����ʱ��: 2017-11-02 by zam
********************************************************************************/
void get_qxwz_sdk_status(qxwz_rtcm_status_code status)
{
	qxwz_sdk_ntrip_status = status;
}

/**-------------------------------------�첽socket list֪ͨ����---------------------------------------*/
/*******************************************************************************
�� �� ����void async_notify(void)
�����������첽socket list֪ͨ����
��ڲ�����							
���ز�����
����ʱ��: 2017-11-02 by zam
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

