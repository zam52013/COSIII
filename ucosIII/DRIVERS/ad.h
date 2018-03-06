/*
 * File      : ad.h
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
 #ifndef __AD_H
 #define __AD_H
 
 #ifdef __cplusplus
 extern "C" {
#endif 
	 
 #include "stm32f10x.h"
 /***********AD�豸****************/
 //#define ADC1_PERPH
// #define ADC2_PERPH
// #define ADC3_PERPH
 /***************************/
 #define POARTA  1
 #define POARTB  2
 #define POARTC  3
 #define POARTD  4
 #define POARTE  5
 #define POARTF  6
 #define POARTG  7
 /********AD����*******************/
 #define ADC_CONFIG_1	\
 {										\
		ADC_Mode_RegSimult,		\
		DISABLE,							\
	 DISABLE,								\
	 ADC_ExternalTrigConv_None,	\
	 ADC_DataAlign_Right,				\
	 1													\
}
 #define ADC_CONFIG_2	\
 {										\
		ADC_Mode_RegSimult,		\
		DISABLE,							\
	 DISABLE,								\
	 ADC_ExternalTrigConv_None,	\
	 ADC_DataAlign_Right,				\
	 1													\
}
 #define ADC_CONFIG_3	\
 {										\
		ADC_Mode_RegSimult,		\
		DISABLE,							\
	 DISABLE,								\
	 ADC_ExternalTrigConv_None,	\
	 ADC_DataAlign_Right,				\
	 1													\
}
 /***************************/
 
 struct adc_configure
 {
	uint32_t adc_mode;			/*****ADCģʽ****/
	FunctionalState adc_scanmode;		/*****��ͨ��ɨ��ģʽ****/
	FunctionalState adc_convmode;		/****ת��ģʽ*****/
	uint32_t adc_trigcon;		/****����ģʽ*****/
	uint32_t adc_alig;			/*****ADC�������з�ʽ****/
	uint8_t adc_nchanel;		/*****����ͨ����Ŀ****/
};
 /***********AD�˿����ñ���ad.c������******************/
 void Adc_Init(void);
 u16 Get_ADC(ADC_TypeDef* ADCx,u8 CH);
 u16 AD_Filter(ADC_TypeDef* ADCx,u8 CH);

 #ifdef __cplusplus
}
#endif

 #endif
 
