/*
 * File      : ef_qx.h
 * This file is ef comp
 * COPYRIGHT (C) 2017,
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at my addr
 *
 *
 * Change Logs:
 * Date           Author       ZAM
 * 2017-09-26     Bernard      the first version
 */
 #ifndef __EF_QX_H
 #define __EF_QX_H
 
 #include "socket.h"
 #include "delay.h"
 #include "usart.h"
 #include "main.h"
 #include "prefs.h"

 
/***********定义输出RTCM数据端口****************/
 #define RTCM_USART USART3
 /**********************************************/
 
/*************接收GGA的串口*******************************/
 #define GGA_USART USART3
 #define GGA_MESS USART3_MESSG
 /********************************************/
 /*************账户信息配置的串口*******************************/
 #define ACCOUNT_USART USART3
 #define ACCOUNT_MESS USART3_MESSG
 /********************************************/
 
  typedef struct
 {
		char appk_buff[20];
		char apps_buff[100];
		char id_buff[20];
		char type_buff[20];
 }qian_xun;
 
 
 extern qxwz_config config;
 
 void qxwz_status_response_callback(qxwz_rtcm_status_code status);
 void  get_qxwz_sdk_account_info(void);
 void qxwz_rtcm_response_callback(char* rtcm, size_t length);
 unsigned char Get_qx_count(char *str);

#endif
 
 
 
 
 
 