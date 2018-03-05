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
 ;ģ������:WT588C_Init
 ;�� ��:��ʼ���˿�
 ;�� ��:
 ;�� ��:
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
	WT_SHUTDOWN_ON;//Ĭ�ϴ򿪹���
	WTH_2L(0x0a0140); //оƬ��ʼ��
	#ifdef DEBUG_PERPH
	printf("wt5883 open!\r\n");
	#endif
}

/*--------------------------------------
;ģ������:WTH_2L
;�� ��:WTH ���ߴ���ͨ�ź���,��λ��ǰ,���� 24 λ����
;�� ��:���͵� 24 λ����
;�� ��:оƬ���ص�����
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
	return_dat &= 0x7ffffe; //����ǰ�����õ�����
	return return_dat;
}
/*--------------------------------------
;ģ������: Play_voice
;�� ��:WTH ���ߴ���ͨ��,WTH оƬ��������
;�� ��:���ŵĵ�ַ 0~255
;�� ��:0������ʧ�ܣ� 1�����ųɹ�
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
	if (WTH_2L(dat) == dat) return 1; //���ųɹ�
	return 0; //����ʧ��
}
/*--------------------------------------
;ģ������:WTH_Check_sate
;�� ��:WTH ���ߴ���ͨ��,��ѯ WTH оƬ״̬����λ��ǰ���� 16 λ����
;�� ��:��
;�� ��:0��оƬδ���ڲ���״̬�� 1��оƬ���ڲ���״̬
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
		delay_us(50); //�ȴ� 50us ��Ŷ�ȡ DO ����
		if (WTDO_READ) return_dat |= 0x01;
		else return_dat &= ~(0x01);
		return_dat <<= 1;
		delay_us(50); //50us
		WTCLK_LOW;
		delay_us(100); //100us
	}
	if (WTDO_READ) return_dat |= 0x01;
	return_dat &= 0xffff;
	if (return_dat & 0x80) //ֻ�жϵ� 9 λ����
		return 1; //���ڲ���
	return 0; //δ����
}
	

