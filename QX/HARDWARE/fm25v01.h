/*
 * File      : fm25v01.h
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
 #ifndef __FM25V01_
 #define __FM25V01_
 #include "stm32f10x.h"
 #include "spi.h"
 
#define FM25V_WREN 0x06			/***дʹ��****/
#define FM25V_WRDI 0x04			/***дʧ��****/
#define FM25V_RDSR 0x05			/***��״̬****/
#define FM25V_WRSR 0x01			/***д״̬****/
#define FM25V_READ 0x03			/***������****/
#define FM25V_FSTRD 0x0B		/***���ٶ�����****/
#define FM25V_WRITE 0x02		/***д����****/
#define FM25V_SLEEP 0xB9		/***��������ģʽ****/
#define FM25V_RDID 0x9f			/***��ID****/

#define Device_Station_Addr 		0x0001		//�豸��ʶ

#define Move_Station_Net_Len_Addr 	0x0020		//�ƶ�վ���糤�ȵ�ַ
#define Move_Station_Net_Addr		0x0021		//�ƶ�վ����

#define Move_Station_Union_Len_Addr		0x0040	//�ƶ�վ��Ԫ��ַ���ȵ�ַ
#define Move_Station_Union_Addr		0x0041

#define Base_Station_Net_Len_Addr	0x0060		//��վ���糤�ȵ�ַ
#define Base_Station_Net_Addr		0x0061		//��վ����

 void fm25v_init(void);
 void Fram_wirte(u16 addr,u16 lenth,u8 *date);
 void Fram_read(u16 addr,u16 lenth,u8 *date);
 
 #endif 


