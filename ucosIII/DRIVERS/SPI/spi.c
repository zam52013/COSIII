/*
 * File      : spi.c
 * This file is ef comp
 * COPYRIGHT (C) 2017,
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at my addr
 *
 * Change Logs:
 * Date           Author       ZAM
 * 2017-09-28     Bernard      the first version
 */
 #include "spi.h"
 #include "usart.h"

  /*******************************************************************************
�� �� ����void spi_io_init(void)
���������� spi_rcc��ʼ��IO��
��ڲ�����	��
���ز�������
����ʱ��: 2018-1-05 by zam
********************************************************************************/
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
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_SPI1 | RCC_APB2Periph_GPIOA,ENABLE);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);	

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 ;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  
		GPIO_Init(GPIOA, &GPIO_InitStructure);	
		
		SPI_InitStructure.SPI_Direction = config1.spi_dir;
		SPI_InitStructure.SPI_Mode = config1.spi_mode;
		SPI_InitStructure.SPI_DataSize = config1.spi_datelenth;
		SPI_InitStructure.SPI_CPOL = config1.spi_cpol;
		SPI_InitStructure.SPI_CPHA = config1.spi_cpha;
		SPI_InitStructure.SPI_NSS = config1.spi_nss;
		SPI_InitStructure.SPI_BaudRatePrescaler = config1.spi_baud;
		SPI_InitStructure.SPI_FirstBit = config1.spi_first;
		SPI_InitStructure.SPI_CRCPolynomial = 7;//7λѭ���������
		SPI_Init(SPI1,&SPI_InitStructure);
		SPI_Cmd(SPI1,ENABLE);
		#ifdef DEBUG_MEG
			__DBG__("SPI1���ܴ�!\r\n");
		#endif
	#endif
	#ifdef SPI2_PERIPH
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB,ENABLE);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);	
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 ;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  
		GPIO_Init(GPIOB, &GPIO_InitStructure);	
		
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);	
		
		SPI_InitStructure.SPI_Direction = config2.spi_dir;
		SPI_InitStructure.SPI_Mode = config2.spi_mode;
		SPI_InitStructure.SPI_DataSize = config2.spi_datelenth;
		SPI_InitStructure.SPI_CPOL = config2.spi_cpol;
		SPI_InitStructure.SPI_CPHA = config2.spi_cpha;
		SPI_InitStructure.SPI_NSS = config2.spi_nss;
		SPI_InitStructure.SPI_BaudRatePrescaler = config2.spi_baud;
		SPI_InitStructure.SPI_FirstBit = config2.spi_first;
		SPI_InitStructure.SPI_CRCPolynomial = 7;//7λѭ���������
		SPI_Init(SPI2,&SPI_InitStructure);
		SPI_Cmd(SPI2,ENABLE);
		#ifdef DEBUG_MEG
			__DBG__("SPI2���ܴ�!\r\n");
		#endif
	#endif
	#ifdef SPI3_PERIPH
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE); 
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3,ENABLE);
		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_5;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);	
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 ;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		
		SPI_InitStructure.SPI_Direction = config3.spi_dir;
		SPI_InitStructure.SPI_Mode = config3.spi_mode;
		SPI_InitStructure.SPI_DataSize = config3.spi_datelenth;
		SPI_InitStructure.SPI_CPOL = config3.spi_cpol;
		SPI_InitStructure.SPI_CPHA = config3.spi_cpha;
		SPI_InitStructure.SPI_NSS = config3.spi_nss;
		SPI_InitStructure.SPI_BaudRatePrescaler = config3.spi_baud;
		SPI_InitStructure.SPI_FirstBit = config3.spi_first;
		SPI_InitStructure.SPI_CRCPolynomial = 7;//7λѭ���������
		SPI_Init(SPI3,&SPI_InitStructure);
		SPI_Cmd(SPI3,ENABLE);
		#ifdef DEBUG_MEG
			__DBG__("SPI3���ܴ�!\r\n");
		#endif
	#endif
 }
  /*******************************************************************************
�� �� ����void Spi_init(void)
���������� spi_rcc��ʼ��IO��
��ڲ�����	��
���ز�������
����ʱ��: 2018-1-05 by zam
********************************************************************************/
 void Spi_init(void)
 {
	spi_io_init();
 }
   /*******************************************************************************
�� �� ����void SPI_ReadWriteByte(void)
���������� spiд����
��ڲ�����SPIx:�豸  TxData:����
���ز�����0:ʧ��
����ʱ��: 2018-1-05 by zam
********************************************************************************/
 unsigned char SPI_ReadWriteByte(SPI_TypeDef* SPIx,unsigned char TxData)
{		
	unsigned char retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET) //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
	{
		retry++;
		if(retry>200)
			return 0;
	}			  
	SPI_I2S_SendData(SPIx, TxData); //ͨ������SPIx����һ������
	retry=0;
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET) //���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
	{
		retry++;
		if(retry>200)
			return 0;
	}	  						    
	return SPI_I2S_ReceiveData(SPIx); //����ͨ��SPIx������յ�����					    
}
  



 
