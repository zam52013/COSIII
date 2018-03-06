/*
 * File      : can.h
 * This file is ef comp
 * COPYRIGHT (C) 2018,
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at my addr
 *+-----------------------------------------------------------+
 * |                     ����˵��                            |
 +-----------------------------+---------------+-------------+
 *   CAN1:   PB11....CAN_RX    PB12....CAN_TX
 *
 * Change Logs:
 * Date           Author       ZAM
 * 2018-01-9     Bernard      the first version
 */
 
 #ifndef __CAN_H
 #define __CAN_H
 
  #ifdef __cplusplus
 extern "C" {
#endif 
 #include "stm32f10x.h"
 
 /**********CAN�豸******************/
 #define CAN1_PERPH
 /***********************************/
 
 
 /***************������������*******************/
 #define CAN_BAUDRATE  1000      /* 1MBps   */
/* #define CAN_BAUDRATE  500*/  /* 500kBps */
/* #define CAN_BAUDRATE  250*/  /* 250kBps */
/* #define CAN_BAUDRATE  125*/  /* 125kBps */
/* #define CAN_BAUDRATE  100*/  /* 100kBps */ 
/* #define CAN_BAUDRATE  50*/   /* 50kBps  */ 
/* #define CAN_BAUDRATE  20*/   /* 20kBps  */ 
/* #define CAN_BAUDRATE  10*/   /* 10kBps  */ 
/***********************************/
/************�����͵�����Э��*********************/
#define Send_Date_Config1							\
{																			\
	0x12,																\
	0x12,																\
	CAN_Id_Standard,										\
	CAN_RTR_Data												\
}
/***********************************/

struct Can_Date_Config
{
	uint32_t StdId;  /*****��׼��ʶ��****/
  uint32_t ExtId; /*****��չ��ʶ��****/
  uint8_t IDE;     /*****��ʾ�����****/
  uint8_t RTR;  	/*****��Ϣ���****/
};
 #ifdef __cplusplus
}
#endif

 #endif
 
 

