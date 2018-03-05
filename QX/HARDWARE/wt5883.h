#ifndef _WT_5883_H
#define _WT_5883_H

#include "stm32f10x.h"

/*********���͸�λ******************/
#define WT_RESET_PORT GPIOD
#define WT_RESET_RCC RCC_APB2Periph_GPIOD
#define WT_RESET_GPIO GPIO_Pin_2
#define WTRESET_HIGH GPIO_SetBits(WT_RESET_PORT,WT_RESET_GPIO)
#define WTRESET_LOW GPIO_ResetBits(WT_RESET_PORT,WT_RESET_GPIO)
/***************************/
/*********���Ͷ˿�******************/
#define WT_DI_PORT GPIOC
#define WT_DI_RCC RCC_APB2Periph_GPIOC
#define WT_DI_GPIO GPIO_Pin_11
#define WTDI_HIGH GPIO_SetBits(WT_DI_PORT,WT_DI_GPIO)
#define WTDI_LOW GPIO_ResetBits(WT_DI_PORT,WT_DI_GPIO)
/***************************/

/*********ʱ�Ӷ˿�******************/
#define WT_CLK_PORT GPIOC
#define WT_CLK_RCC RCC_APB2Periph_GPIOC
#define WT_CLK_GPIO GPIO_Pin_10
#define WTCLK_HIGH GPIO_SetBits(WT_CLK_PORT,WT_CLK_GPIO)
#define WTCLK_LOW GPIO_ResetBits(WT_CLK_PORT,WT_CLK_GPIO)
/***************************/

/*********���ſ��ƶ˿�******************/
#define WT_SHUTDOWN_PORT GPIOA
#define WT_SHUTDOWN_RCC RCC_APB2Periph_GPIOA
#define WT_SHUTDOWN_GPIO GPIO_Pin_11
#define WT_SHUTDOWN_ON GPIO_SetBits(WT_SHUTDOWN_PORT,WT_SHUTDOWN_GPIO)
#define WT_SHUTDOWN_OFF GPIO_ResetBits(WT_SHUTDOWN_PORT,WT_SHUTDOWN_GPIO)
/***************************/


/*********����˿�******************/
#define WT_DO_PORT GPIOC
#define WT_DO_RCC RCC_APB2Periph_GPIOC
#define WT_DO_GPIO GPIO_Pin_12
#define WTDO_READ GPIO_ReadInputDataBit(WT_DO_PORT,WT_DO_GPIO) //����оƬ�����
/***************************/
/**************************����ָ���λ*********/
typedef enum
{
	BOOT_BEGIN=0,								//��ӭʹ���׷���RTKϵͳ
	SHUT_DOWN,									//�ػ�
	LOW_POW,									//�͵���
	RTK_FIX=3,									//�̶���
	RTK_Float,									//������
	SING_Point,									//�����
	PSEU_Diff,									//α����
	CONFIG_ING=7,								//��������ģʽ
	SEND_Data,									//��ʼ���Ͳ������
	POW_alarm,									//��������
	GET_NET_OK=10,								//�����˺Ż�ȡ�ɹ�
	GET_NET_ERRO,								//�����˺Ż�ȡʧ��
	NET_ERRO,									//�����ʼ��ʧ��
	NO_SIM=13,									//δ��⵽sim��
	CONFIG_WILESS_ADDR_OK,						//���������ַ���óɹ�
	CONFIG_WILESS_UNIO_OK,						//���ߵ�Ԫ��ַ���óɹ�
	NET_BASE=16,								//�ƻ�վ���óɹ�
	NNET_BASE,									//�̶���վ���óɹ�
	NET_INSTRUMENT,								//�Ʋ�������óɹ�
	NNET_INSTRUMENT,							//��������óɹ�
	NONE_RECIVE=20,								//û���յ�����
	ERRO_DATE,									//���ݴ���
	SYS_ERRO,									//ϵͳ����
	WILESS_RECOVER,								//����ģ��ָ���������
	DIFF_DATE_OVER=24,							//������ݳ�ʱ
	SIM_CMD_ERRO,								//sim��ָ�����
	INPUT_COUNT,								//���������˺�
	BASE_PERF,									//��ǰ�豸Ϊ��վ
	INSTRUMENT_PERF=28,							//��ǰ�豸Ϊ�����
	FAULT_PERF,									//�豸����
	NOTICE_ERRO,								//���󾯸�
	GET_CMD,									//���յ�ָ��
	SET_NAME,									//���������޸ĳɹ�
	NONE_DATE									//δָ֪��
}MUSIC_VOICE;
/**********************************************/

void WT588C_Init(void);
unsigned long int WTH_2L(unsigned long int dat);
unsigned char Play_voice(MUSIC_VOICE VOID_addr);
unsigned char WTH_Check_sate(void);

#endif
