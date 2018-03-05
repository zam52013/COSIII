/*
 * File      : IIC.h
 * This file is ef comp
 * COPYRIGHT (C) 2017,
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at my addr
 *
 *
 * Change Logs:
 * Date           Author       ZAM
 * 2017-10-19     Bernard      the first version
 */
#ifndef __IIC_H
#define __IIC_H

#include "stm32f10x.h"

/*********************/
typedef enum {Error = 0, Success = !Error } Status;
/*********************/

/***IIC设备选择****/
//#define IIC1_PERPH
//#define IIC2_PERPH
/*********************/

/********配置参数*************/
#define IIC_CONFIG_1	\
{						\
	I2C_Mode_I2C,		\
	I2C_DutyCycle_2,	\
	0x0A,				\
	I2C_Ack_Enable,		\
	I2C_AcknowledgedAddress_7bit,\
	400000						\
}
#define IIC_CONFIG_2	\
{						\
	I2C_Mode_I2C,		\
	I2C_DutyCycle_2,	\
	0x0A,				\
	I2C_Ack_Enable,		\
	I2C_AcknowledgedAddress_7bit,\
	400000						\
}

/*********************/
struct iic_config
{
	uint16_t iic_mode;		/*配置模式*/
	uint16_t iic_duty;		/*时钟频率*/
	uint16_t iic_addr;		/*本身地址*/
	uint16_t iic_ack;		/*应答使能*/
	uint16_t iic_ackaddr;	/*数据位*/
	uint32_t iic_clock;		/*时钟频率*/
};

/****iic函数*****************/
Status I2C_Read(I2C_TypeDef* I2Cx, uint8_t* buf, uint32_t nbuf, uint8_t SlaveAddress);
Status I2C_Write(I2C_TypeDef* I2Cx, const uint8_t* buf, uint32_t nbuf,  uint8_t SlaveAddress);
void IIC_Init(void);

/*********************/

#endif
