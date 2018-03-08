/*
 * File      : ms5611.h
 * This file is ef comp
 * COPYRIGHT (C) 2018,
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at my addr
 *
 *    2320780008@qq.com
 * Change Logs:
 * Date           Author       ZAM
 * 2018-03-08     Bernard      the first version
 */
 #include "ms5611.h"
 #include "spi.h"
 #include "usart.h"
 #include "delay.h"
 #include <math.h>
 /*************���PROM����****************/
 unsigned int MS5611_PROM_BUFF[7];
/*****************************************/
/*******************************************************************************
�� �� ���� MS5611_Reset(void)
������������λ5611
��ڲ�����	
���ز�����
����ʱ��: 2018-3-08 by zam
********************************************************************************/
 static void MS5611_Reset()
 {
		MS5611_RESET;
		SPI_ReadWriteByte(MS5611_SPI,CMD_MS5611_RESET);
		delay_ms(3);
		MS5611_SET;
 }
 /*******************************************************************************
�� �� ���� MS5611_Read_Reg(void)
������������ȡ�Ĵ���ֵ5611
 ��ڲ�����	reg :��ַ
���ز�����������ֵ
����ʱ��: 2018-3-08 by zam
********************************************************************************/
 unsigned char MS5611_Read_Reg(unsigned char reg)
{
		unsigned char value;
		MS5611_RESET;
		SPI_ReadWriteByte(MS5611_SPI,reg|DIR_READ);
		value=SPI_ReadWriteByte(MS5611_SPI,DIR_WRITE);
		MS5611_SET;
		return value;
}
 /*******************************************************************************
�� �� ���� MS5611_Read_PROM(void)
������������ȡ�Ĵ���ֵPROM
 ��ڲ�����
���ز�����
����ʱ��: 2018-3-08 by zam
********************************************************************************/
 static void MS5611_Read_PROM()
 {
		unsigned char i;
		unsigned char buff_h,buff_l;
	  for(i=0;i<7;i++)
		{
				buff_h=MS5611_Read_Reg(CMD_MS5611_PROM_READ+i*2);
				buff_l=MS5611_Read_Reg(CMD_MS5611_PROM_READ+i*2+1);
				MS5611_PROM_BUFF[i]=((unsigned int)buff_h<<8)+buff_l;
		}	
 }
/*******************************************************************************
�� �� ���� MS5611_ReadADCResult(void)
������������ȡ�Ĵ���ֵ
 ��ڲ�����command:��ַ
���ز�����ֵ
����ʱ��: 2018-3-08 by zam
********************************************************************************/
 u32 MS5611_ReadADCResult(u8 command)
{
		u32 adc=0;
		u8 Dx_TempH=0,
		Dx_TempM=0,
		Dx_TempL=0;

		MS5611_RESET;

		SPI_ReadWriteByte(MS5611_SPI,command|DIR_READ);			 //����ת��ָ��
		//delay_ms(20);
	
		MS5611_SET;
		MS5611_RESET;
		/*
		ע��:��Ȼ24λ����ֻ��Ҫ��3��,������ʱ��ͼ��֪,��һ�ζ�ȡ��8λ��������Ч��,����Ҫ"�ն�"һ��
		�������ܰ�������ݸ�ȥ��
		*/
		SPI_ReadWriteByte(MS5611_SPI,CMD_MS5611_ADC_READ);										//������ͷ8λ�����ֽ�
		Dx_TempH=MS5611_Read_Reg(DIR_READ);           //����  
		Dx_TempM=MS5611_Read_Reg(DIR_READ);           //����  
		Dx_TempL=MS5611_Read_Reg(DIR_READ);     		  //���� 
		MS5611_SET;
	
		adc=(((u32)Dx_TempH)<<16)+(((u32)Dx_TempM)<<8)+Dx_TempL;

		return adc;
}
/*******************************************************************************
�� �� ���� MS5611_GetTP(double *TEMP,double *PRES)
�������������õ����¶Ⱥ�ѹ��ת��Ϊ���mbar
 ��ڲ�����
���ز�����
����ʱ��: 2018-3-08 by zam
********************************************************************************/
void MS5611_GetTP(double *TEMP,double *PRES)
{
	uint32_t D1,D2;
	double dT,OFF,SENS;
	double temperatureTemp,
						pressureTemp,
						T2;
	float Aux,OFF2,SENS2;  //�¶�У��ֵ
	D1=MS5611_ReadADCResult(CMD_MS5611_D1_4096);   // ��δ������ѹ��ֵ 
	D2=MS5611_ReadADCResult(CMD_MS5611_D2_4096);   // ��δ�������¶�ֵ 
	dT=D2-MS5611_PROM_BUFF[5]*pow(2.0,8.0);
		
	OFF=MS5611_PROM_BUFF[2]*pow(2.0,16.0)+dT*MS5611_PROM_BUFF[4]/pow(2.0,7.0);     
	SENS=MS5611_PROM_BUFF[1]*pow(2.0,15.0)+dT*MS5611_PROM_BUFF[3]/pow(2.0,8.0); 
	temperatureTemp=(2000+(dT*MS5611_PROM_BUFF[6])/pow(2.0,23.0)); 
	/*�����¶Ȳ���*/
	if(temperatureTemp < 2000)// second order temperature compensation when under 20 degrees C
	{
		T2 = (dT*dT) /pow(2.0,31.0);
		Aux = (temperatureTemp-2000)*(temperatureTemp-2000);
		OFF2 = 2.5*Aux;
		SENS2 = 1.25*Aux;
		if(temperatureTemp < -1500)
		{
			Aux = (temperatureTemp+1500)*(temperatureTemp+1500);
			OFF2 = OFF2 + 7*Aux;
			SENS2 =SENS2+ 5.5*Aux;
		}
	}
	else  //(Temperature > 2000)
	{
		T2 = 0;
		OFF2 = 0;
		SENS2 = 0;
	}
	
	OFF = OFF - OFF2;
	SENS = SENS - SENS2;	
	
	*TEMP =( temperatureTemp - T2)/100;		//��λ:��

	*PRES=(D1*SENS/2097152-OFF)/32768/100;	//��λ:mbar  1mbar=100Pa
}
 /*******************************************************************************
�� �� ����void Init_MS5611_Cs(void)
����������5611Ƭѡ��ʼ��
��ڲ�����	��
���ز�������
����ʱ��: 2018-1-05 by zam
********************************************************************************/
 static void Init_MS5611_Cs(void)
 {
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(MS5611_RCC,ENABLE);
	GPIO_InitStructure.GPIO_Pin = MS5611_CS;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(MS5611_PORT, &GPIO_InitStructure);
	#ifdef DEBUG_MEG
		__DBG__("MS5611���ܴ�!\r\n");
	#endif
 }
 /*******************************************************************************
�� �� ����void MS5611_Init(void)
����������5611��ʼ��
��ڲ�����	��
���ز�������
����ʱ��: 2018-1-05 by zam
********************************************************************************/
 void MS5611_Init()
 {
		Init_MS5611_Cs();
		MS5611_Reset();
		MS5611_Read_PROM();
 }
 
 
 
 