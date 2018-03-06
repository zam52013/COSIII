#include "delay.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_UCOS
#include "App.h"					//ucos ʹ��	  
#endif
////////////////////////////////////////////////////////////////////////////////// 
// #ifdef CPU_CFG_CRITICAL_METHOD   //��CPU_CFG_CRITICAL_METHOD��������˵��ʹ����UCOSIII
// #define OS_CRITICAL_METHOD
// #define OS_TICKS_PER_SEC  OSCfg_TickRate_Hz
// #define OS_TRUE OS_STATE_OS_RUNNING
// #define OSLockNesting OSIntNestingCtr
// #endif  

static u8  fac_us=0;//us��ʱ������			   
static u16 fac_ms=0;//ms��ʱ������,��ucos��,����ÿ�����ĵ�ms��

unsigned char Time_statr_flag=0;	//������ʱ��־
unsigned char Time_out_flag=0;		//��ʱ��־
unsigned int Time_wait_cnt=0;		//�ȴ�ʱ��
/*--------------------------------------
��������time_tick
�������������ڳ�ʱ����
���룺��
�������
;-------------------------------------*/
void time_tick()
{
	static unsigned int Time_tic_cnt=0;
	if(Time_statr_flag)
	{
			if(Time_tic_cnt>=Time_wait_cnt)
			{
				Time_out_flag=1;
			}
			Time_tic_cnt++;
	}
	else
	{
		Time_tic_cnt=0;
	}
}

//��ʼ���ӳٺ���
//��ʹ��ucos��ʱ��,�˺������ʼ��ucos��ʱ�ӽ���
//SYSTICK��ʱ�ӹ̶�ΪHCLKʱ�ӵ�1/8
//SYSCLK:ϵͳʱ��
void delay_init(u8 SYSCLK)
{
	u32 reload;
 	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	fac_us=SYSCLK/8;		//�����Ƿ�ʹ��ucos,fac_us����Ҫʹ��
	fac_ms=(u16)fac_us*1000;//��ucos��,����ÿ��ms��Ҫ��systickʱ����   
	reload=SYSCLK/8;		//ÿ���ӵļ������� ��λΪK	     
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   	//����SYSTICK�ж�
	SysTick->LOAD=reload; 	//ÿ1/OS_TICKS_PER_SEC���ж�һ��	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;   	//����SYSTICK
}								    
#ifdef CPU_CFG_CRITICAL_METHOD
	void delay_us(u32 nus)
	{
		OS_ERR err;
		unsigned char us_cnt=0;
		u32 us_cnt_time=0;
		OSSchedLock(&err);	
		for(us_cnt_time=nus;us_cnt_time>0;us_cnt_time--)
		{
			for(us_cnt=72;us_cnt>0;us_cnt--);
		}
		OSSchedUnlock(&err);
	}
	void delay_ms(u16 nms)
	{
		OS_ERR err;
		OSTimeDly(  (OS_TICK    )nms, 
											(OS_OPT     )OS_OPT_TIME_DLY, 
											(OS_ERR     *)&err);
	}
#else  //��ʹ�ò���ϵͳ
	//��ʱnus
	//nusΪҪ��ʱ��us��.	
	//ע��:nus��ֵ,��Ҫ����798915us
	void delay_us(u32 nus)
	{		
		u32 temp;	    	  
		SysTick->LOAD=nus*fac_us; //ʱ�����	  		 
		SysTick->VAL=0x00;        //��ռ�����
		SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //��ʼ���� 
		do
		{
			temp=SysTick->CTRL;
		}
		while((temp&0x01)&&!(temp&(1<<16)));//�ȴ�ʱ�䵽��   
		SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //�رռ�����
		SysTick->VAL =0X00;       //��ռ�����	 
	}
	//��ʱnms
	//ע��nms�ķ�Χ
	//SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ:
	//nms<=0xffffff*8*1000/SYSCLK
	//SYSCLK��λΪHz,nms��λΪms
	//��168M������,nms<=798ms 
	void delay_xms(u16 nms)
	{	 		  	  
		u32 temp;		   
		SysTick->LOAD=(u32)nms*fac_ms;//ʱ�����(SysTick->LOADΪ24bit)
		SysTick->VAL =0x00;           //��ռ�����
		SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //��ʼ����  
		do
		{
			temp=SysTick->CTRL;
		}
		while((temp&0x01)&&!(temp&(1<<16)));//�ȴ�ʱ�䵽��   
		SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //�رռ�����
		SysTick->VAL =0X00;       //��ռ�����	  	    
	} 
	//��ʱnms 
	//nms:0~65535
	void delay_ms(u16 nms)
	{	 	 
		u8 repeat=nms/540;	//������540,�ǿ��ǵ�ĳЩ�ͻ����ܳ�Ƶʹ��,
							//���糬Ƶ��248M��ʱ��,delay_xms���ֻ����ʱ541ms������
		u16 remain=nms%540;
		while(repeat)
		{
			delay_xms(540);
			repeat--;
		}
		if(remain)delay_xms(remain);	
	}
#endif

// #ifdef OS_CRITICAL_METHOD 	//���OS_CRITICAL_METHOD������,˵��ʹ��ucos��.
// //��ʱnus
// //nus:Ҫ��ʱ��us��.		    								   
// void delay_us(u32 nus)
// {		
// #ifdef CPU_CFG_CRITICAL_METHOD   //ʹ��UCOSIII
// 	OS_ERR err;
// #endif
// 	u32 ticks;
// 	u32 told,tnow,tcnt=0;
// 	u32 reload=SysTick->LOAD;	//LOAD��ֵ	    	 
// 	ticks=nus*fac_us; 			//��Ҫ�Ľ�����	  		 
// 	tcnt=0;
// 	
// #ifdef CPU_CFG_CRITICAL_METHOD  //ʹ��UCOSIII
// 	OSSchedLock(&err);		
// #else							//����UCOSII
// 	OSSchedLock();				//��ֹucos���ȣ���ֹ���us��ʱ
// #endif
// 	
// 	told=SysTick->VAL;        	//�ս���ʱ�ļ�����ֵ
// 	while(1)
// 	{
// 		tnow=SysTick->VAL;	
// 		if(tnow!=told)
// 		{	    
// 			if(tnow<told)tcnt+=told-tnow;//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
// 			else tcnt+=reload-tnow+told;	    
// 			told=tnow;
// 			if(tcnt>=ticks)break;//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
// 		}  
// 	};
// 	
// #ifdef CPU_CFG_CRITICAL_METHOD  //ʹ��UCOSIII
// 	OSSchedUnlock(&err);		//����ucos���� 		
// #else							//����UCOSII
// 	OSSchedUnlock();			//����ucos���� 
// #endif									    
// }

// //��ʱnms
// //nms:Ҫ��ʱ��ms��
// void delay_ms(u16 nms)
// {	
// 	#ifdef CPU_CFG_CRITICAL_METHOD   //ʹ��UCOSIII
// 		OS_ERR err;
// 	#endif
// 	if(OSRunning==OS_TRUE&&OSLockNesting==0)//���os�Ѿ�������	   
// 	{		  
// 		if(nms>=fac_ms)//��ʱ��ʱ�����ucos������ʱ������ 
// 		{
// 			#ifdef CPU_CFG_CRITICAL_METHOD
// 				OSTimeDly(nms/fac_ms,OS_OPT_TIME_PERIODIC,&err);	//UCOSIII ��ʱ��������ģʽ
// 			#else
// 				OSTimeDly(nms/fac_ms);	//UCOSII��ʱ
// 			#endif
// 		}
// 		nms%=fac_ms;				//ucos�Ѿ��޷��ṩ��ôС����ʱ��,������ͨ��ʽ��ʱ    
// 	}
// 	delay_us((u32)(nms*1000));		//��ͨ��ʽ��ʱ 
// }
// #else  //��ʹ�ò���ϵͳ
// //��ʱnus
// //nusΪҪ��ʱ��us��.	
// //ע��:nus��ֵ,��Ҫ����798915us
// void delay_us(u32 nus)
// {		
// 	u32 temp;	    	  
// 	SysTick->LOAD=nus*fac_us; //ʱ�����	  		 
// 	SysTick->VAL=0x00;        //��ռ�����
// 	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //��ʼ���� 
// 	do
// 	{
// 		temp=SysTick->CTRL;
// 	}
// 	while((temp&0x01)&&!(temp&(1<<16)));//�ȴ�ʱ�䵽��   
// 	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //�رռ�����
// 	SysTick->VAL =0X00;       //��ռ�����	 
// }
// //��ʱnms
// //ע��nms�ķ�Χ
// //SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ:
// //nms<=0xffffff*8*1000/SYSCLK
// //SYSCLK��λΪHz,nms��λΪms
// //��168M������,nms<=798ms 
// void delay_xms(u16 nms)
// {	 		  	  
// 	u32 temp;		   
// 	SysTick->LOAD=(u32)nms*fac_ms;//ʱ�����(SysTick->LOADΪ24bit)
// 	SysTick->VAL =0x00;           //��ռ�����
// 	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //��ʼ����  
// 	do
// 	{
// 		temp=SysTick->CTRL;
// 	}
// 	while((temp&0x01)&&!(temp&(1<<16)));//�ȴ�ʱ�䵽��   
// 	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //�رռ�����
// 	SysTick->VAL =0X00;       //��ռ�����	  	    
// } 
// //��ʱnms 
// //nms:0~65535
// void delay_ms(u16 nms)
// {	 	 
// 	u8 repeat=nms/540;	//������540,�ǿ��ǵ�ĳЩ�ͻ����ܳ�Ƶʹ��,
// 						//���糬Ƶ��248M��ʱ��,delay_xms���ֻ����ʱ541ms������
// 	u16 remain=nms%540;
// 	while(repeat)
// 	{
// 		delay_xms(540);
// 		repeat--;
// 	}
// 	if(remain)delay_xms(remain);	
// }
// #endif
			 




































