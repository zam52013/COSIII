/*
 * File      : ad.c
 * This file is ef comp
 * COPYRIGHT (C) 2017,
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at my addr
 *
 *
 * Change Logs:
 * Date           Author       ZAM
 * 2017-09-30     Bernard      the first version
 */
 
 #include "ad.h"
 #include "led.h"
 #include "wt5883.h"
 #include "usart.h"

/***********AD�˿����ñ�******************/
const uint32_t AD_IO_CONFIG[3][3]={
		{RCC_APB2Periph_GPIOA,GPIO_Pin_4,POARTA},
		{0,0,0},
		{0,0,0}
		//{RCC_APB2Periph_GPIOA,GPIO_Pin_9,POARTA}
 };
/*****************************************/
 static void Adc_Rcc_Init(void)
 {
	#ifdef ADC1_PERPH
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	#endif
	#ifdef ADC2_PERPH
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2,ENABLE);
	#endif
	#ifdef ADC3_PERPH
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3,ENABLE);
	#endif
 }
 static void Ad_Perph_Init(void)
 {
		ADC_InitTypeDef ADC_InitStructure;
		#ifdef ADC1_PERPH
			struct adc_configure config1=ADC_CONFIG_1;
		#endif
		#ifdef ADC2_PERPH
			struct adc_configure config2=ADC_CONFIG_2;
		#endif
		#ifdef ADC3_PERPH
			struct adc_configure config3=ADC_CONFIG_3;
		#endif
		#ifdef ADC1_PERPH
			ADC_InitStructure.ADC_Mode = config1.adc_mode;
			ADC_InitStructure.ADC_ScanConvMode = config1.adc_scanmode;
			ADC_InitStructure.ADC_ContinuousConvMode = config1.adc_convmode;
			ADC_InitStructure.ADC_ExternalTrigConv = config1.adc_trigcon;
			ADC_InitStructure.ADC_DataAlign = config1.adc_alig;
			ADC_InitStructure.ADC_NbrOfChannel = config1.adc_nchanel;
			ADC_Init(ADC1,&ADC_InitStructure);
			ADC_Cmd(ADC1,ENABLE);
			ADC_ResetCalibration(ADC1);
			while(ADC_GetResetCalibrationStatus(ADC1));
			ADC_StartCalibration(ADC1);
			while(ADC_GetCalibrationStatus(ADC1));
			ADC_SoftwareStartConvCmd(ADC1,DISABLE);
		#endif
		#ifdef ADC2_PERPH
			ADC_InitStructure.ADC_Mode = config2.adc_mode;
			ADC_InitStructure.ADC_ScanConvMode = config2.adc_scanmode;
			ADC_InitStructure.ADC_ContinuousConvMode = config2.adc_convmode;
			ADC_InitStructure.ADC_ExternalTrigConv = config2.adc_trigcon;
			ADC_InitStructure.ADC_DataAlign = config2.adc_alig;
			ADC_InitStructure.ADC_NbrOfChannel = config2.adc_nchanel;
			ADC_Init(ADC2,&ADC_InitStructure);
			ADC_Cmd(ADC2,ENABLE);
			ADC_ResetCalibration(ADC2);
			while(ADC_GetResetCalibrationStatus(ADC2));
			ADC_StartCalibration(ADC2);
			while(ADC_GetCalibrationStatus(ADC2));
			ADC_SoftwareStartConvCmd(ADC2,DISABLE);
		#endif
		#ifdef ADC3_PERPH
			ADC_InitStructure.ADC_Mode = config3.adc_mode;
			ADC_InitStructure.ADC_ScanConvMode = config3.adc_scanmode;
			ADC_InitStructure.ADC_ContinuousConvMode = config3.adc_convmode;
			ADC_InitStructure.ADC_ExternalTrigConv = config3.adc_trigcon;
			ADC_InitStructure.ADC_DataAlign = config3.adc_alig;
			ADC_InitStructure.ADC_NbrOfChannel = config3.adc_nchanel;
			ADC_Init(ADC3,&ADC_InitStructure);
			ADC_Cmd(ADC3,ENABLE);
			ADC_ResetCalibration(ADC3);
			while(ADC_GetResetCalibrationStatus(ADC3));
			ADC_StartCalibration(ADC3);
			while(ADC_GetCalibrationStatus(ADC3));
			ADC_SoftwareStartConvCmd(ADC3,DISABLE);
		#endif
 }
 static void Adcio_Init(void)
 {	
	unsigned int x;
	unsigned char i;
	GPIO_InitTypeDef GPIO_InitStructure;
	x=sizeof(AD_IO_CONFIG);
	x=x/12;
	for(i=0;i<x;i++)
	{
		if(AD_IO_CONFIG[i][0]!=0)
		{
			RCC_APB2PeriphClockCmd(AD_IO_CONFIG[i][0],ENABLE);
			GPIO_InitStructure.GPIO_Pin=AD_IO_CONFIG[i][1] & 0x0000ffff;
			GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;
			switch(AD_IO_CONFIG[i][2] & 0x0000000f)
			{
				case 1:
					GPIO_Init(GPIOA,&GPIO_InitStructure);
					break;
				case 2:
					GPIO_Init(GPIOB,&GPIO_InitStructure);
					break;
				case 3:
					GPIO_Init(GPIOC,&GPIO_InitStructure);
					break;
				case 4:
					GPIO_Init(GPIOD,&GPIO_InitStructure);
					break;
				case 5:
					GPIO_Init(GPIOE,&GPIO_InitStructure);
					break;
				case 6:
					GPIO_Init(GPIOF,&GPIO_InitStructure);
					break;
				case 7:
					GPIO_Init(GPIOG,&GPIO_InitStructure);
					break;
			}
		}
	}
 }
 void Adc_Init(void)
 {
 	Adc_Rcc_Init();
	Ad_Perph_Init();
	Adcio_Init();
	#ifdef DEBUG_PERPH
		#ifdef ADC1_PERPH
			printf("ADC1!\r\n");
		#endif
		#ifdef ADC2_PERPH
			printf("ADC2!\r\n");
		#endif
		#ifdef ADC3_PERPH
			printf("ADC3!\r\n");
		#endif
	#endif
 }
 /********************************************************************
***��������: Get_ADC1(u8 CH)
***����˵��:��ȡADC1��AD����ֵ
***�������:ͨ��
***�������:��ȡ��ֵ
***
********************************************************************/
u16 Get_ADC(ADC_TypeDef* ADCx,u8 CH)
{
	unsigned char retry=0;
	static u16 ad_buff=0;
	ADC_RegularChannelConfig(ADCx,CH, 1, ADC_SampleTime_13Cycles5);//����ָ����ͨ���Լ�ת������
	ADC_SoftwareStartConvCmd(ADCx,ENABLE);//����ת��
	while(!ADC_GetFlagStatus(ADCx,ADC_FLAG_EOC))//�ȴ�ת������
	{
		retry++;
		if(retry>200)
		return ad_buff;	 //�ɼ���������һ�ε�ֵ
	}
	ADC_SoftwareStartConvCmd(ADCx,DISABLE);//�ر�ת��
	ad_buff=ADC_GetConversionValue(ADCx);//���ػ�ȡ��AD�Ĵ���ֵ
	return ad_buff;
}
/********************************************************************
***��������: AD_Filter(u8 CH)
***����˵��:��ȡADC1��AD����ֵ��������λֵƽ���˲�
***�������:ͨ��
***�������:��ȡ��ֵ
***
********************************************************************/
u16 AD_Filter(ADC_TypeDef* ADCx,u8 CH)
{
	u16 AD_Buffer=0;
	u16 And=0;
	u16 Max=0;
	u16 Min=0x0fff;
	u8 i;
	for(i=0;i<4;i++)
	{
		AD_Buffer=Get_ADC(ADCx,CH);
		And+=AD_Buffer;
		if(AD_Buffer>Max) Max=AD_Buffer;
		if(AD_Buffer<Min) Min=AD_Buffer;
	}
	And=And-Max-Min;
	return (And>>1);
}
/********************************************************************
***��������: Ad_Led_statu(u16 value)
***����˵��:AD��������������������״̬��ʾ
***�������:ad����ֵ
***�������:
***
********************************************************************/

void Ad_Led_statu(u16 value)
{
	static unsigned char statu=0;
	static unsigned char alarm=0;
	//static unsigned char value_count=0;
	switch(statu)
	{
		case 0:
			if(value>BAT_NORMAL)
			{
				LED_PWO1_ON;
				LED_PWO2_OFF;
			}
			else
			{
				statu++;
			}
			break;
		case 1:
			if(value>BAT_ARM)
			{
				LED_PWO1_ON_OFF;
				LED_PWO2_OFF;
				if(!alarm)
				{
					Play_voice(POW_alarm);
					alarm=!alarm;
				}
			}
			else
			{
				alarm=0;
				statu++;
			}
			break;
		case 2:
			if(value>BAT_LOW)//�͵�ѹ
			{
				LED_PWO1_OFF;
				LED_PWO2_ON;
				if(++alarm>10)
				{
					Play_voice(LOW_POW);
					alarm=0;
				}
			}
			else
			{
				alarm=0;
				statu++;
			}
			break;
		case 3:
			LED_PWO1_OFF;
			LED_PWO2_OFF;
			//POWER_OFF;//�ػ�
			break;
		default:
			break;
	}
}
