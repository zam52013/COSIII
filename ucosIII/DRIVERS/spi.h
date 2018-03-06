/*
 * File      : spi.h
 * This file is ef comp
 * COPYRIGHT (C) 2017,
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at my addr
 *+-----------------------------------------------------------+
 * |                     ����˵��                            |
 +-----------------------------+---------------+-------------+
  *SPI1: PA5....SCK    PA6....MISO    PA7....MOSI
 *SPI2: PB13....SCK     PB14....MISO    PB15....MOSI
 *SPI3: PB3....SCK    PB4....MISO    PB5....MOSI
 *
 * Change Logs:
 * Date           Author       ZAM
 * 2017-09-28     Bernard      the first version
 */
#ifndef __SPI_H
#define __SPI_H
 
 
  #ifdef __cplusplus
 extern "C" {
#endif 
	 
	 
#include "stm32f10x.h"
 
#define SYSTEM_SUPPORT_OS		//֧�ֲ���ϵͳ
 
#define TRUE 1
#define FALSE 0

/***SPI�豸ѡ��****/
//#define SPI1_PERIPH
#define SPI2_PERIPH
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
	SPI_BaudRatePrescaler_4,			\
	SPI_FirstBit_MSB					\
}
#define SPI_CONFGI_2		\
{							\
	SPI_Direction_2Lines_FullDuplex,	\
	SPI_Mode_Master,					\
	SPI_DataSize_8b,					\
	SPI_CPOL_Low,						\
	SPI_CPHA_1Edge,						\
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

 void Spi_init(void);
 unsigned char SPI_ReadWriteByte(SPI_TypeDef* SPIx,unsigned char TxData);

#ifdef __cplusplus
}
#endif

 #endif
 
