/*
 * File      : key_pow.c
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
 #include "key_pow.h"
 #include "usart.h"

 void All_Pow_Io_Init(void)
 {
	GPIO_InitTypeDef  GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(ALL_POWER_RCC | ALL_POWER_KYE_RCC, ENABLE);	 

	GPIO_InitStructure.GPIO_Pin=ALL_POWER_GPIO;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(ALL_POWER_CONTRO_PORT,&GPIO_InitStructure);
	POWER_OFF;
	
 	GPIO_InitStructure.GPIO_Pin=ALL_POWER_KYE_GPIO;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init(ALL_POWER_KYE_PORT,&GPIO_InitStructure);
	PWR_BackupAccessCmd(ENABLE);
	RCC_LSEConfig(RCC_LSE_OFF);
	PWR_BackupAccessCmd(DISABLE);	
 	#ifdef DEBUG_PERPH
 	printf("pow open!\r\n");
 	#endif
 }
/********************************************************************
***函数名称:Key_Scan()
***函数说明:按键扫描程序，每10ms扫描检测一次
***输入参数:无
***输出参数:按键状态
***1:滤波状态  2:短按下 3:短弹起 4:长按下 5:长弹起
********************************************************************/
unsigned char Key_Scan(void)
{
	static unsigned char key_stus=0;
	static unsigned char KEY_COUNT=0;
	unsigned char KEY_FUN;
	unsigned key_buff=POWER_KEY_READ;
	KEY_COUNT++;
	switch(key_stus)
	{
		case 0:	
			if(key_buff==0)//按键按下准备滤波处理
			{
				key_stus++;
				KEY_FUN=KEY_FILE;//滤波状态
			}
			else
			{
				key_stus=0;
			}
			break;
		case 1:
			if(key_buff==0)
			{
				key_stus++;
				KEY_FUN=KEY_SHORT_DOWN;//滤波成功为短按
				KEY_COUNT=0;//开始按键计数
			}
			else
			{
				key_stus=0;//没有被按下
			}
			break;
		case 2:
			if(key_buff==0)
			{
				if(KEY_COUNT>KEY_LONG_TIME)//按键时间为2S
				{
					key_stus++;
					KEY_COUNT=KEY_LONG_TIME+1;
					KEY_FUN=KEY_LONG_DOWN;//长按下
				}
			}
			else
			{
				key_stus=0;
				KEY_FUN=KEY_SHORT_UP;//短按弹起
			}
			break;
		case 3:
			if(key_buff)
			{
				KEY_FUN=KEY_LONG_UP;//长按弹起
				key_stus=0;
			}
			break;
		default:
			key_stus=0;
	}
	return KEY_FUN;
}



 
 