#include "init.h"
#include "usart.h"
/*******************************************************************************
* Function Name  : Delay
* Description    : Inserts a delay time.
* Input          : nCount: specifies the delay time length.
* Output         : None
* Return         : None
*******************************************************************************/
void Delay(volatile CPU_INT32U nCount)
{
  for(; nCount != 0; nCount--);
}

/*******************************************************************************
�� �� ����void IWDG_Init(void)
�������������Ź���ʼ��						
��ڲ�����							
���ز�����
����ʱ��: 2011.6.24
********************************************************************************/
void IWDG_Init(void)
{
	IWDG_WriteAccessCmd( IWDG_WriteAccess_Enable );
	IWDG_SetPrescaler( IWDG_Prescaler_64);	//��С
	IWDG_SetReload( 0x138);		//40KHz�ڲ�ʱ�� (1/40000 * 64 * 0x138 = 0.5s)
	IWDG_WriteAccessCmd( IWDG_WriteAccess_Disable );
	IWDG_Enable();
	IWDG_ReloadCounter();
		#ifdef DEBUG_MEG
			__DBG__("���Ź����ܴ�!\r\n");
		#endif
}
/*************************************************************************
��������:Soft_Reset
��������:�����λ����
�������:��
�������:��
*************************************************************************/
void Soft_Reset()
{
	__set_FAULTMASK(1);
	NVIC_SystemReset();
}
/*******************************************************************************
* Function Name :void SysTickInit(void)
* Description   :ϵͳ��ʱ��ʱ������
* Input         :
* Output        :
* Other         :ʱ��Ϊ1ms
* Date          :2011.11.03  12:59:13
*******************************************************************************/
void SysTickInit(void)
{
	SysTick_Config(SystemCoreClock / 1000);			//uCOSʱ��1ms
}

/*******************************************************************************
* Function Name :void InterruptOrder(void)
* Description   :�ж����������ȼ�
* Input         :
* Output        :
* Other         :
* Date          :2011.10.27  11:50:05
*******************************************************************************/
void NVIC_Configuration(void)
{
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4);//���ȼ�����  ȫΪ��ռʽ���ȼ�
}
/*******************************************************************************
* Function Name :void SystemConfig(void)
* Description   :ϵͳʱ��� ��ʼ��
* Input         :
* Output        :
* Other         :
* Date          :2012.6.15  13:14:59
*******************************************************************************/
#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
void  CPU_TS_TmrInit (void)
{
}
#endif
/*******************************************************************************
* Function Name :void SystemConfig(void)
* Description   :��ʱ��� ����ֵ
* Input         :�����ļ���ֵ
* Output        :
* Other         :
* Date          :2012.6.15  13:14:59
*******************************************************************************/
#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
CPU_TS_TMR  CPU_TS_TmrRd (void)
{
    return (SysTick->VAL);
}
#endif
