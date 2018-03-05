/*
 * File      : tca62724.c
 * This file is ef comp
 * COPYRIGHT (C) 2018,
 *	led IIC����
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at my addr
 *
 *
 * Change Logs:
 * Date           Author       ZAM
 * 2018-1-16     Bernard      the first version
 */
 
 #include "tca62724.h"
 #include "iic.h"
 #include "usart.h"
 
 /********************************************************************/
 rgbled_rgbset RGB_SET_VALUE;
 static unsigned char Send_Led_IIC_Write(unsigned char* buf,  uint32_t nbyte, uint8_t SlaveAddress)
 {
		IIC_Start();
		IIC_Send_Byte(SlaveAddress<<1);
		if(!IIC_Wait_Ack())
		{
			return 1;
		}
		nbyte++;
		while(nbyte--)
		{
			IIC_Send_Byte(*buf++);
			if(!IIC_Wait_Ack())
			{
				return 1;
			}
		}
		IIC_Stop();
		return 1;
 }
 /********************************************************************
***��������: Send_Led_Enabel
***����˵��:ģʽ����
***�������:��
***�������:��
***
********************************************************************/
static void Send_Led_Enabel(unsigned char enable)
{
	unsigned char setting_byte=0;
	unsigned char msg[1];
	if(enable)
	{
		setting_byte |=SETTING_ENABLE;
	}
	setting_byte |=SETTING_NOT_POWERSAVE;
	msg[0]=setting_byte;
	//Send_Led_IIC_Write(msg,sizeof(msg),I2C_OBDEV_LED_ADD);
	I2C_Write(I2C_OBDEV,msg,sizeof(msg),I2C_OBDEV_LED_ADD,SUB_ADD_SETTINGS_ON);
	//I2C_Master_BufferWrite(I2C_OBDEV,msg,sizeof(msg),Polling,I2C_OBDEV_LED_ADD);
}
  /********************************************************************
***��������: Set_Color
***����˵��:��ɫ����
***�������:��ɫֵ
***�������:��
***
********************************************************************/
static void Set_Color(rgbled_color_t color)
{
	switch(color)
	{
		case REGLED_COLOR_OFF:
			RGB_SET_VALUE.RED_LED=0;
			RGB_SET_VALUE.GREEN_LED=0;
			RGB_SET_VALUE.BLUE_LED=0;
			break;
		case REGLED_COLOR_RED:
			RGB_SET_VALUE.RED_LED=0x0F;
			RGB_SET_VALUE.GREEN_LED=0;
			RGB_SET_VALUE.BLUE_LED=0;
			break;
		case REGLED_COLOR_YELLOW:
			RGB_SET_VALUE.RED_LED=0x0F;
			RGB_SET_VALUE.GREEN_LED=0x0C;
			RGB_SET_VALUE.BLUE_LED=0;
			break;
		case REGLED_COLOR_PURPLE:
			RGB_SET_VALUE.RED_LED=0x0F;
			RGB_SET_VALUE.GREEN_LED=0;
			RGB_SET_VALUE.BLUE_LED=0x0F;
			break;
		case REGLED_COLOR_GREEN:
			RGB_SET_VALUE.RED_LED=0;
			RGB_SET_VALUE.GREEN_LED=0x0F;
			RGB_SET_VALUE.BLUE_LED=0;
			break;
		case REGLED_COLOR_BLUE:
			RGB_SET_VALUE.RED_LED=0;
			RGB_SET_VALUE.GREEN_LED=0;
			RGB_SET_VALUE.BLUE_LED=0x0F;
			break;
		case REGLED_COLOR_WHITE:
			RGB_SET_VALUE.RED_LED=0x0F;
			RGB_SET_VALUE.GREEN_LED=0x0F;
			RGB_SET_VALUE.BLUE_LED=0x0F;
			break;
		case REGLED_COLOR_AMBER:
			RGB_SET_VALUE.RED_LED=0x0F;
			RGB_SET_VALUE.GREEN_LED=0x05;
			RGB_SET_VALUE.BLUE_LED=0;
			break;
		case REGLED_COLOR_DIM_RED:
			RGB_SET_VALUE.RED_LED=0x06;
			RGB_SET_VALUE.GREEN_LED=0;
			RGB_SET_VALUE.BLUE_LED=0;
			break;
		case REGLED_COLOR_DIM_YELLOW:
			RGB_SET_VALUE.RED_LED=0x05;
			RGB_SET_VALUE.GREEN_LED=0x02;
			RGB_SET_VALUE.BLUE_LED=0;
			break;
		case REGLED_COLOR_DIM_PURPLE:
			RGB_SET_VALUE.RED_LED=0x03;
			RGB_SET_VALUE.GREEN_LED=0;
			RGB_SET_VALUE.BLUE_LED=0x03;
			break;
		case REGLED_COLOR_DIM_GREEN:
			RGB_SET_VALUE.RED_LED=0;
			RGB_SET_VALUE.GREEN_LED=0x06;
			RGB_SET_VALUE.BLUE_LED=0;
			break;
		case REGLED_COLOR_DIM_BLUE:
			RGB_SET_VALUE.RED_LED=0;
			RGB_SET_VALUE.GREEN_LED=0;
			RGB_SET_VALUE.BLUE_LED=0x06;
			break;
		case REGLED_COLOR_DIM_WHITE:
			RGB_SET_VALUE.RED_LED=0x02;
			RGB_SET_VALUE.GREEN_LED=0x02;
			RGB_SET_VALUE.BLUE_LED=0x02;
			break;
		case REGLED_COLOR_DIM_AMBER:
			RGB_SET_VALUE.RED_LED=0x05;
			RGB_SET_VALUE.GREEN_LED=0x01;
			RGB_SET_VALUE.BLUE_LED=0;
			break;
		default:
			break;
	}
}
 /********************************************************************
***��������: Send_Led_Rgb
***����˵��:�����ɫֵ
***�������:��ɫֵ���Ƿ�ʹ��PWM����
***�������:��
***
********************************************************************/
 void Send_Led_Rgb(rgbled_color_t color,unsigned char enable)
{
	unsigned char msg[3];
	Send_Led_Enabel(enable);
	Set_Color(color);
	msg[0]=RGB_SET_VALUE.RED_LED;
	msg[1]=RGB_SET_VALUE.GREEN_LED;
	msg[2]=RGB_SET_VALUE.BLUE_LED;
	//Send_Led_IIC_Write(msg,sizeof(msg),I2C_OBDEV_LED_ADD);
	I2C_Write(I2C_OBDEV,msg,sizeof(msg),I2C_OBDEV_LED_ADD,SUB_ADD_ON_PWM0);
	//I2C_Master_BufferWrite(I2C_OBDEV,msg,sizeof(msg),Polling,I2C_OBDEV_LED_ADD);
}
 






