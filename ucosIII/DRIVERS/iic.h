/*
 * File      : IIC.h
 * This file is ef comp
 * COPYRIGHT (C) 2017,
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at my addr
 *+-----------------------------------------------------------+
 * |                     引脚说明                            |
 +-----------------------------+---------------+-------------+
 * IIC1:   PB6...SCL      PB7....SDA
 * IIC2:		PB10...SCL     PB11...SDA
 *
 * Change Logs:
 * Date           Author       ZAM
 * 2017-10-19     Bernard      the first version
 */
#ifndef __IIC_H
#define __IIC_H

 #ifdef __cplusplus
 extern "C" {
#endif 
	 
#include "stm32f10x.h"


/***IIC设备选择****/
#define IIC1_PERPH

//#define IIC2_PERPH
//#define SOFT_IIC
/*********************/
/***IIC-DMA设备选择****/
//#define IIC1_DMA_TR

//#define IIC2_DMA_TR
/*********************/
/**************软件模拟IIC端口配置**********************/
#define SOFT_IIC_PORT GPIOB
#define SOFT_IIC_RCC RCC_APB2Periph_GPIOB

#define SOFT_IIC_SCL_GPIO GPIO_Pin_6
#define SOFT_IIC_SDA_GPIO GPIO_Pin_7

#define SOFT_IIC_SCL_SET GPIO_SetBits(SOFT_IIC_PORT,SOFT_IIC_SCL_GPIO)
#define SOFT_IIC_SCL_RESET GPIO_ResetBits(SOFT_IIC_PORT,SOFT_IIC_SCL_GPIO)

#define SOFT_IIC_SDA_SET GPIO_SetBits(SOFT_IIC_PORT,SOFT_IIC_SDA_GPIO)
#define SOFT_IIC_SDA_RESET GPIO_ResetBits(SOFT_IIC_PORT,SOFT_IIC_SDA_GPIO)
#define SOFT_IIC_SDA_READ GPIO_ReadInputDataBit(SOFT_IIC_PORT,SOFT_IIC_SDA_GPIO)

/*******************输入-输出配置*******************************/
#define SOFT_IIC_SDA_OUT {GPIO_InitTypeDef GPIO_InitStructure;				\
										RCC_APB2PeriphClockCmd(SOFT_IIC_RCC,ENABLE);		\
										GPIO_InitStructure.GPIO_Pin = SOFT_IIC_SDA_GPIO;		\
										GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		\
										GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			\
										GPIO_Init(SOFT_IIC_PORT, &GPIO_InitStructure);}

#define SOFT_IIC_SDA_IN {GPIO_InitTypeDef GPIO_InitStructure;				\
										RCC_APB2PeriphClockCmd(SOFT_IIC_RCC,ENABLE);		\
										GPIO_InitStructure.GPIO_Pin = SOFT_IIC_SDA_GPIO ;			\
										GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  	\
										GPIO_Init(SOFT_IIC_PORT, &GPIO_InitStructure);}
/**************************************************/

/*********************/
/* I2C SPE mask */
#define CR1_PE_Set              ((uint16_t)0x0001)
#define CR1_PE_Reset            ((uint16_t)0xFFFE)

/* I2C START mask */
#define CR1_START_Set           ((uint16_t)0x0100)
#define CR1_START_Reset         ((uint16_t)0xFEFF)

#define CR1_POS_Set           ((uint16_t)0x0800)
#define CR1_POS_Reset         ((uint16_t)0xF7FF)

/* I2C STOP mask */
#define CR1_STOP_Set            ((uint16_t)0x0200)
#define CR1_STOP_Reset          ((uint16_t)0xFDFF)

/* I2C ACK mask */
#define CR1_ACK_Set             ((uint16_t)0x0400)
#define CR1_ACK_Reset           ((uint16_t)0xFBFF)

/* I2C ENARP mask */
#define CR1_ENARP_Set           ((uint16_t)0x0010)
#define CR1_ENARP_Reset         ((uint16_t)0xFFEF)

/* I2C NOSTRETCH mask */
#define CR1_NOSTRETCH_Set       ((uint16_t)0x0080)
#define CR1_NOSTRETCH_Reset     ((uint16_t)0xFF7F)

/* I2C registers Masks */
#define CR1_CLEAR_Mask          ((uint16_t)0xFBF5)

/* I2C DMAEN mask */
#define CR2_DMAEN_Set           ((uint16_t)0x0800)
#define CR2_DMAEN_Reset         ((uint16_t)0xF7FF)

/* I2C LAST mask */
#define CR2_LAST_Set            ((uint16_t)0x1000)
#define CR2_LAST_Reset          ((uint16_t)0xEFFF)

/* I2C FREQ mask */
#define CR2_FREQ_Reset          ((uint16_t)0xFFC0)

/* I2C ADD0 mask */
#define OAR1_ADD0_Set           ((uint16_t)0x0001)
#define OAR1_ADD0_Reset         ((uint16_t)0xFFFE)

/* I2C ENDUAL mask */
#define OAR2_ENDUAL_Set         ((uint16_t)0x0001)
#define OAR2_ENDUAL_Reset       ((uint16_t)0xFFFE)

/* I2C ADD2 mask */
#define OAR2_ADD2_Reset         ((uint16_t)0xFF01)

/* I2C F/S mask */
#define CCR_FS_Set              ((uint16_t)0x8000)

/* I2C CCR mask */
#define CCR_CCR_Set             ((uint16_t)0x0FFF)

/* I2C FLAG mask */
#define FLAG_Mask               ((uint32_t)0x00FFFFFF)

/* I2C Interrupt Enable mask */
#define ITEN_Mask               ((uint32_t)0x07000000)


#define I2C_IT_BUF                      ((uint16_t)0x0400)
#define I2C_IT_EVT                      ((uint16_t)0x0200)
#define I2C_IT_ERR                      ((uint16_t)0x0100)

#define I2C_DIRECTION_TX 0
#define I2C_DIRECTION_RX 1

#define OwnAddress1 0x28
#define OwnAddress2 0x30


#define I2C1_DMA_CHANNEL_TX           DMA1_Channel6
#define I2C1_DMA_CHANNEL_RX           DMA1_Channel7

#define I2C2_DMA_CHANNEL_TX           DMA1_Channel4
#define I2C2_DMA_CHANNEL_RX           DMA1_Channel5

#define I2C1_DR_Address              0x40005410
#define I2C2_DR_Address              0x40005810

typedef enum
{
    Polling = 0x00,
    Interrupt = 0x01,
    DMA = 0x02
} I2C_ProgrammingModel;

/*********************/
typedef enum {Error = 0, Success = !Error } Status;
/*********************/

/********配置参数*************/
#define IIC_CONFIG_1	\
{						\
	I2C_Mode_I2C,		\
	I2C_DutyCycle_2,	\
	0x0A,				\
	I2C_Ack_Enable,		\
	I2C_AcknowledgedAddress_7bit,\
	100000						\
}
#define IIC_CONFIG_2	\
{						\
	I2C_Mode_I2C,		\
	I2C_DutyCycle_2,	\
	0x0A,				\
	I2C_Ack_Enable,		\
	I2C_AcknowledgedAddress_7bit,\
	100000						\
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
Status I2C_Read(I2C_TypeDef* I2Cx, uint8_t *buf,uint32_t nbyte, uint8_t SlaveAddress,uint8_t ReadAddr);
Status I2C_Write(I2C_TypeDef* I2Cx, uint8_t *buf, uint32_t nbyte,uint8_t SlaveAddress,uint8_t WriteAddr);
Status I2C_Master_BufferRead(I2C_TypeDef* I2Cx, uint8_t* pBuffer,  uint32_t NumByteToRead, I2C_ProgrammingModel Mode, uint8_t SlaveAddress);
Status I2C_Master_BufferWrite(I2C_TypeDef* I2Cx, uint8_t* pBuffer,  uint32_t NumByteToWrite, I2C_ProgrammingModel Mode, uint8_t SlaveAddress );
void IIC_Start(void);
void IIC_Stop(void);
u8 IIC_Wait_Ack(void);
void IIC_Ack(void);
void IIC_NAck(void);
void IIC_Send_Byte(unsigned char txd);
u8 IIC_Read_Byte(unsigned char ack);

void IIC_Init(void);

/*********************/
#ifdef __cplusplus
}
#endif

#endif
