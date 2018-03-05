#ifndef __QXWZ_SOC_H
#define __QXWZ_SOC_H
#include <stdlib.h>
#include <stdint.h>
#define QXWZ_SOCKET_DATA_CACHE_LEN 1024//socket数据包缓存大小
typedef int qxwz_ssize_t;
typedef int8_t qxwz_soc;


typedef struct {
    char *hostName;
    short port;
}qxwz_soc_address;//连接地址

/**
 *  创建一个TCP(***宿主实现***)
 *  @return tcp通道号
 */
qxwz_soc qxwz_soc_create(void);


/**
 *  连接到指定的服务器(***宿主实现***)
 *
 *  @param soc         tcp通道号(由调用方法qxwz_soc_create获得)
 *  @param address 服务器地址
 *
 *  @return 连接状态(0:连接成功,1:address无效,2:连接失败)
 */
int8_t qxwz_soc_connect(qxwz_soc soc,qxwz_soc_address address);



/**
 *  发送数据(***宿主实现***)
 *
 *  @param soc         tcp通道号
 *  @param send_buffer 发送的数据缓存
 *  @param length      发送数据的长度
 *
 *  @return 实际发送数据的长度(如果发送失败返回-1)
 */

qxwz_ssize_t qxwz_soc_send(qxwz_soc soc,char *send_buffer,size_t length); 


/**
 *  接收到数据回掉，当宿主接收到数据的事时候，由宿主调用本方法。
 *	目前sdk采取的方案是，每个数据包大小超过缓存大小，那么直接丢弃，另外如果缓存中存不下下一个数据包了，那么这个数据包也会被丢弃
 *  @param soc         tcp通道号
 *  @param send_buffer 接收数据缓存
 *  @param length     	接收数据的长度
 *
 */
void qxwz_on_soc_data_received(qxwz_soc soc, char * buffer, size_t length);

/**
 *  关闭某个TCP链接(***宿主实现***)
 *
 *  @param soc TCP通道号
 */
void qxwz_soc_close(qxwz_soc soc);

/**
 *  当socket连接发生错误的时候由宿主调用通知给SDK
 *
 *  @param soc TCP通道号
 */
void qxwz_soc_error(qxwz_soc soc);
 




/** -------------------------------------------- 使用异步SOCKET时调用 -------------------------------------------------*/


/**
 *  连接完成回调,使用异步socket时使用
 *
 *  @param soc         tcp通道号(由调用方法qxwz_soc_create获得)
 *  @param connect_status 是否连接成功(0:连接成功,1:address无效,2:连接失败)
 */
void qxwz_soc_connect_complete(qxwz_soc soc,int8_t connect_status);



/**
 * 	数据发送完成回调
 *
 *  @param soc         tcp通道号(由调用方法qxwz_soc_create获得)
 *  @param sendLength 实际发送数据的长度(如果发送失败返回-1)
 */
void qxwz_soc_send_complete(qxwz_soc soc,qxwz_ssize_t send_length);




/**
 *  关闭某个TCP连接，回调
 *
 *  @param soc TCP通道号
 *  @param close_status 是否关闭成功(0:关闭成功,1:address无效,2:关闭失败)
 */
void qxwz_soc_close_complete(qxwz_soc soc,int8_t close_status);

#endif
