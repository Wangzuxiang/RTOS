/*
*********************************************************************************************************
* File      : MyRTOS.C
* Version   : V1.0
* By        : Zuxiang Wang
* For       : ARMv7M Cortex-M3
* Mode      : Thumb2
* Toolchain : Keil MDK 4.12
*********************************************************************************************************
*/
#include "MyRTOS.h"

/*ϵͳ��������*/
TaskCtrlBlock TCB[OS_TASK_MAX]; 								/*������ƿ��*/
TaskCtrlBlock *OSTCBCur;	   									/*��ǰ����ָ��*/
TaskCtrlBlock *OSTCBHighRdy;  									/*������ȼ���������ָ��*/
INT8U         OSPrioCur;	   									/*��ǰ�������ȼ�*/
INT8U         OSPrioHighRdy;									/*��������������ȼ�*/
INT8U         OSRunning;										/*�������б�־*/
INT32U        OSTime;											/*��������ʱ��*/
INT32U        OSInterruptSum;									/*�����жϴ���*/
INT32U        OSIntNesting;		 								/*�ж�Ƕ����*/
INT32U 		  IDELTASK_STK[32];									/*ϵͳ���������ջ*/
INT32U        OSRdyTbl;											/*�����������*/


/*
**********************************************************************************************************
*����ԭ�ͣ�        void OSGetHighRdy(void) 
*����������			��
*��������ֵ��		��
*�������ܣ�        ������������в���������ȼ�����
**********************************************************************************************************
*/
void OSGetHighRdy(void)
{
	INT8U OSNextTaskPrio = 0;
	for(OSNextTaskPrio=0; OSNextTaskPrio < OS_TASK_MAX; OSNextTaskPrio++)
	{
		if(OSRdyTbl == 0x80000000)
		{
			OSPrioHighRdy = 31;
			break;
		}
		if(OSRdyTbl&(0x01<<OSNextTaskPrio))
		{
			OSPrioHighRdy = OSNextTaskPrio;
			break;
		}
	}
}
/*
***********************************************************************************************************************
*����ԭ�ͣ�        void OSTimeDly(INT32U ticks)
*���������� 		ticks
*��������ֵ��		��
*�������ܣ�			��������ʱһ��ʱ��
***********************************************************************************************************************
*/
void OSTimeDly(INT32U ticks)
{
	OS_CPU_SR  cpu_sr = 0;
	if(ticks > 0)					 							/*��ʱ��Ч*/
	{													
		OS_ENTER_CRITICAL();									/*�����ٽ���*/
		OSDelPrioRdy(OSPrioCur);								/*ɾ����ǰ����*/
		TCB[OSPrioCur].OSTCBDly = ticks;						/*������ʱ������*/
		OS_EXIT_CRITICAL();                         			/*�˳��ٽ���*/
		OSSched();                                  			/*�������*/
	}
}
/*
************************************************************************************************************************
*����ԭ�ͣ�    void OSSched(void) 
*����������		��
*��������ֵ��	��
*�������ܣ�    ����һ���������
************************************************************************************************************************
*/
void OSSched()
{
	OS_CPU_SR  cpu_sr = 0;
	OS_ENTER_CRITICAL();
	OSGetHighRdy();
	if(OSPrioHighRdy !=  OSPrioCur)
	{
		OSTCBCur = &TCB[OSPrioCur];
		OSTCBHighRdy = &TCB[OSPrioHighRdy];
		OSPrioCur = OSPrioHighRdy;
		OS_TASK_SW();
	}
	OS_EXIT_CRITICAL();
}
/*
******************************************************************************************************************
*����ԭ�ͣ�		void OSTaskCreate(void (*task)(void *pdata), void *p_arg, OS_STK *p_Stack, INT8U Prio)
*����������    task, p_arg, p_Stack, Prio
*��������ֵ��  ��
*�������ܣ�    ����һ��ϵͳ���񣬲�ָ�����ȼ�
*******************************************************************************************************************
*/
void OSTaskCreate(void (*task)(void *pdata), void *p_arg, OS_STK *p_Stack, INT8U Prio)
{
	if(Prio <= OS_TASK_MAX)
	{
		*(p_Stack) = (INT32U)0x01000000L;					
	    *(--p_Stack) = (INT32U)task;								
	    *(--p_Stack) = (INT32U)0xFFFFFFFEL;							
	                                                                           
	    *(--p_Stack) = (INT32U)0x12121212L;						
	    *(--p_Stack) = (INT32U)0x03030303L;						
	    *(--p_Stack) = (INT32U)0x02020202L;						
	    *(--p_Stack) = (INT32U)0x01010101L;					
		*(--p_Stack) = (INT32U)p_arg;						

		*(--p_Stack) = (INT32U)0x11111111L;					
	    *(--p_Stack) = (INT32U)0x10101010L;				
	    *(--p_Stack) = (INT32U)0x09090909L;					
	    *(--p_Stack) = (INT32U)0x08080808L;				
	    *(--p_Stack) = (INT32U)0x07070707L;					
	    *(--p_Stack) = (INT32U)0x06060606L;					
	    *(--p_Stack) = (INT32U)0x05050505L;					
	    *(--p_Stack) = (INT32U)0x04040404L;												/*�����ջ��ʼ��*/
		TCB[Prio].OSTCBStkPtr = (INT32U)p_Stack;
		TCB[Prio].OSTCBDly = 0;
		OSSetPrioRdy(Prio);
	}
}
/*
*****************************************************************************************************************
*����ԭ�ͣ�		 void OSTaskSuspend(INT8U prio)
*����������		 ��
*��������ֵ��   ��
*�������ܣ�     ��ָ�����ȼ������������������ǵ�ǰ���񣬽���һ���������
*****************************************************************************************************************
*/
void OSTaskSuspend(INT8U prio)
{
	OS_CPU_SR  cpu_sr = 0;
	OS_ENTER_CRITICAL();
	TCB[prio].OSTCBDly = 0;
	OSDelPrioRdy(prio);                   							/*�������������ɾ��*/
	OS_EXIT_CRITICAL();
	if(OSPrioCur == prio)				 							/*��Ҫ���������Ϊ��ǰ�����ǣ����µ���*/
	{
		OSSched();
	}
}
/*
********************************************************************************************************************
*����ԭ�ͣ�		void OSTaskResume(INT8U prio)
*����������    ��
*��������ֵ��  ��
*�������ܣ�    �����������ָ������ָ����������ȼ��ȵ�ǰ�������ȼ��ߣ�����һ���������
********************************************************************************************************************
*/
void OSTaskResume(INT8U prio)
{
	OS_CPU_SR  cpu_sr = 0;
	OS_ENTER_CRITICAL();
	OSSetPrioRdy(prio);
	TCB[prio].OSTCBDly = 0;
	OS_EXIT_CRITICAL();
	if(OSPrioCur > prio)
	{
		OSSched();	
	}
}
/*
**********************************************************************************************************************
*����ԭ�ͣ� 	 void OSStart(void)
*����������     ��
*��������ֵ��   ��
*�������ܣ�     ����ϵͳ���ҵ�������ȼ��������У�����ʼϵͳ���������
**********************************************************************************************************************
*/
void OSStart()
{
	if(OSRunning == 0)
	{
		OSRunning = 1;

		OSTaskCreate(IdleTask, (void *)0, (OS_STK *)&IDELTASK_STK[31], IdelTaskPrio);	/*������������	*/

		OSGetHighRdy();
		OSPrioCur = OSPrioHighRdy;
		OSTCBCur = &TCB[OSPrioCur];
		OSTCBHighRdy = &TCB[OSPrioHighRdy];
		OSStartHighRdy();
	}
}
/*
********************************************************************************************************************
*����ԭ�ͣ�		void OSIntEnter(void)
*����������    ��
*��������ֵ��  ��
*�������ܣ�    �����жϣ����ж�Ƕ������һ
********************************************************************************************************************
*/
void OSIntEnter()
{
	OS_CPU_SR cpu_sr;
	OS_ENTER_CRITICAL();
	if(OSIntNesting < 255)
	{
		OSIntNesting++;
	}
	OS_EXIT_CRITICAL();
}
/*
**********************************************************************************************************************
*����ԭ�ͣ�		 void OSIntExit(void)
*����������      ��
*��������ֵ��    ��
*�������ܣ�      �˳��жϣ����ж�Ƕ������һ��������һ���������
**********************************************************************************************************************
*/
void OSIntExit()
{
	OS_CPU_SR  cpu_sr = 0;
	OS_ENTER_CRITICAL();
	
	if(OSIntNesting > 0)
		OSIntNesting--;
	if(OSIntNesting == 0)
	{
		OSGetHighRdy();												/*�ҳ��������ȼ���ߵľ�������*/
		if(OSPrioHighRdy!=OSPrioCur)								/*��ǰ���񲢷����ȼ���ߵľ�������*/
		{
			OSTCBCur = &TCB[OSPrioCur];
			OSTCBHighRdy = &TCB[OSPrioHighRdy];
			OSPrioCur = OSPrioHighRdy;
			OSIntCtxSw();											/*�жϼ��������*/
		}
	}
	OS_EXIT_CRITICAL();
}
/*
********************************************************************************************************************
*����ԭ�ͣ�		  void IdleTask(void *pdata)
*����������      pdata
*��������ֵ��    ��
*�������ܣ�      ����ϵͳ�������񣬲��ϵĶ�IdelCount��������CPU���¿���
********************************************************************************************************************
*/
void IdleTask(void *pdata)
{
	INT32U IdleCount = 0;
	while(1)
	{
		IdleCount++;
	}
}
/*���Ӻ�������ຯ���е��ã�������*/
void OSTaskSwHook(void)
{

}
