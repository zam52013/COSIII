/*
 * File      : flash_st.h
 * This file is ef comp
 * COPYRIGHT (C) 2017,
 *¶ÔÆ¬ÉÏ´æ´¢½øĞĞ²Ù×÷ÕâÀï¿ÉÒÔÓÃÓÚbootÉı¼¶ÓÃ
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at my addr
 *
 *
 * Change Logs:
 * Date           Author       ZAM
 * 2018-01-24     Bernard      the first version
 */
 
 #ifndef __FLASH_ST_H
 #define __FLASH_ST_H
 
 
  #ifdef __cplusplus
 extern "C" {
#endif 
	 
	 
 #include "stm32f10x.h"
 
 #define BANK1_WRITE_END_ADDR 0x8080000					//½áÊøµØÖ· Õâ¸ö¸úÈİÁ¿´óĞ¡¶ø¶¨
 #define BANK1_WRITE_START_ADDR 0x8004000				//·ÖÅä8kÄÚ´æÓÃÓÚ´æ´¢boot
 #define FLASH_PAGE_SIZE 0x800		//·ÖÒ³´óĞ¡ Õâ¸ö¸úÈİÁ¿´óĞ¡¶ø¶
 
void Boot_Flash_ErasePage();
void Boot_Flash_Write(uint32_t Addr,uint32_t *date,uint16_t num);
void Boot_Flash_Read(uint32_t Addr,uint32_t *date,uint16_t num);
	
	 
#ifdef __cplusplus
}
#endif

 #endif
 
 