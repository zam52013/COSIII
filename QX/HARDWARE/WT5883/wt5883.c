/*
 * File      : bluetooth.h
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
#include "wt5883.h"
#include "delay.h"

 /*--------------------------------------
 ;模块名称:WT588C_Init
 ;功 能:初始化端口
 ;入 参:
 ;出 参:
 ;-------------------------------------*/
 void WT588C_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(WT_RESET_RCC | WT_DI_RCC | WT_CLK_RCC | WT_DO_RCC | WT_SHUTDOWN_RCC,ENABLE);
	GPIO_InitStructure.GPIO_Pin=WT_RESET_GPIO;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(WT_RESET_PORT,&GPIO_InitStructure);
	WTDI_LOW;

	GPIO_InitStructure.GPIO_Pin=WT_DI_GPIO;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(WT_DI_PORT,&GPIO_InitStructure);
	WTDI_LOW;
	
	GPIO_InitStructure.GPIO_Pin=WT_CLK_GPIO;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(WT_CLK_PORT,&GPIO_InitStructure);
	WTCLK_LOW;
	
	GPIO_InitStructure.GPIO_Pin=WT_DO_GPIO;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init(WT_DO_PORT,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin=WT_SHUTDOWN_GPIO;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(WT_SHUTDOWN_PORT,&GPIO_InitStructure);
	WT_SHUTDOWN_ON;//默认打开功放
	WTH_2L(0x0a0140); //芯片初始化
	#ifdef DEBUG_PERPH
	printf("wt5883 open!\r\n");
	#endif
}

/*--------------------------------------
;模块名称:WTH_2L
;功 能:WTH 二线串口通信函数,高位在前,发送 24 位数据
;入 参:发送的 24 位数据
;出 参:芯片返回的数据
;-------------------------------------*/
unsigned long int WTH_2L(unsigned long int dat)
{
	unsigned char i;
	unsigned long int return_dat;
	WTCLK_HIGH;
	delay_us(10);
	for (i = 0; i < 24; i++)
	{
		WTCLK_HIGH;
		if (dat & 0x800000) WTDI_HIGH;
		else WTDI_LOW;
		dat <<= 1;
		delay_us(10); //
		if (WTDO_READ) return_dat |= 0x01;
		else return_dat &= ~(0x01);
		return_dat <<= 1;
		delay_us(10);//
		WTCLK_LOW;
		delay_us(10); //
	}
	delay_us(10); //
	if (WTDO_READ) return_dat |= 0x01;
	else return_dat &= ~(0x01);
	return_dat &= 0x7ffffe; //屏蔽前后无用的数据
	return return_dat;
}
/*--------------------------------------
;模块名称: Play_voice
;功 能:WTH 二线串口通信,WTH 芯片播放语音
;入 参:播放的地址 0~255
;出 参:0：播放失败； 1：播放成功
;-------------------------------------*/
unsigned char Play_voice(MUSIC_VOICE VOID_addr)
{
	unsigned long int dat;
	unsigned char addr;
	addr=VOID_addr;
	WTRESET_HIGH;
	delay_us(10);
	WTRESET_LOW;
	dat = 0x1800c8 + (addr << 5);
	if (WTH_2L(dat) == dat) return 1; //播放成功
	return 0; //播放失败
}
/*--------------------------------------
;模块名称:WTH_Check_sate
;功 能:WTH 二线串口通信,查询 WTH 芯片状态，高位在前发送 16 位数据
;入 参:无
;出 参:0：芯片未处于播放状态； 1：芯片正在播放状态
;-------------------------------------*/
unsigned char WTH_Check_sate(void)
{
	unsigned char i;
	unsigned long int dat = 0x2200;
	unsigned long int return_dat;
	WTCLK_HIGH;
	delay_us(200);
	for (i = 0; i < 16; i++)
	{
		WTCLK_HIGH;
		if (dat & 0x8000) WTDI_HIGH;
		else WTDI_LOW;
		dat <<= 1;
		delay_us(50); //等待 50us 后才读取 DO 数据
		if (WTDO_READ) return_dat |= 0x01;
		else return_dat &= ~(0x01);
		return_dat <<= 1;
		delay_us(50); //50us
		WTCLK_LOW;
		delay_us(100); //100us
	}
	if (WTDO_READ) return_dat |= 0x01;
	return_dat &= 0xffff;
	if (return_dat & 0x80) //只判断第 9 位数据
		return 1; //正在播放
	return 0; //未播放
}
	

