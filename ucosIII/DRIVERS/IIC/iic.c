/*
 * File      : IIC.C
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
 #include "iic.h"
 #include "usart.h"
 #include "delay.h"
 
 __IO uint32_t Timeout = 0;
//时间溢出判断
#define Timed(x) Timeout = 0xFFFF; while (x) { if (Timeout-- == 0) goto errReturn;}

/* 
 *  See AN2824 STM32F10xxx I2C optimized examples
 *
 *  This code implements polling based solution
 *
 */
	/* Private typedef -----------------------------------------------------------*/
	/* Private define ------------------------------------------------------------*/
	/* Private macro -------------------------------------------------------------*/
	/* Private variables ---------------------------------------------------------*/
	DMA_InitTypeDef  I2CDMA_InitStructure;
	__IO uint32_t I2CDirection = I2C_DIRECTION_TX;
	__IO uint32_t NumbOfBytes1;
	__IO uint32_t NumbOfBytes2;
	__IO uint8_t Address;

/**
 *  Names of events used in stdperipheral library
 *
 *      I2C_EVENT_MASTER_MODE_SELECT                          : EV5
 *      I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED            : EV6     
 *      I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED               : EV6
 *      I2C_EVENT_MASTER_BYTE_RECEIVED                        : EV7
 *      I2C_EVENT_MASTER_BYTE_TRANSMITTING                    : EV8
 *      I2C_EVENT_MASTER_BYTE_TRANSMITTED                     : EV8_2
 *     
 **/



/*
 *  Read process is documented in RM008
 *
 *   There are three cases  -- read  1 byte  AN2824 Figure 2 
 *                             read  2 bytes AN2824 Figure 2
 *                             read >2 bytes AN2824 Figure 1
 */  
 /********************************************************************
***函数名称: I2C_Read
***函数说明:IIC读取
***输入参数:I2Cx :IIC标号 buf:数据缓存  nbyte:读取缓存个数  SlaveAddress:从机地址
***输出参数:状态 Success 成功 Error:失败
***
********************************************************************/
Status I2C_Read(I2C_TypeDef* I2Cx, uint8_t *buf,uint32_t nbyte, uint8_t SlaveAddress,uint8_t ReadAddr)
{
  if (!nbyte)
    return Success;
  // Wait for idle I2C interface
  Timed(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
  I2C_AcknowledgeConfig(I2Cx, ENABLE);
  I2C_GenerateSTART(I2Cx, ENABLE);
  Timed(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
  // Send Address
  I2C_Send7bitAddress(I2Cx, SlaveAddress, I2C_Direction_Transmitter);
  // EV6
  Timed(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
  I2C_Cmd(I2Cx, ENABLE);
  I2C_SendData(I2Cx,ReadAddr);
  Timed(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  I2C_GenerateSTART(I2Cx, ENABLE);
  Timed(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
  // Wait for stop
 // Timed(I2C_GetFlagStatus(I2Cx, I2C_FLAG_STOPF));
  I2C_Send7bitAddress(I2Cx, SlaveAddress, I2C_Direction_Receiver);
  Timed(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
  while(nbyte)
  {
		Timed(I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED));
		*buf = I2C_ReceiveData(I2Cx);
		 buf++;
		 nbyte--;
  }
  I2C_AcknowledgeConfig(I2Cx, DISABLE);
  I2C_GenerateSTOP(I2Cx, ENABLE);
  //I2C_AcknowledgeConfig(I2Cx, ENABLE);
  return Success;
	errReturn:
  // Any cleanup here
  return Error;
}

/*
 * Read buffer of bytes -- AN2824 Figure 3
 */
/********************************************************************
***函数名称: I2C_Write
***函数说明:IIC写
***输入参数:I2Cx :IIC标号 buf:数据缓存   SlaveAddress:从机地址 WriteAddr:读地址  nbyte:个数
***输出参数:状态 Success 成功 Error:失败
***
********************************************************************/
Status I2C_Write(I2C_TypeDef* I2Cx, uint8_t *buf, uint32_t nbyte,uint8_t SlaveAddress,uint8_t WriteAddr)
{
  //  uint32_t IIC_Timeout = 0;
    /* Enable Error IT (used in all modes: DMA, Polling and Interrupts */
    //    I2Cx->CR2 |= I2C_IT_ERR;
	Timed(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
	I2C_AcknowledgeConfig(I2Cx,ENABLE);
    I2C_GenerateSTART(I2Cx, ENABLE);
    Timed(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
	I2C_Send7bitAddress(I2Cx, SlaveAddress, I2C_Direction_Transmitter);
	Timed(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	I2C_SendData(I2Cx, WriteAddr);
	Timed(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	while(nbyte)
	{
		I2C_SendData(I2Cx, *buf);
		Timed(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
		buf++;
		nbyte--;
	}
	I2C_GenerateSTOP(I2Cx, ENABLE);
    return Success;
	errReturn:
  // Any cleanup here
  	return Error;
}
/**
  * @brief  Initializes DMA channel used by the I2C Write/read routines.
  * @param  None.
  * @retval None.
  */
void I2C_DMAConfig(I2C_TypeDef* I2Cx, uint8_t* pBuffer, uint32_t BufferSize, uint32_t Direction)
{
    /* Initialize the DMA with the new parameters */
    if (Direction == I2C_DIRECTION_TX)
    {
        /* Configure the DMA Tx Channel with the buffer address and the buffer size */
        I2CDMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)pBuffer;
        I2CDMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
        I2CDMA_InitStructure.DMA_BufferSize = (uint32_t)BufferSize;

        if (I2Cx == I2C1)
        {
            I2CDMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)I2C1_DR_Address;
            DMA_Cmd(I2C1_DMA_CHANNEL_TX, DISABLE);
            DMA_Init(I2C1_DMA_CHANNEL_TX, &I2CDMA_InitStructure);
            DMA_Cmd(I2C1_DMA_CHANNEL_TX, ENABLE);
        }
        else
        {
            I2CDMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)I2C2_DR_Address;
            DMA_Cmd(I2C2_DMA_CHANNEL_TX, DISABLE);
            DMA_Init(I2C2_DMA_CHANNEL_TX, &I2CDMA_InitStructure);
            DMA_Cmd(I2C2_DMA_CHANNEL_TX, ENABLE);
        }
    }
    else /* Reception */
    {
        /* Configure the DMA Rx Channel with the buffer address and the buffer size */
        I2CDMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)pBuffer;
        I2CDMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
        I2CDMA_InitStructure.DMA_BufferSize = (uint32_t)BufferSize;
        if (I2Cx == I2C1)
        {

            I2CDMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)I2C1_DR_Address;
            DMA_Cmd(I2C1_DMA_CHANNEL_RX, DISABLE);
            DMA_Init(I2C1_DMA_CHANNEL_RX, &I2CDMA_InitStructure);
            DMA_Cmd(I2C1_DMA_CHANNEL_RX, ENABLE);
        }

        else
        {
            I2CDMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)I2C2_DR_Address;
            DMA_Cmd(I2C2_DMA_CHANNEL_RX, DISABLE);
            DMA_Init(I2C2_DMA_CHANNEL_RX, &I2CDMA_InitStructure);
            DMA_Cmd(I2C2_DMA_CHANNEL_RX, ENABLE);
        }

    }
}

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Reads buffer of bytes  from the slave.
  * @param pBuffer: Buffer of bytes to be read from the slave.
  * @param NumByteToRead: Number of bytes to be read by the Master.
  * @param Mode: Polling or DMA or Interrupt having the highest priority in the application.
  * @param SlaveAddress: The address of the slave to be addressed by the Master.
  * @retval : None.
  */
Status I2C_Master_BufferRead(I2C_TypeDef* I2Cx, uint8_t* pBuffer,  uint32_t NumByteToRead, I2C_ProgrammingModel Mode, uint8_t SlaveAddress)

{
    __IO uint32_t temp = 0;
    __IO uint32_t Timeout = 0;

    /* Enable I2C errors interrupts (used in all modes: Polling, DMA and Interrupts */
    I2Cx->CR2 |= I2C_IT_ERR;

    if (Mode == DMA) /* I2Cx Master Reception using DMA */
    {
        /* Configure I2Cx DMA channel */
        I2C_DMAConfig(I2Cx, pBuffer, NumByteToRead, I2C_DIRECTION_RX);
        /* Set Last bit to have a NACK on the last received byte */
        I2Cx->CR2 |= CR2_LAST_Set;
        /* Enable I2C DMA requests */
        I2Cx->CR2 |= CR2_DMAEN_Set;
        Timeout = 0xFFFF;
        /* Send START condition */
        I2Cx->CR1 |= CR1_START_Set;
        /* Wait until SB flag is set: EV5  */
        while ((I2Cx->SR1&0x0001) != 0x0001)
        {
            if (Timeout-- == 0)
                return Error;
        }
        Timeout = 0xFFFF;
        /* Send slave address */
        /* Set the address bit0 for read */
        SlaveAddress |= OAR1_ADD0_Set;
        Address = SlaveAddress;
        /* Send the slave address */
        I2Cx->DR = Address;
        /* Wait until ADDR is set: EV6 */
        while ((I2Cx->SR1&0x0002) != 0x0002)
        {
            if (Timeout-- == 0)
                return Error;
        }
        /* Clear ADDR flag by reading SR2 register */
        temp = I2Cx->SR2;
        if (I2Cx == I2C1)
        {
            /* Wait until DMA end of transfer */
            while (!DMA_GetFlagStatus(DMA1_FLAG_TC7));
            /* Disable DMA Channel */
            DMA_Cmd(I2C1_DMA_CHANNEL_RX, DISABLE);
            /* Clear the DMA Transfer Complete flag */
            DMA_ClearFlag(DMA1_FLAG_TC7);

        }

        else /* I2Cx = I2C2*/
        {
            /* Wait until DMA end of transfer */
            while (!DMA_GetFlagStatus(DMA1_FLAG_TC5));
            /* Disable DMA Channel */
            DMA_Cmd(I2C2_DMA_CHANNEL_RX, DISABLE);
            /* Clear the DMA Transfer Complete flag */
            DMA_ClearFlag(DMA1_FLAG_TC5);
        }
        /* Program the STOP */
        I2Cx->CR1 |= CR1_STOP_Set;
        /* Make sure that the STOP bit is cleared by Hardware before CR1 write access */
        while ((I2Cx->CR1&0x200) == 0x200);
    }

    else if (Mode == Polling) /* I2Cx Master Reception using Polling */
    {


        if (NumByteToRead == 1)
        {
            Timeout = 0xFFFF;
            /* Send START condition */
            I2Cx->CR1 |= CR1_START_Set;
            /* Wait until SB flag is set: EV5  */
            while ((I2Cx->SR1&0x0001) != 0x0001)
            {
                if (Timeout-- == 0)
                    return Error;
            }
            /* Send slave address */
            /* Reset the address bit0 for read */
            SlaveAddress |= OAR1_ADD0_Set;
            Address = SlaveAddress;
            /* Send the slave address */
            I2Cx->DR = Address;
            /* Wait until ADDR is set: EV6_3, then program ACK = 0, clear ADDR
            and program the STOP just after ADDR is cleared. The EV6_3 
            software sequence must complete before the current byte end of transfer.*/
            /* Wait until ADDR is set */
            Timeout = 0xFFFF;
            while ((I2Cx->SR1&0x0002) != 0x0002)
            {
                if (Timeout-- == 0)
                    return Error;
            }
            /* Clear ACK bit */
            I2Cx->CR1 &= CR1_ACK_Reset;
            /* Disable all active IRQs around ADDR clearing and STOP programming because the EV6_3
            software sequence must complete before the current byte end of transfer */
            __disable_irq();
            /* Clear ADDR flag */
            temp = I2Cx->SR2;
            /* Program the STOP */
            I2Cx->CR1 |= CR1_STOP_Set;
            /* Re-enable IRQs */
            __enable_irq();
            /* Wait until a data is received in DR register (RXNE = 1) EV7 */
            while ((I2Cx->SR1 & 0x00040) != 0x000040);
            /* Read the data */
            *pBuffer = I2Cx->DR;
            /* Make sure that the STOP bit is cleared by Hardware before CR1 write access */
            while ((I2Cx->CR1&0x200) == 0x200);
            /* Enable Acknowledgement to be ready for another reception */
            I2Cx->CR1 |= CR1_ACK_Set;

        }

        else if (NumByteToRead == 2)
        {
            /* Set POS bit */
            I2Cx->CR1 |= CR1_POS_Set;
            Timeout = 0xFFFF;
            /* Send START condition */
            I2Cx->CR1 |= CR1_START_Set;
            /* Wait until SB flag is set: EV5 */
            while ((I2Cx->SR1&0x0001) != 0x0001)
            {
                if (Timeout-- == 0)
                    return Error;
            }
            Timeout = 0xFFFF;
            /* Send slave address */
            /* Set the address bit0 for read */
            SlaveAddress |= OAR1_ADD0_Set;
            Address = SlaveAddress;
            /* Send the slave address */
            I2Cx->DR = Address;
            /* Wait until ADDR is set: EV6 */
            while ((I2Cx->SR1&0x0002) != 0x0002)
            {
                if (Timeout-- == 0)
                    return Error;
            }
            /* EV6_1: The acknowledge disable should be done just after EV6,
            that is after ADDR is cleared, so disable all active IRQs around ADDR clearing and 
            ACK clearing */
            __disable_irq();
            /* Clear ADDR by reading SR2 register  */
            temp = I2Cx->SR2;
            /* Clear ACK */
            I2Cx->CR1 &= CR1_ACK_Reset;
            /*Re-enable IRQs */
            __enable_irq();
            /* Wait until BTF is set */
            while ((I2Cx->SR1 & 0x00004) != 0x000004);
            /* Disable IRQs around STOP programming and data reading because of the limitation ?*/
            __disable_irq();
            /* Program the STOP */
            I2C_GenerateSTOP(I2Cx, ENABLE);
            /* Read first data */
            *pBuffer = I2Cx->DR;
            /* Re-enable IRQs */
            __enable_irq();
            /**/
            pBuffer++;
            /* Read second data */
            *pBuffer = I2Cx->DR;
            /* Make sure that the STOP bit is cleared by Hardware before CR1 write access */
            while ((I2Cx->CR1&0x200) == 0x200);
            /* Enable Acknowledgement to be ready for another reception */
            I2Cx->CR1  |= CR1_ACK_Set;
            /* Clear POS bit */
            I2Cx->CR1  &= CR1_POS_Reset;

        }

        else

        {

            Timeout = 0xFFFF;
            /* Send START condition */
            I2Cx->CR1 |= CR1_START_Set;
            /* Wait until SB flag is set: EV5 */
            while ((I2Cx->SR1&0x0001) != 0x0001)
            {
                if (Timeout-- == 0)
                    return Error;
            }
            Timeout = 0xFFFF;
            /* Send slave address */
            /* Reset the address bit0 for write */
            SlaveAddress |= OAR1_ADD0_Set;;
            Address = SlaveAddress;
            /* Send the slave address */
            I2Cx->DR = Address;
            /* Wait until ADDR is set: EV6 */
            while ((I2Cx->SR1&0x0002) != 0x0002)
            {
                if (Timeout-- == 0)
                    return Error;
            }
            /* Clear ADDR by reading SR2 status register */
            temp = I2Cx->SR2;
            /* While there is data to be read */
            while (NumByteToRead)
            {
                /* Receive bytes from first byte until byte N-3 */
                if (NumByteToRead != 3)
                {
                    /* Poll on BTF to receive data because in polling mode we can not guarantee the
                    EV7 software sequence is managed before the current byte transfer completes */
                    while ((I2Cx->SR1 & 0x00004) != 0x000004);
                    /* Read data */
                    *pBuffer = I2Cx->DR;
                    /* */
                    pBuffer++;
                    /* Decrement the read bytes counter */
                    NumByteToRead--;
                }

                /* it remains to read three data: data N-2, data N-1, Data N */
                if (NumByteToRead == 3)
                {

                    /* Wait until BTF is set: Data N-2 in DR and data N -1 in shift register */
                    while ((I2Cx->SR1 & 0x00004) != 0x000004);
                    /* Clear ACK */
                    I2Cx->CR1 &= CR1_ACK_Reset;

                    /* Disable IRQs around data reading and STOP programming because of the
                    limitation ? */
                    __disable_irq();
                    /* Read Data N-2 */
                    *pBuffer = I2Cx->DR;
                    /* Increment */
                    pBuffer++;
                    /* Program the STOP */
                    I2Cx->CR1 |= CR1_STOP_Set;
                    /* Read DataN-1 */
                    *pBuffer = I2Cx->DR;
                    /* Re-enable IRQs */
                    __enable_irq();
                    /* Increment */
                    pBuffer++;
                    /* Wait until RXNE is set (DR contains the last data) */
                    while ((I2Cx->SR1 & 0x00040) != 0x000040);
                    /* Read DataN */
                    *pBuffer = I2Cx->DR;
                    /* Reset the number of bytes to be read by master */
                    NumByteToRead = 0;

                }
            }
            /* Make sure that the STOP bit is cleared by Hardware before CR1 write access */
            while ((I2Cx->CR1&0x200) == 0x200);
            /* Enable Acknowledgement to be ready for another reception */
            I2Cx->CR1 |= CR1_ACK_Set;

        }

    }

    else /* I2Cx Master Reception using Interrupts with highest priority in an application */
    {
        /* Enable EVT IT*/
        I2Cx->CR2 |= I2C_IT_EVT;
        /* Enable BUF IT */
        I2Cx->CR2 |= I2C_IT_BUF;
        /* Set the I2C direction to reception */
        I2CDirection = I2C_DIRECTION_RX;
        SlaveAddress |= OAR1_ADD0_Set;
        Address = SlaveAddress;
        if (I2Cx == I2C1)    NumbOfBytes1 = NumByteToRead;
        else NumbOfBytes2 = NumByteToRead;
        /* Send START condition */
        I2Cx->CR1 |= CR1_START_Set;
        /* Wait until the START condition is generated on the bus: START bit is cleared by hardware */
        while ((I2Cx->CR1&0x100) == 0x100);
        /* Wait until BUSY flag is reset (until a STOP is generated) */
        while ((I2Cx->SR2 &0x0002) == 0x0002);
        /* Enable Acknowledgement to be ready for another reception */
        I2Cx->CR1 |= CR1_ACK_Set;
    }

    return Success;
}



/**
  * @brief  Writes buffer of bytes.
  * @param pBuffer: Buffer of bytes to be sent to the slave.
  * @param NumByteToWrite: Number of bytes to be sent by the Master.
  * @param Mode: Polling or DMA or Interrupt having the highest priority in the application.
  * @param SlaveAddress: The address of the slave to be addressed by the Master.
  * @retval : None.
  */
Status I2C_Master_BufferWrite(I2C_TypeDef* I2Cx, uint8_t* pBuffer,  uint32_t NumByteToWrite, I2C_ProgrammingModel Mode, uint8_t SlaveAddress )

{

    __IO uint32_t temp = 0;
    __IO uint32_t Timeout = 0;

    /* Enable Error IT (used in all modes: DMA, Polling and Interrupts */
    I2Cx->CR2 |= I2C_IT_ERR;
    if (Mode == DMA)  /* I2Cx Master Transmission using DMA */
    {
        Timeout = 0xFFFF;
        /* Configure the DMA channel for I2Cx transmission */
        I2C_DMAConfig (I2Cx, pBuffer, NumByteToWrite, I2C_DIRECTION_TX);
        /* Enable the I2Cx DMA requests */
        I2Cx->CR2 |= CR2_DMAEN_Set;
        /* Send START condition */
        I2Cx->CR1 |= CR1_START_Set;
        /* Wait until SB flag is set: EV5 */
        while ((I2Cx->SR1&0x0001) != 0x0001)
        {
            if (Timeout-- == 0)
                return Error;
        }
        Timeout = 0xFFFF;
        /* Send slave address */
        /* Reset the address bit0 for write */
        SlaveAddress &= OAR1_ADD0_Reset;
        Address = SlaveAddress;
        /* Send the slave address */
        I2Cx->DR = Address;
        /* Wait until ADDR is set: EV6 */
        while ((I2Cx->SR1&0x0002) != 0x0002)
        {
            if (Timeout-- == 0)
                return Error;
        }

        /* Clear ADDR flag by reading SR2 register */
        temp = I2Cx->SR2;
        if (I2Cx == I2C1)
        {
            /* Wait until DMA end of transfer */
            while (!DMA_GetFlagStatus(DMA1_FLAG_TC6));
            /* Disable the DMA1 Channel 6 */
            DMA_Cmd(I2C1_DMA_CHANNEL_TX, DISABLE);
            /* Clear the DMA Transfer complete flag */
            DMA_ClearFlag(DMA1_FLAG_TC6);
        }
        else  /* I2Cx = I2C2 */
        {
            /* Wait until DMA end of transfer */
            while (!DMA_GetFlagStatus(DMA1_FLAG_TC4));
            /* Disable the DMA1 Channel 4 */
            DMA_Cmd(I2C2_DMA_CHANNEL_TX, DISABLE);
            /* Clear the DMA Transfer complete flag */
            DMA_ClearFlag(DMA1_FLAG_TC4);
        }

        /* EV8_2: Wait until BTF is set before programming the STOP */
        while ((I2Cx->SR1 & 0x00004) != 0x000004);
        /* Program the STOP */
        I2Cx->CR1 |= CR1_STOP_Set;
        /* Make sure that the STOP bit is cleared by Hardware */
        while ((I2Cx->CR1&0x200) == 0x200);

    }
    else if (Mode == Polling) /* I2Cx Master Transmission using Polling */
    {

        Timeout = 0xFFFF;
        /* Send START condition */
        I2Cx->CR1 |= CR1_START_Set;
        /* Wait until SB flag is set: EV5 */
        while ((I2Cx->SR1&0x0001) != 0x0001)
        {
            if (Timeout-- == 0)
                return Error;
        }

        /* Send slave address */
        /* Reset the address bit0 for write*/
        SlaveAddress &= OAR1_ADD0_Reset;
        Address = SlaveAddress;
        /* Send the slave address */
        I2Cx->DR = Address;
        Timeout = 0xFFFF;
        /* Wait until ADDR is set: EV6 */
        while ((I2Cx->SR1 &0x0002) != 0x0002)
        {
            if (Timeout-- == 0)
                return Error;
        }

        /* Clear ADDR flag by reading SR2 register */
        temp = I2Cx->SR2;
        /* Write the first data in DR register (EV8_1) */
        I2Cx->DR = *pBuffer;
        /* Increment */
        pBuffer++;
        /* Decrement the number of bytes to be written */
        NumByteToWrite--;
        /* While there is data to be written */
        while (NumByteToWrite--)
        {
            /* Poll on BTF to receive data because in polling mode we can not guarantee the
              EV8 software sequence is managed before the current byte transfer completes */
            while ((I2Cx->SR1 & 0x00004) != 0x000004);
            /* Send the current byte */
            I2Cx->DR = *pBuffer;
            /* Point to the next byte to be written */
            pBuffer++;
        }
        /* EV8_2: Wait until BTF is set before programming the STOP */
        while ((I2Cx->SR1 & 0x00004) != 0x000004);
        /* Send STOP condition */
        I2Cx->CR1 |= CR1_STOP_Set;
        /* Make sure that the STOP bit is cleared by Hardware */
        while ((I2Cx->CR1&0x200) == 0x200);

    }

    else /* I2Cx Master Transmission using Interrupt with highest priority in the application */

    {
        /* Enable EVT IT*/
        I2Cx->CR2 |= I2C_IT_EVT;
        /* Enable BUF IT */
        I2Cx->CR2 |= I2C_IT_BUF;
        /* Set the I2C direction to Transmission */
        I2CDirection = I2C_DIRECTION_TX;
        SlaveAddress &= OAR1_ADD0_Reset;
        Address = SlaveAddress;
        if (I2Cx == I2C1)    NumbOfBytes1 = NumByteToWrite;
        else NumbOfBytes2 = NumByteToWrite;
        /* Send START condition */
        I2Cx->CR1 |= CR1_START_Set;
        /* Wait until the START condition is generated on the bus: the START bit is cleared by hardware */
        while ((I2Cx->CR1&0x100) == 0x100);
        /* Wait until BUSY flag is reset: a STOP has been generated on the bus signaling the end
        of transmission */
        while ((I2Cx->SR2 &0x0002) == 0x0002);
    }

    return Success;

}


/**
  * @brief Prepares the I2Cx slave for transmission.
  * @param I2Cx: I2C1 or I2C2.
  * @param Mode: DMA or Interrupt having the highest priority in the application.
  * @retval : None.
  */

void I2C_Slave_BufferReadWrite(I2C_TypeDef* I2Cx,I2C_ProgrammingModel Mode)

{
    /* Enable Event IT needed for ADDR and STOPF events ITs */
    I2Cx->CR2 |= I2C_IT_EVT ;
    /* Enable Error IT */
    I2Cx->CR2 |= I2C_IT_ERR;

    if (Mode == DMA)  /* I2Cx Slave Transmission using DMA */
    {
        /* Enable I2Cx DMA requests */
        I2Cx->CR2 |= CR2_DMAEN_Set;
    }

    else  /* I2Cx Slave Transmission using Interrupt with highest priority in the application */
    {
        /* Enable Buffer IT (TXE and RXNE ITs) */
        I2Cx->CR2 |= I2C_IT_BUF;

    }

}

/********************************************************************
***函数名称: IIC_Io_Init
***函数说明:IIC端口初始化
***输入参数:无
***输出参数:无
***
********************************************************************/
static void IIC_Io_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
    I2C_InitTypeDef  I2C_InitStructure;
	#ifdef IIC1_PERPH
		struct iic_config config1=IIC_CONFIG_1;
	#endif
	#ifdef IIC2_PERPH
		struct iic_config config2=IIC_CONFIG_2;
	#endif
	#ifdef IIC1_PERPH
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

	I2C_InitStructure.I2C_Mode = config1.iic_mode;
	I2C_InitStructure.I2C_DutyCycle = config1.iic_duty;
    I2C_InitStructure.I2C_OwnAddress1 = config1.iic_addr;
   	I2C_InitStructure.I2C_Ack = config1.iic_ack;
    I2C_InitStructure.I2C_AcknowledgedAddress = config1.iic_ackaddr;
    I2C_InitStructure.I2C_ClockSpeed = config1.iic_clock;
    I2C_Init(I2C1, &I2C_InitStructure);
    I2C_Cmd(I2C1, ENABLE); 
		#ifdef DEBUG_MEG
			__DBG__("IIC1功能打开!\r\n");
		#endif
	#endif
	#ifdef IIC2_PERPH
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	 
	I2C_InitStructure.I2C_Mode = config2.iic_mode;
    I2C_InitStructure.I2C_DutyCycle = config2.iic_duty;
    I2C_InitStructure.I2C_OwnAddress1 = config2.iic_addr;
   	I2C_InitStructure.I2C_Ack = config2.iic_ack;
    I2C_InitStructure.I2C_AcknowledgedAddress = config2.iic_ackaddr;
    I2C_InitStructure.I2C_ClockSpeed = config2.iic_clock;
    I2C_Init(I2C2, &I2C_InitStructure);
    I2C_Cmd(I2C2, ENABLE);
		#ifdef DEBUG_MEG
			__DBG__("IIC2功能打开!\r\n");
		#endif
	#endif
}
/********************************************************************
***函数名称: IIC_Dma_Init
***函数说明:IICdma
***输入参数:无
***输出参数:无
***
********************************************************************/
static void IIC_Dma_Init()
{
	#ifdef IIC1_DMA_TR
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
		DMA_DeInit(I2C1_DMA_CHANNEL_TX);
        I2CDMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)I2C1_DR_Address;
        I2CDMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)0;   /* This parameter will be configured durig communication */
        I2CDMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;    /* This parameter will be configured durig communication */
        I2CDMA_InitStructure.DMA_BufferSize = 0xFFFF;            /* This parameter will be configured durig communication */
        I2CDMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
        I2CDMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
        I2CDMA_InitStructure.DMA_PeripheralDataSize = DMA_MemoryDataSize_Byte;
        I2CDMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
        I2CDMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
        I2CDMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
        I2CDMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
        DMA_Init(I2C1_DMA_CHANNEL_TX, &I2CDMA_InitStructure);

        /* I2C1 RX DMA Channel configuration */
        DMA_DeInit(I2C1_DMA_CHANNEL_RX);
        DMA_Init(I2C1_DMA_CHANNEL_RX, &I2CDMA_InitStructure);
	#endif
	#ifdef IIC2_DMA_TR
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
		 /* I2C2 TX DMA Channel configuration */
        DMA_DeInit(I2C2_DMA_CHANNEL_TX);
        I2CDMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)I2C2_DR_Address;
        I2CDMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)0;   /* This parameter will be configured durig communication */
        I2CDMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;    /* This parameter will be configured durig communication */
        I2CDMA_InitStructure.DMA_BufferSize = 0xFFFF;            /* This parameter will be configured durig communication */
        I2CDMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
        I2CDMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
        I2CDMA_InitStructure.DMA_PeripheralDataSize = DMA_MemoryDataSize_Byte;
        I2CDMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
        I2CDMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
        I2CDMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
        I2CDMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
        DMA_Init(I2C2_DMA_CHANNEL_TX, &I2CDMA_InitStructure);

        /* I2C2 RX DMA Channel configuration */
        DMA_DeInit(I2C2_DMA_CHANNEL_RX);
        DMA_Init(I2C2_DMA_CHANNEL_RX, &I2CDMA_InitStructure);
	#endif
}
/********************************************************************
***函数名称: IIC_Start
***函数说明:IIC起始信号
***输入参数:无
***输出参数:无
***
********************************************************************/
void IIC_Start(void)
{
	SOFT_IIC_SDA_OUT;     //sda线输出
	SOFT_IIC_SDA_SET;	  	  
	SOFT_IIC_SCL_SET;
	delay_us(4);
 	SOFT_IIC_SDA_RESET;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	SOFT_IIC_SCL_RESET;//钳住I2C总线，准备发送或接收数据 
}	 
/********************************************************************
***函数名称: IIC_Stop
***函数说明:产生IIC停止信号
***输入参数:无
***输出参数:无
***
********************************************************************/
void IIC_Stop(void)
{
	SOFT_IIC_SDA_OUT;//sda线输出
	SOFT_IIC_SCL_RESET;
	SOFT_IIC_SDA_RESET;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	SOFT_IIC_SCL_SET; 
	SOFT_IIC_SDA_SET;//发送I2C总线结束信号
	delay_us(4);							   	
}
/********************************************************************
***函数名称: IIC_Stop
***函数说明:等待应答信号到来
***输入参数:无
***输出参数:1，接收应答失败  0，接收应答成功
***
********************************************************************/
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SOFT_IIC_SDA_IN;      //SDA设置为输入  
	SOFT_IIC_SDA_SET;delay_us(1);	   
	SOFT_IIC_SCL_SET;delay_us(1);	 
	while(SOFT_IIC_SDA_READ)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	SOFT_IIC_SCL_RESET;//时钟输出0 	   
	return 0;  
}
/********************************************************************
***函数名称: IIC_Ack
***函数说明:产生ACK应答
***输入参数:无
***输出参数:无
***
********************************************************************/
void IIC_Ack(void)
{
	SOFT_IIC_SCL_RESET;
	SOFT_IIC_SDA_OUT;
	SOFT_IIC_SDA_RESET;
	delay_us(2);
	SOFT_IIC_SCL_SET;
	delay_us(2);
	SOFT_IIC_SCL_RESET;
}
/********************************************************************
***函数名称: IIC_NAck
***函数说明:不产生ACK应答		
***输入参数:无
***输出参数:无
***
********************************************************************/  
void IIC_NAck(void)
{
	SOFT_IIC_SCL_RESET;
	SOFT_IIC_SDA_OUT;
	SOFT_IIC_SDA_SET;
	delay_us(2);
	SOFT_IIC_SCL_SET;
	delay_us(2);
	SOFT_IIC_SCL_RESET;
}	
/********************************************************************
***函数名称: IIC_Send_Byte
***函数说明:IIC发送一个字节
***输入参数:无		
***输出参数:无
***
********************************************************************/ 		  
void IIC_Send_Byte(unsigned char txd)
{                        
  u8 t;   
	SOFT_IIC_SDA_OUT; 	    
	SOFT_IIC_SCL_RESET;//拉低时钟开始数据传输
	for(t=0;t<8;t++)
	{              
			//IIC_SDA=(txd&0x80)>>7;
		if((txd&0x80)>>7)
			SOFT_IIC_SDA_SET;
		else
			SOFT_IIC_SDA_RESET;
		txd<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		SOFT_IIC_SCL_SET;
		delay_us(2); 
		SOFT_IIC_SCL_RESET;	
		delay_us(2);
	}	 
} 	
/********************************************************************
***函数名称: IIC_Send_Byte
***函数说明:IIC读1个字节
***输入参数:ack=1时，发送ACK，ack=0，发送nACK 	
***输出参数:无
***
********************************************************************/
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SOFT_IIC_SDA_IN;//SDA设置为输入
  for(i=0;i<8;i++ )
	{
		SOFT_IIC_SCL_RESET; 
		delay_us(2);
		SOFT_IIC_SCL_SET;
		receive<<=1;
		if(SOFT_IIC_SDA_READ)receive++;   
		delay_us(1); 
 	}					 
  	if (!ack)
      IIC_NAck();//发送nACK
  	else
      IIC_Ack(); //发送ACK   
 	 return receive;
}
void Soft_Iic_Init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(SOFT_IIC_RCC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = SOFT_IIC_SCL_GPIO | SOFT_IIC_SDA_GPIO;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(SOFT_IIC_PORT, &GPIO_InitStructure);
}
/********************************************************************
***函数名称: IIC_Init
***函数说明:IIC初始化
***输入参数:无
***输出参数:无
***
********************************************************************/
void IIC_Init(void)
{
	#ifdef SOFT_IIC
		Soft_Iic_Init();
	#else
		IIC_Io_Init();
		IIC_Dma_Init();
	#endif
}
