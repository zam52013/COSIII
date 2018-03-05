#include "delay.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_UCOS
#include "App.h"					//ucos 使用	  
#endif
////////////////////////////////////////////////////////////////////////////////// 
// #ifdef CPU_CFG_CRITICAL_METHOD   //如CPU_CFG_CRITICAL_METHOD被定义了说明使用了UCOSIII
// #define OS_CRITICAL_METHOD
// #define OS_TICKS_PER_SEC  OSCfg_TickRate_Hz
// #define OS_TRUE OS_STATE_OS_RUNNING
// #define OSLockNesting OSIntNestingCtr
// #endif  

static u8  fac_us=0;//us延时倍乘数			   
static u16 fac_ms=0;//ms延时倍乘数,在ucos下,代表每个节拍的ms数

unsigned char Time_statr_flag=0;	//启动定时标志
unsigned char Time_out_flag=0;		//超时标志
unsigned int Time_wait_cnt=0;		//等待时间
/*--------------------------------------
函数名：time_tick
函数描述：用于超时作用
输入：无
输出：无
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

//初始化延迟函数
//当使用ucos的时候,此函数会初始化ucos的时钟节拍
//SYSTICK的时钟固定为HCLK时钟的1/8
//SYSCLK:系统时钟
void delay_init(u8 SYSCLK)
{
	u32 reload;
 	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	fac_us=SYSCLK/8;		//不论是否使用ucos,fac_us都需要使用
	fac_ms=(u16)fac_us*1000;//非ucos下,代表每个ms需要的systick时钟数   
	reload=SYSCLK/8;		//每秒钟的计数次数 单位为K	     
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   	//开启SYSTICK中断
	SysTick->LOAD=reload; 	//每1/OS_TICKS_PER_SEC秒中断一次	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;   	//开启SYSTICK
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
#else  //不使用操作系统
	//延时nus
	//nus为要延时的us数.	
	//注意:nus的值,不要大于798915us
	void delay_us(u32 nus)
	{		
		u32 temp;	    	  
		SysTick->LOAD=nus*fac_us; //时间加载	  		 
		SysTick->VAL=0x00;        //清空计数器
		SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //开始倒数 
		do
		{
			temp=SysTick->CTRL;
		}
		while((temp&0x01)&&!(temp&(1<<16)));//等待时间到达   
		SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //关闭计数器
		SysTick->VAL =0X00;       //清空计数器	 
	}
	//延时nms
	//注意nms的范围
	//SysTick->LOAD为24位寄存器,所以,最大延时为:
	//nms<=0xffffff*8*1000/SYSCLK
	//SYSCLK单位为Hz,nms单位为ms
	//对168M条件下,nms<=798ms 
	void delay_xms(u16 nms)
	{	 		  	  
		u32 temp;		   
		SysTick->LOAD=(u32)nms*fac_ms;//时间加载(SysTick->LOAD为24bit)
		SysTick->VAL =0x00;           //清空计数器
		SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //开始倒数  
		do
		{
			temp=SysTick->CTRL;
		}
		while((temp&0x01)&&!(temp&(1<<16)));//等待时间到达   
		SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //关闭计数器
		SysTick->VAL =0X00;       //清空计数器	  	    
	} 
	//延时nms 
	//nms:0~65535
	void delay_ms(u16 nms)
	{	 	 
		u8 repeat=nms/540;	//这里用540,是考虑到某些客户可能超频使用,
							//比如超频到248M的时候,delay_xms最大只能延时541ms左右了
		u16 remain=nms%540;
		while(repeat)
		{
			delay_xms(540);
			repeat--;
		}
		if(remain)delay_xms(remain);	
	}
#endif

// #ifdef OS_CRITICAL_METHOD 	//如果OS_CRITICAL_METHOD定义了,说明使用ucos了.
// //延时nus
// //nus:要延时的us数.		    								   
// void delay_us(u32 nus)
// {		
// #ifdef CPU_CFG_CRITICAL_METHOD   //使用UCOSIII
// 	OS_ERR err;
// #endif
// 	u32 ticks;
// 	u32 told,tnow,tcnt=0;
// 	u32 reload=SysTick->LOAD;	//LOAD的值	    	 
// 	ticks=nus*fac_us; 			//需要的节拍数	  		 
// 	tcnt=0;
// 	
// #ifdef CPU_CFG_CRITICAL_METHOD  //使用UCOSIII
// 	OSSchedLock(&err);		
// #else							//否则UCOSII
// 	OSSchedLock();				//阻止ucos调度，防止打断us延时
// #endif
// 	
// 	told=SysTick->VAL;        	//刚进入时的计数器值
// 	while(1)
// 	{
// 		tnow=SysTick->VAL;	
// 		if(tnow!=told)
// 		{	    
// 			if(tnow<told)tcnt+=told-tnow;//这里注意一下SYSTICK是一个递减的计数器就可以了.
// 			else tcnt+=reload-tnow+told;	    
// 			told=tnow;
// 			if(tcnt>=ticks)break;//时间超过/等于要延迟的时间,则退出.
// 		}  
// 	};
// 	
// #ifdef CPU_CFG_CRITICAL_METHOD  //使用UCOSIII
// 	OSSchedUnlock(&err);		//开启ucos调度 		
// #else							//否则UCOSII
// 	OSSchedUnlock();			//开启ucos调度 
// #endif									    
// }

// //延时nms
// //nms:要延时的ms数
// void delay_ms(u16 nms)
// {	
// 	#ifdef CPU_CFG_CRITICAL_METHOD   //使用UCOSIII
// 		OS_ERR err;
// 	#endif
// 	if(OSRunning==OS_TRUE&&OSLockNesting==0)//如果os已经在跑了	   
// 	{		  
// 		if(nms>=fac_ms)//延时的时间大于ucos的最少时间周期 
// 		{
// 			#ifdef CPU_CFG_CRITICAL_METHOD
// 				OSTimeDly(nms/fac_ms,OS_OPT_TIME_PERIODIC,&err);	//UCOSIII 延时采用周期模式
// 			#else
// 				OSTimeDly(nms/fac_ms);	//UCOSII延时
// 			#endif
// 		}
// 		nms%=fac_ms;				//ucos已经无法提供这么小的延时了,采用普通方式延时    
// 	}
// 	delay_us((u32)(nms*1000));		//普通方式延时 
// }
// #else  //不使用操作系统
// //延时nus
// //nus为要延时的us数.	
// //注意:nus的值,不要大于798915us
// void delay_us(u32 nus)
// {		
// 	u32 temp;	    	  
// 	SysTick->LOAD=nus*fac_us; //时间加载	  		 
// 	SysTick->VAL=0x00;        //清空计数器
// 	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //开始倒数 
// 	do
// 	{
// 		temp=SysTick->CTRL;
// 	}
// 	while((temp&0x01)&&!(temp&(1<<16)));//等待时间到达   
// 	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //关闭计数器
// 	SysTick->VAL =0X00;       //清空计数器	 
// }
// //延时nms
// //注意nms的范围
// //SysTick->LOAD为24位寄存器,所以,最大延时为:
// //nms<=0xffffff*8*1000/SYSCLK
// //SYSCLK单位为Hz,nms单位为ms
// //对168M条件下,nms<=798ms 
// void delay_xms(u16 nms)
// {	 		  	  
// 	u32 temp;		   
// 	SysTick->LOAD=(u32)nms*fac_ms;//时间加载(SysTick->LOAD为24bit)
// 	SysTick->VAL =0x00;           //清空计数器
// 	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //开始倒数  
// 	do
// 	{
// 		temp=SysTick->CTRL;
// 	}
// 	while((temp&0x01)&&!(temp&(1<<16)));//等待时间到达   
// 	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //关闭计数器
// 	SysTick->VAL =0X00;       //清空计数器	  	    
// } 
// //延时nms 
// //nms:0~65535
// void delay_ms(u16 nms)
// {	 	 
// 	u8 repeat=nms/540;	//这里用540,是考虑到某些客户可能超频使用,
// 						//比如超频到248M的时候,delay_xms最大只能延时541ms左右了
// 	u16 remain=nms%540;
// 	while(repeat)
// 	{
// 		delay_xms(540);
// 		repeat--;
// 	}
// 	if(remain)delay_xms(remain);	
// }
// #endif
			 




































