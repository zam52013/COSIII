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
 
 typedef struct
 {
		char appk_buff[20];
		char apps_buff[100];
		char id_buff[20];
		char type_buff[20];
 }qian_xun;
 
 typedef struct
{
	double utc;				//UTC时间
	double lat;				//纬度
	char   lat_dir;			//纬度方向
	double lon;				//经度
	char   lon_dir;			//经度方向
	char   quality;			//定位质量指示
	char   sats;			//解算卫星个数
	float  hdop;			//水平精度
	float  alt;				//天线高度
	char   a_units;			//天线高度单位
	float  undulation;		//大地水准面高程差
	char   u_units;			//高程差单位
	char   age;				//差分基站参考标号
}gpgga;
 
typedef struct
{
	double latitude;				//纬度
	double longitude;				//经度
	double hgt;				//高程
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
 
 
 
 
 
 



