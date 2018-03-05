/*
 * File      : spi.h
 * This file is ef comp
 * COPYRIGHT (C) 2017,
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at my addr
 *
 *
 * Change Logs:
 * Date           Author       ZAM
 * 2017-09-28     Bernard      the first version
 */
#ifndef __SPI_H
#define __SPI_H
 
#include "stm32f10x.h"
 
#define SYSTEM_SUPPORT_OS		//֧�ֲ���ϵͳ
 
#define TRUE 1
#define FALSE 0

/***SPI�豸ѡ��****/
#define SPI1_PERIPH
//#define SPI2_PERIPH
//#define SPI3_PERIPH
/*********************/

/***SPI�豸��ʼ������****/
#define SPI_CONFGI_1		\
{							\
	SPI_Direction_2Lines_FullDuplex,	\
	SPI_Mode_Master,					\
	SPI_DataSize_8b,					\
	SPI_CPOL_High,						\
	SPI_CPHA_2Edge,						\
	SPI_NSS_Soft,						\
	SPI_BaudRatePrescaler_2,			\
	SPI_FirstBit_MSB					\
}
#define SPI_CONFGI_2		\
{							\
	SPI_Direction_2Lines_FullDuplex,	\
	SPI_Mode_Master,					\
	SPI_DataSize_8b,					\
	SPI_CPOL_High,						\
	SPI_CPHA_2Edge,						\
	SPI_NSS_Soft,						\
	SPI_BaudRatePrescaler_2,			\
	SPI_FirstBit_MSB					\
}
#define SPI_CONFGI_3		\
{							\
	SPI_Direction_2Lines_FullDuplex,	\
	SPI_Mode_Master,					\
	SPI_DataSize_8b,					\
	SPI_CPOL_High,						\
	SPI_CPHA_2Edge, 					\
	SPI_NSS_Soft,						\
	SPI_BaudRatePrescaler_2,			\
	SPI_FirstBit_MSB					\
}
/*********************/

/*********************/
struct spi_config
{
	uint16_t spi_dir;		/*SPI ���䷽��**/
	uint16_t spi_mode;		/*SPI ����ģʽ**/
	uint16_t spi_datelenth;	/*SPI �������ݳ���**/
	uint16_t spi_cpol;		/*SPI ����ʱ��**/
	uint16_t spi_cpha;		/*SPI ʱ����ѡ��**/
	uint16_t spi_nss;		/*SPI Ƭѡ��**/
	uint16_t spi_baud;		/*SPI ��Ƶ**/
	uint16_t spi_first;		/*SPI ��ʼλ*****/
};
typedef enum
{
	OFF=0,
	ON   
}SELET;
/*********************/

/***SPI�豸��*********/
typedef enum
{
	FM25V_ID=0,
	MPU9250_ID
}DEV_ID;
/*********************/

/***SPI�豸Fram csѡ��**************************************/
#define FM25V_SPI SPI1
#define FM25V_PORT GPIOC
#define FM25V_RCC RCC_APB2Periph_GPIOC
#define FM25V_CS GPIO_Pin_4
#define FM25V_SET GPIO_SetBits(FM25V_PORT,FM25V_CS)
#define FM25V_RESET GPIO_ResetBits(FM25V_PORT,FM25V_CS)
/*****************************************************/
 /***SPI�豸MPU9250 csѡ��**************************************/
#define MPU9250_SPI SPI3
#define MPU9250_PORT GPIOB
#define MPU9250_RCC RCC_APB2Periph_GPIOB
#define MPU9250_CS GPIO_Pin_7
#define MPU9250_SET GPIO_SetBits(MPU9250_PORT,MPU9250_CS)
#define MPU9250_RESET GPIO_ResetBits(MPU9250_PORT,MPU9250_CS)
 /*****************************************************/


 void Spi_init(void);
 unsigned char SPI_ReadWriteByte(SPI_TypeDef* SPIx,unsigned char TxData);
 void Spi_select(DEV_ID devid,SELET selected);

 #endif
 
