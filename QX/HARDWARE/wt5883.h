#ifndef _WT_5883_H
#define _WT_5883_H

#include "stm32f10x.h"

/*********发送复位******************/
#define WT_RESET_PORT GPIOD
#define WT_RESET_RCC RCC_APB2Periph_GPIOD
#define WT_RESET_GPIO GPIO_Pin_2
#define WTRESET_HIGH GPIO_SetBits(WT_RESET_PORT,WT_RESET_GPIO)
#define WTRESET_LOW GPIO_ResetBits(WT_RESET_PORT,WT_RESET_GPIO)
/***************************/
/*********发送端口******************/
#define WT_DI_PORT GPIOC
#define WT_DI_RCC RCC_APB2Periph_GPIOC
#define WT_DI_GPIO GPIO_Pin_11
#define WTDI_HIGH GPIO_SetBits(WT_DI_PORT,WT_DI_GPIO)
#define WTDI_LOW GPIO_ResetBits(WT_DI_PORT,WT_DI_GPIO)
/***************************/

/*********时钟端口******************/
#define WT_CLK_PORT GPIOC
#define WT_CLK_RCC RCC_APB2Periph_GPIOC
#define WT_CLK_GPIO GPIO_Pin_10
#define WTCLK_HIGH GPIO_SetBits(WT_CLK_PORT,WT_CLK_GPIO)
#define WTCLK_LOW GPIO_ResetBits(WT_CLK_PORT,WT_CLK_GPIO)
/***************************/

/*********功放控制端口******************/
#define WT_SHUTDOWN_PORT GPIOA
#define WT_SHUTDOWN_RCC RCC_APB2Periph_GPIOA
#define WT_SHUTDOWN_GPIO GPIO_Pin_11
#define WT_SHUTDOWN_ON GPIO_SetBits(WT_SHUTDOWN_PORT,WT_SHUTDOWN_GPIO)
#define WT_SHUTDOWN_OFF GPIO_ResetBits(WT_SHUTDOWN_PORT,WT_SHUTDOWN_GPIO)
/***************************/


/*********输入端口******************/
#define WT_DO_PORT GPIOC
#define WT_DO_RCC RCC_APB2Periph_GPIOC
#define WT_DO_GPIO GPIO_Pin_12
#define WTDO_READ GPIO_ReadInputDataBit(WT_DO_PORT,WT_DO_GPIO) //语音芯片的输出
/***************************/
/**************************语音指令段位*********/
typedef enum
{
	BOOT_BEGIN=0,								//欢迎使用易飞行RTK系统
	SHUT_DOWN,									//关机
	LOW_POW,									//低电量
	RTK_FIX=3,									//固定解
	RTK_Float,									//浮动解
	SING_Point,									//单点解
	PSEU_Diff,									//伪距差分
	CONFIG_ING=7,								//进入配置模式
	SEND_Data,									//开始发送差分数据
	POW_alarm,									//电量警告
	GET_NET_OK=10,								//网络账号获取成功
	GET_NET_ERRO,								//网络账号获取失败
	NET_ERRO,									//网络初始化失败
	NO_SIM=13,									//未检测到sim卡
	CONFIG_WILESS_ADDR_OK,						//无线网络地址配置成功
	CONFIG_WILESS_UNIO_OK,						//无线单元地址配置成功
	NET_BASE=16,								//云基站设置成功
	NNET_BASE,									//固定基站设置成功
	NET_INSTRUMENT,								//云测绘仪设置成功
	NNET_INSTRUMENT,							//测绘仪设置成功
	NONE_RECIVE=20,								//没有收到数据
	ERRO_DATE,									//数据错误
	SYS_ERRO,									//系统故障
	WILESS_RECOVER,								//无线模块恢复出厂配置
	DIFF_DATE_OVER=24,							//差分数据超时
	SIM_CMD_ERRO,								//sim卡指令错误
	INPUT_COUNT,								//请输入差分账号
	BASE_PERF,									//当前设备为基站
	INSTRUMENT_PERF=28,							//当前设备为测绘仪
	FAULT_PERF,									//设备故障
	NOTICE_ERRO,								//错误警告
	GET_CMD,									//接收到指令
	SET_NAME,									//蓝牙名字修改成功
	NONE_DATE									//未知指令
}MUSIC_VOICE;
/**********************************************/

void WT588C_Init(void);
unsigned long int WTH_2L(unsigned long int dat);
unsigned char Play_voice(MUSIC_VOICE VOID_addr);
unsigned char WTH_Check_sate(void);

#endif
