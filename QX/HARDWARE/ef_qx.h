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

 
/***********�������RTCM���ݶ˿�****************/
 #define RTCM_USART USART3
 /**********************************************/
 
/*************����GGA�Ĵ���*******************************/
 #define GGA_USART USART3
 #define GGA_MESS USART3_MESSG
 /********************************************/
 
 typedef struct
 {
		char appk_buff[20];
		char apps_buff[100];
		char id_buff[20];
		char type_buff[20];
 }qian_xun;
 
 typedef struct
{
	double utc;				//UTCʱ��
	double lat;				//γ��
	char   lat_dir;			//γ�ȷ���
	double lon;				//����
	char   lon_dir;			//���ȷ���
	char   quality;			//��λ����ָʾ
	char   sats;			//�������Ǹ���
	float  hdop;			//ˮƽ����
	float  alt;				//���߸߶�
	char   a_units;			//���߸߶ȵ�λ
	float  undulation;		//���ˮ׼��̲߳�
	char   u_units;			//�̲߳λ
	char   age;				//��ֻ�վ�ο����
}gpgga;
 
typedef struct
{
	double latitude;				//γ��
	double longitude;				//����
	double hgt;				//�߳�
}rtkposa;

extern gpgga GPGGA;
extern rtkposa GET_POSA;
extern rtkposa RTKPOSA_BUFF[3];


 extern qxwz_config config;
 extern unsigned char Init_Sim;
 void qxwz_status_response_callback(qxwz_rtcm_status_code status);
 void  get_qxwz_sdk_account_info(void);
 void qxwz_rtcm_response_callback(char* rtcm, size_t length);
 unsigned char Get_qx_count(char *str);
 void RTK_Status(char* buffe,int buff_len);

#endif
 
 
 
 
 
 



