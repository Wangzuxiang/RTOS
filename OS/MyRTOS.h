/*
*********************************************************************************************************
* File      : MyRTOS.h
* Version   : V1.0
* By        : Zuxiang Wang
* For       : ARMv7M Cortex-M3
* Mode      : Thumb2
* Toolchain : Keil MDK 4.12
*********************************************************************************************************
*/
#ifndef _MYRTOS_H
#define _MYRTOS_H

#include "MyOS_type.h"

#define  OS_TASK_SW()         {OSCtxSw();}	                    /*任务切换宏*/

#define  OS_ENTER_CRITICAL()  {cpu_sr = OS_CPU_SR_Save();}	    /*进入临界区函数*/
#define  OS_EXIT_CRITICAL()   {OS_CPU_SR_Restore(cpu_sr);}		/*退出临界区函数*/

#define OS_TASK_MAX 32		  									/*最大任务数为32 */
#define IdelTaskPrio    31 		  								/*空闲任务优先级31  */

#define OSSetPrioRdy(prio)     {OSRdyTbl|=0x01 << (prio);}  	/*在任务就续表中登记指定任务*/
#define OSDelPrioRdy(prio)	   {OSRdyTbl&=~(0x01<<(prio));}		/*在任务就续表中删除指定任务*/

typedef struct TaskCtrlBlockHead
{
	INT32U OSTCBStkPtr;
	INT32U OSTCBDly;
} TaskCtrlBlock;											   /*任务控制块结构*/

/*系统变量声明*/
extern TaskCtrlBlock TCB[OS_TASK_MAX]; 							/*任务控制块表*/
extern TaskCtrlBlock *OSTCBCur;	   								/*当前任务指针*/
extern TaskCtrlBlock *OSTCBHighRdy;  							/*最高优先级就绪任务指针*/
extern INT8U         OSPrioCur;	   								/*当前任务优先级*/
extern INT8U         OSPrioHighRdy;								/*就绪任务最高优先级*/
extern INT8U         OSRunning;									/*任务运行标志*/
extern INT32U        OSTime;									/*任务运行时间*/
extern INT32U        OSInterruptSum;							/*进入中断次数*/
extern INT32U        OSIntNesting;		 						/*中断嵌套数*/
extern INT32U 		 OSRdyTbl;									/*任务就续表*/
extern INT32U 		 IDELTASK_STK[32];      					/*系统空闲任务堆栈*/

	
/*以下函数在MyRTOS.c中实现*/
void OSGetHighRdy(void);										/*在任务就续表中查找最高优先级任务*/
void OSTimeDly(INT32U ticks);									/*任务延时函数*/
void OSIntEnter(void);											/*进入中断函数*/
void OSIntExit(void);											/*退出中断函数*/									
void OSSched(void);												/*任务调度函数*/
void OSStart(void);												/*系统开始运行*/
void IdleTask(void *pdata);										/*系统空闲函数*/


void OSTaskCreate(void (*task)(void *pdata), void *p_arg, OS_STK *p_Stack, INT8U Prio);/*任务创建函数*/
void OSTaskSuspend(INT8U prio);									/*挂起指定任务*/
void OSTaskResume(INT8U prio);									/*回复指定的挂起任务*/

void OSTaskSwHook(void);										/*钩子函数，为空，无意义*/




/*以下函数在汇编语言文件中实现*/
OS_CPU_SR  OS_CPU_SR_Save(void);
void       OS_CPU_SR_Restore(OS_CPU_SR cpu_sr);
void OSStartHighRdy(void);										/*调度第一个任务*/
void OSCtxSw(void);												/*函数级任务切换*/
void OSIntCtxSw(void);											/*中断级任务切换*/

#endif

