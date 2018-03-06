/*
 * File      : flash_st.c
 * This file is ef comp
 * COPYRIGHT (C) 2017,
 *��Ƭ�ϴ洢���в��������������boot������
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at my addr
 *
 *
 * Change Logs:
 * Date           Author       ZAM
 * 2018-01-24     Bernard      the first version
 */
 
 #include "flash_st.h"
/*******************************************************************************
�� �� ����void Boot_Flash_ErasePage(void)
���������� ����flash
��ڲ�����	��
���ز�������
����ʱ��: 2018-01-24 by zam
********************************************************************************/
 void Boot_Flash_ErasePage()
 {
		uint32_t NbrOfPage;
		uint32_t EraseCounter;
		FLASH_Status FLASHStatus = FLASH_COMPLETE;
		FLASH_UnlockBank1();
	  NbrOfPage = (BANK1_WRITE_END_ADDR - BANK1_WRITE_START_ADDR) / FLASH_PAGE_SIZE;
	  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
		for(EraseCounter = 0; (EraseCounter < NbrOfPage) && (FLASHStatus == FLASH_COMPLETE); EraseCounter++)
		{
			FLASHStatus = FLASH_ErasePage(BANK1_WRITE_START_ADDR + (FLASH_PAGE_SIZE * EraseCounter));
		}
		FLASH_LockBank1();
 }
 /*******************************************************************************
�� �� ����void Boot_Flash_Write(void)
���������� дflash
��ڲ�����	Addr ��ַ  date ����  num����
���ز�������
����ʱ��: 2018-01-24 by zam
********************************************************************************/

 void Boot_Flash_Write(uint32_t Addr,uint32_t *date,uint16_t num)
 {
	 uint16_t cnt=0;
	 FLASH_UnlockBank1();
	 for(cnt=0;cnt<num;num++)
	 {
			FLASH_ProgramWord(Addr,*date);
			date++;
	 }
	 FLASH_LockBank1();
 }
  /*******************************************************************************
�� �� ����void Boot_Flash_Read(void)
���������� ��flash
��ڲ�����	Addr ��ַ  date ����  num����
���ز�������
����ʱ��: 2018-01-24 by zam
********************************************************************************/
 void Boot_Flash_Read(uint32_t Addr,uint32_t *date,uint16_t num)
 {
	 uint16_t cnt=0;
	 FLASH_UnlockBank1();
	 for(cnt=0;cnt<num;num++)
	 {
			*(date+cnt)=(*(__IO uint32_t*) Addr);
	 }
	 FLASH_LockBank1();
 }
   /*******************************************************************************
�� �� ����void BootLoader_Jump(uint32_t Sect,uint32_t Msp,uint32_t Reset)
���������� ��ת������ȥִ��
��ڲ�����	
���ز�������
����ʱ��: 2018-01-24 by zam
********************************************************************************/
 void BootLoader_Jump(uint32_t Sect,uint32_t Msp,uint32_t Reset)
 {
		uint32_t base;
		uint32_t offset;
		base=(Sect>NVIC_VectTab_FLASH)?NVIC_VectTab_FLASH:NVIC_VectTab_RAM;
		NVIC_SetVectorTable(base,offset);
		__set_MSP(Msp);
		((void (*)())(Reset))();
 }
 