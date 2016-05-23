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

/*系统变量定义*/
TaskCtrlBlock TCB[OS_TASK_MAX]; 								/*任务控制块表*/
TaskCtrlBlock *OSTCBCur;	   									/*当前任务指针*/
TaskCtrlBlock *OSTCBHighRdy;  									/*最高优先级就绪任务指针*/
INT8U         OSPrioCur;	   									/*当前任务优先级*/
INT8U         OSPrioHighRdy;									/*就绪任务最高优先级*/
INT8U         OSRunning;										/*任务运行标志*/
INT32U        OSTime;											/*任务运行时间*/
INT32U        OSInterruptSum;									/*进入中断次数*/
INT32U        OSIntNesting;		 								/*中断嵌套数*/
INT32U 		  IDELTASK_STK[32];									/*系统空闲任务堆栈*/
INT32U        OSRdyTbl;											/*任务就续表定义*/


/*
**********************************************************************************************************
*函数原型：        void OSGetHighRdy(void) 
*函数参数：			无
*函数返回值：		无
*函数功能：        在任务就续表中查找最高优先级任务
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
*函数原型：        void OSTimeDly(INT32U ticks)
*函数参数： 		ticks
*函数返回值：		无
*函数功能：			将任务延时一段时间
***********************************************************************************************************************
*/
void OSTimeDly(INT32U ticks)
{
	OS_CPU_SR  cpu_sr = 0;
	if(ticks > 0)					 							/*延时有效*/
	{													
		OS_ENTER_CRITICAL();									/*进入临界区*/
		OSDelPrioRdy(OSPrioCur);								/*删除当前任务*/
		TCB[OSPrioCur].OSTCBDly = ticks;						/*设置延时节拍数*/
		OS_EXIT_CRITICAL();                         			/*退出临界区*/
		OSSched();                                  			/*任务调度*/
	}
}
/*
************************************************************************************************************************
*函数原型：    void OSSched(void) 
*函数参数：		无
*函数返回值：	无
*函数功能：    进行一次任务调度
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
*函数原型：		void OSTaskCreate(void (*task)(void *pdata), void *p_arg, OS_STK *p_Stack, INT8U Prio)
*函数参数：    task, p_arg, p_Stack, Prio
*函数返回值：  无
*函数功能：    创建一个系统任务，并指定优先级
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
	    *(--p_Stack) = (INT32U)0x04040404L;												/*任务堆栈初始化*/
		TCB[Prio].OSTCBStkPtr = (INT32U)p_Stack;
		TCB[Prio].OSTCBDly = 0;
		OSSetPrioRdy(Prio);
	}
}
/*
*****************************************************************************************************************
*函数原型：		 void OSTaskSuspend(INT8U prio)
*函数参数：		 无
*函数返回值：   无
*函数功能：     将指定优先级的任务挂起，若挂起的是当前任务，进行一次任务调度
*****************************************************************************************************************
*/
void OSTaskSuspend(INT8U prio)
{
	OS_CPU_SR  cpu_sr = 0;
	OS_ENTER_CRITICAL();
	TCB[prio].OSTCBDly = 0;
	OSDelPrioRdy(prio);                   							/*从任务就续表中删除*/
	OS_EXIT_CRITICAL();
	if(OSPrioCur == prio)				 							/*当要挂起的任务为当前任务是，重新调度*/
	{
		OSSched();
	}
}
/*
********************************************************************************************************************
*函数原型：		void OSTaskResume(INT8U prio)
*函数参数：    无
*函数返回值：  无
*函数功能：    将挂起的任务恢复，若恢复的任务优先级比当前任务优先级高，进行一次任务调度
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
*函数原型： 	 void OSStart(void)
*函数参数：     无
*函数返回值：   无
*函数功能：     启动系统，找到最高优先级任务运行，并开始系统多任务调度
**********************************************************************************************************************
*/
void OSStart()
{
	if(OSRunning == 0)
	{
		OSRunning = 1;

		OSTaskCreate(IdleTask, (void *)0, (OS_STK *)&IDELTASK_STK[31], IdelTaskPrio);	/*创建空闲任务	*/

		OSGetHighRdy();
		OSPrioCur = OSPrioHighRdy;
		OSTCBCur = &TCB[OSPrioCur];
		OSTCBHighRdy = &TCB[OSPrioHighRdy];
		OSStartHighRdy();
	}
}
/*
********************************************************************************************************************
*函数原型：		void OSIntEnter(void)
*函数参数：    无
*函数返回值：  无
*函数功能：    进入中断，将中断嵌套数加一
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
*函数原型：		 void OSIntExit(void)
*函数参数：      无
*函数返回值：    无
*函数功能：      退出中断，将中断嵌套数减一，并进行一次任务调度
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
		OSGetHighRdy();												/*找出任务优先级最高的就绪任务*/
		if(OSPrioHighRdy!=OSPrioCur)								/*当前任务并非优先级最高的就绪任务*/
		{
			OSTCBCur = &TCB[OSPrioCur];
			OSTCBHighRdy = &TCB[OSPrioHighRdy];
			OSPrioCur = OSPrioHighRdy;
			OSIntCtxSw();											/*中断级任务调度*/
		}
	}
	OS_EXIT_CRITICAL();
}
/*
********************************************************************************************************************
*函数原型：		  void IdleTask(void *pdata)
*函数参数：      pdata
*函数返回值：    无
*函数功能：      定义系统空闲任务，不断的对IdelCount计数，让CPU有事可做
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
/*钩子函数，汇编函数中调用，无意义*/
void OSTaskSwHook(void)
{

}
