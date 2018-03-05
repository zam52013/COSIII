/*
 * File      : lcd_drver.h
 * This file is ef comp
 * COPYRIGHT (C) 2017,
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at my addr
 *
 *
 * Change Logs:
 * Date           Author       ZAM
 * 2017-10-18     Bernard      the first version
 */

#ifndef __LCD_DRIVER_H
#define __LCD_DRIVER_H

#include "stm32f10x.h"
#include "stdlib.h"

////////////////////////////////////////////////////////////////////
//-----------------LCD�˿ڶ���---------------- 
#define LCD_LED_PORT 	GPIOG
#define LCD_LED_RCC 	RCC_APB2Periph_GPIOG
#define LCD_LED_GPIO    GPIO_Pin_11
#define LCD_LED_ON		GPIO_SetBits(LCD_LED_PORT,LCD_LED_GPIO)
#define LCD_LED_OFF		GPIO_ResetBits(LCD_LED_PORT,LCD_LED_GPIO)


#define LCD_RS_PORT 	GPIOG
#define LCD_RS_RCC 		RCC_APB2Periph_GPIOG
#define LCD_RS_GPIO    	GPIO_Pin_15
#define LCD_RS_ON		GPIO_SetBits(LCD_RS_PORT,LCD_RS_GPIO)
#define LCD_RS_OFF		GPIO_ResetBits(LCD_RS_PORT,LCD_RS_GPIO)

  /***SPI�豸lcd csѡ��**************************************/
#define LCD_SPI SPI2
#define LCD_CS_PORT GPIOB
#define LCD_CS_RCC RCC_APB2Periph_GPIOB
#define LCD_CS GPIO_Pin_12
#define LCD_CS_SET GPIO_SetBits(LCD_CS_PORT,LCD_CS)
#define LCD_CS_RESET GPIO_ResetBits(LCD_CS_PORT,LCD_CS)
/*****************************************************/
/***************************************************************/


//LCD��Ҫ������
typedef struct  
{										    
	u16 width;			//LCD ���
	u16 height;			//LCD �߶�
	u16 id;				//LCD ID
	u8  dir;			//���������������ƣ�0��������1��������	
	u16	 wramcmd;		//��ʼдgramָ��
	u16  setxcmd;		//����x����ָ��
	u16  setycmd;		//����y����ָ��	 
}_lcd_dev; 	

//LCD����
extern _lcd_dev lcddev;	//����LCD��Ҫ����

#define USE_HORIZONTAL  	0	//�����Ƿ�ʹ�ú��� 		0,��ʹ��.1,ʹ��.
//ʹ��Ӳ��SPI ����ģ��SPI��Ϊ����  
//����LCD�ĳߴ�
#if USE_HORIZONTAL==1	//ʹ�ú���
	#define LCD_W 320
	#define LCD_H 240
#else
	#define LCD_W 240
	#define LCD_H 320
#endif

//TFTLCD������Ҫ���õĺ���		   
extern u16  POINT_COLOR;//Ĭ�Ϻ�ɫ    
extern u16  BACK_COLOR; //������ɫ.Ĭ��Ϊ��ɫ


//������ɫ
#define WHITE       0xFFFF
#define BLACK      	0x0000	  
#define BLUE       	0x001F  
#define BRED        0XF81F
#define GRED 			 	0XFFE0
#define GBLUE			 	0X07FF
#define RED         0xF800
#define MAGENTA     0xF81F
#define GREEN       0x07E0
#define CYAN        0x7FFF
#define YELLOW      0xFFE0
#define BROWN 			0XBC40 //��ɫ
#define BRRED 			0XFC07 //�غ�ɫ
#define GRAY  			0X8430 //��ɫ
//GUI��ɫ

#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
//������ɫΪPANEL����ɫ 
 
#define LIGHTGREEN     	0X841F //ǳ��ɫ
//#define LIGHTGRAY     0XEF5B //ǳ��ɫ(PANNEL)
#define LGRAY 			 		0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE      	0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE          0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)
	    															  
extern u16 BACK_COLOR, POINT_COLOR ; 


void LCD_WR_REG(u8 Index);
void LCD_WR_DATA(u8 Data);
void LCD_WR_DATA_16Bit(u16 Data);
void LCD_SetCursor(u16 Xpos, u16 Ypos);
void Lcd_WriteReg(u8 Index,u8 Data);
void LCD_WriteRAM_Prepare(void);
void LCD_SetParam(void);
void LCD_DrawPoint(u16 x,u16 y);
void LCD_Init(void);
void Lcd_SetRegion(u16 x_start,u16 y_start,u16 x_end,u16 y_end);
void Lcd_SetXY(u16 x,u16 y);
void Gui_DrawPoint(u16 x,u16 y,u16 Data);
unsigned int Lcd_ReadPoint(u16 x,u16 y);
void LCD_Clear(u16 Color);


#endif

