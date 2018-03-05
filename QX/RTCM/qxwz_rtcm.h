
#ifndef __QXWZ_RTCM_H
#define __QXWZ_RTCM_H

#include <stdio.h>
#include <stdint.h>
#include <time.h>
/**
* SDK配置信息
*/
typedef struct {
    char *appkey;
    char *appsecret;
    char *device_ID;
    char *device_Type;
} qxwz_config;

typedef enum {
    QXWZ_STATUS_NTRIP_CONNECTED = 1000, //ntrip连接到服务器
    QXWZ_STATUS_NTRIP_CONNECTING = 1001, //正在连接ntrip服务器
    QXWZ_STATUS_NTRIP_DISCONNECTED = 1002, //ntrip断开服务器
    QXWZ_STATUS_NTRIP_RECEIVINGDATA = 1003, //ntrip正在接受数据
	
	
	
    QXWZ_NET_WORK_ERROR = 1004, //网络异常
    QXWZ_APPKEY_IDENTIFY_FAIL = 1005, //appKey错误
    QXWZ_NTRIP_USER_FAIL = 1006, //appkey,appsecret验证失败
    QXWZ_DEVICEID_NULL = 1007, //deviceid错误
    QXWZ_SETTING_NULL = 1008, //setting错误
    QXWZ_NO_SETTING = 1009, //没有调用setting函数
    QXWZ_NTRIP_USER_MAX = 1010, //appkey用户已经达到上限
    QXWZ_DEVICETYPE_NULL = 1011, //deviceType错误

    //SDK 0.0.5新增状态码
    QXWZ_STATUS_OPENAPI_PARAM_MISSING = 2001, //缺少参数
    QXWZ_STATUS_OPENAPI_ACCOUNT_NOT_EXIST = 2002, //账号不存在
    QXWZ_STATUS_OPENAPI_DUPLICATE_ACCOUNT = 2003, //账号重复
    QXWZ_STATUS_OPENAPI_INCORRECT_PASSWORD = 2004, //错误密码
    QXWZ_STATUS_OPENAPI_DISABLED_ACCOUNT = 2005, //账号未激活
    QXWZ_STATUS_OPENAPI_NO_AVAILABLE_ACCOUNT = 2006, //没有有效的账号
    QXWZ_STATUS_OPENAPI_NO_RELATED_POPUSER = 2007, //POPUser不存在
    QXWZ_STATUS_OPENAPI_SYSTEM_ERROR = 2008, //服务端内部错误
		QXWZ_STATUS_OPENAPI_ACCOUNT_EXPIRED = 2010, //账号已过期，需续费
		QXWZ_STATUS_OPENAPI_ACCOUNT_TOEXPIRE = 2011, //账号即将过期

    // SDK 0.9.2新增状态码
    QXWZ_STATUS_SDK_INTERNAL_ERROR = 1020,      // SDK内部错误
    QXWZ_STATUS_NTRIP_RTCM_SUCCESS = 1021,      // Ntrip播发数据正常
    QXWZ_STATUS_NTRIP_UNAUTHORIZED = 1022,      // Ntrip认证失败
} qxwz_rtcm_status_code;


/**
* SDK 账号信息
*/
typedef struct {
		char *appkey;
		char *deviceID;
		char *deviceType;
		char *serviceType;
		time_t expire_time;  /*自1970年1月1日的秒数*/
		char *NtripUserName;
		char *NtripPassword;
} qxwz_account_info;


/**
* 当收到RTCM数据时，向外部程序通知收到的RTCM数据
*
*  @param rtcm      rtcm数据
*  @param length    rtcm数据长度
*
*/
typedef void(*qxwz_rtcm_response)(char* rtcm, size_t length);


/**
* 错误事件通知
*
*/
typedef void(*qxwz_status_response)(qxwz_rtcm_status_code status);

/**
* 配置SDK
*/
void qxwz_setting(qxwz_config* config);

/**
* 启动SDK
*/
void qxwz_start(qxwz_rtcm_response rtcm_rsp, qxwz_status_response status_rsp);

/**
* 停止SDK
*/
void qxwz_stop(void);

/**
* 可在此方法内执行具体的任务，由外部程序定时调用，由于网路超时等情况，外部程序可能不能及时调用
*
*  @param system_time 系统时间
*/
void qxwz_tick(int32_t system_time);


/**
*
* 发送GGA字符串，用来获取rtcm数据
*
* @param ggastring  GGA字符串
*
*/
void qxwz_send_gga(char* ggastring);


/**
*
* 查询用户账号信息
*
* @return  qxwz_account_info* 
*
*/
qxwz_account_info* getqxwzAccount(void);

#endif
