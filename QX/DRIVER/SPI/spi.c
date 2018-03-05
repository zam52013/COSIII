/*
 * File      : spi.c
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
 #include "spi.h"
 #include "usart.h"
 static void spi_rcc_init(void)
 {
	#ifdef SPI1_PERIPH
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 | RCC_APB2Periph_GPIOA,ENABLE);
	#endif
	#ifdef SPI2_PERIPH
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);	
	#endif
	#ifdef SPI3_PERIPH
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE); 
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3,ENABLE);
	#endif
 }
 
 static void spi_io_init(void)
 {
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;
	#ifdef SPI1_PERIPH
		struct spi_config config1=SPI_CONFGI_1;
	#endif
	#ifdef SPI2_PERIPH
		struct spi_config config2=SPI_CONFGI_2;
	#endif
	#ifdef SPI3_PERIPH
		struct spi_config config3=SPI_CONFGI_3;
	#endif
	#ifdef SPI1_PERIPH
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);	

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 ;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //复用推挽输出
		GPIO_Init(GPIOA, &GPIO_InitStructure);	
		
		SPI_InitStructure.SPI_Direction = config1.spi_dir;
		SPI_InitStructure.SPI_Mode = config1.spi_mode;
		SPI_InitStructure.SPI_DataSize = config1.spi_datelenth;
		SPI_InitStructure.SPI_CPOL = config1.spi_cpol;
		SPI_InitStructure.SPI_CPHA = config1.spi_cpha;
		SPI_InitStructure.SPI_NSS = config1.spi_nss;
		SPI_InitStructure.SPI_BaudRatePrescaler = config1.spi_baud;
		SPI_InitStructure.SPI_FirstBit = config1.spi_first;
		SPI_InitStructure.SPI_CRCPolynomial = 7;//7位循环冗余计算
		SPI_Init(SPI1,&SPI_InitStructure);
		SPI_Cmd(SPI1,ENABLE);
	#endif
	#ifdef SPI2_PERIPH
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);	
		
		SPI_InitStructure.SPI_Direction = config2.spi_dir;
		SPI_InitStructure.SPI_Mode = config2.spi_mode;
		SPI_InitStructure.SPI_DataSize = config2.spi_datelenth;
		SPI_InitStructure.SPI_CPOL = config2.spi_cpol;
		SPI_InitStructure.SPI_CPHA = config2.spi_cpha;
		SPI_InitStructure.SPI_NSS = config2.spi_nss;
		SPI_InitStructure.SPI_BaudRatePrescaler = config2.spi_baud;
		SPI_InitStructure.SPI_FirstBit = config2.spi_first;
		SPI_InitStructure.SPI_CRCPolynomial = 7;//7位循环冗余计算
		SPI_Init(SPI2,&SPI_InitStructure);
		SPI_Cmd(SPI2,ENABLE);
	#endif
	#ifdef SPI3_PERIPH
		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);	
		
		SPI_InitStructure.SPI_Direction = config3.spi_dir;
		SPI_InitStructure.SPI_Mode = config3.spi_mode;
		SPI_InitStructure.SPI_DataSize = config3.spi_datelenth;
		SPI_InitStructure.SPI_CPOL = config3.spi_cpol;
		SPI_InitStructure.SPI_CPHA = config3.spi_cpha;
		SPI_InitStructure.SPI_NSS = config3.spi_nss;
		SPI_InitStructure.SPI_BaudRatePrescaler = config3.spi_baud;
		SPI_InitStructure.SPI_FirstBit = config3.spi_first;
		SPI_InitStructure.SPI_CRCPolynomial = 7;//7位循环冗余计算
		SPI_Init(SPI3,&SPI_InitStructure);
		SPI_Cmd(SPI3,ENABLE);
	#endif
	#ifdef DEBUG_PERPH
		#ifdef SPI1_PERIPH
			printf("spi1 open!\r\n");
		#endif
		#ifdef SPI2_PERIPH
			printf("spi1 open!\r\n");
		#endif
		#ifdef SPI3_PERIPH
			printf("spi1 open!\r\n");
		#endif
	#endif
 }

 void Spi_init(void)
 {
	spi_rcc_init();
	spi_io_init();
 }
 unsigned char SPI_ReadWriteByte(SPI_TypeDef* SPIx,unsigned char TxData)
{		
	unsigned char retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
	{
		retry++;
		if(retry>200)
			return 0;
	}			  
	SPI_I2S_SendData(SPIx, TxData); //通过外设SPIx发送一个数据
	retry=0;
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET) //检查指定的SPI标志位设置与否:接受缓存非空标志位
	{
		retry++;
		if(retry>200)
			return 0;
	}	  						    
	return SPI_I2S_ReceiveData(SPIx); //返回通过SPIx最近接收的数据					    
}
 void Spi_select(DEV_ID devid,SELET selected)
 {
 	if(selected)
 	{
		switch(devid)
		{
			case FM25V_ID:
				FM25V_RESET;
				MPU9250_SET;
				break;
			case MPU9250_ID:
				MPU9250_RESET;
				FM25V_SET;
				break;
			default:
				break;
		}
	}
	else
	{
		switch(devid)
		{
			case FM25V_ID:
				FM25V_SET;
				break;
			case MPU9250_ID:
				MPU9250_SET;
				break;
			default:
				break;
		}
	}
 }


 
