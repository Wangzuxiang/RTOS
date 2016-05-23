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

#define  OS_TASK_SW()         {OSCtxSw();}	                    /*�����л���*/

#define  OS_ENTER_CRITICAL()  {cpu_sr = OS_CPU_SR_Save();}	    /*�����ٽ�������*/
#define  OS_EXIT_CRITICAL()   {OS_CPU_SR_Restore(cpu_sr);}		/*�˳��ٽ�������*/

#define OS_TASK_MAX 32		  									/*���������Ϊ32 */
#define IdelTaskPrio    31 		  								/*�����������ȼ�31  */

#define OSSetPrioRdy(prio)     {OSRdyTbl|=0x01 << (prio);}  	/*������������еǼ�ָ������*/
#define OSDelPrioRdy(prio)	   {OSRdyTbl&=~(0x01<<(prio));}		/*�������������ɾ��ָ������*/

typedef struct TaskCtrlBlockHead
{
	INT32U OSTCBStkPtr;
	INT32U OSTCBDly;
} TaskCtrlBlock;											   /*������ƿ�ṹ*/

/*ϵͳ��������*/
extern TaskCtrlBlock TCB[OS_TASK_MAX]; 							/*������ƿ��*/
extern TaskCtrlBlock *OSTCBCur;	   								/*��ǰ����ָ��*/
extern TaskCtrlBlock *OSTCBHighRdy;  							/*������ȼ���������ָ��*/
extern INT8U         OSPrioCur;	   								/*��ǰ�������ȼ�*/
extern INT8U         OSPrioHighRdy;								/*��������������ȼ�*/
extern INT8U         OSRunning;									/*�������б�־*/
extern INT32U        OSTime;									/*��������ʱ��*/
extern INT32U        OSInterruptSum;							/*�����жϴ���*/
extern INT32U        OSIntNesting;		 						/*�ж�Ƕ����*/
extern INT32U 		 OSRdyTbl;									/*���������*/
extern INT32U 		 IDELTASK_STK[32];      					/*ϵͳ���������ջ*/

	
/*���º�����MyRTOS.c��ʵ��*/
void OSGetHighRdy(void);										/*������������в���������ȼ�����*/
void OSTimeDly(INT32U ticks);									/*������ʱ����*/
void OSIntEnter(void);											/*�����жϺ���*/
void OSIntExit(void);											/*�˳��жϺ���*/									
void OSSched(void);												/*������Ⱥ���*/
void OSStart(void);												/*ϵͳ��ʼ����*/
void IdleTask(void *pdata);										/*ϵͳ���к���*/


void OSTaskCreate(void (*task)(void *pdata), void *p_arg, OS_STK *p_Stack, INT8U Prio);/*���񴴽�����*/
void OSTaskSuspend(INT8U prio);									/*����ָ������*/
void OSTaskResume(INT8U prio);									/*�ظ�ָ���Ĺ�������*/

void OSTaskSwHook(void);										/*���Ӻ�����Ϊ�գ�������*/




/*���º����ڻ�������ļ���ʵ��*/
OS_CPU_SR  OS_CPU_SR_Save(void);
void       OS_CPU_SR_Restore(OS_CPU_SR cpu_sr);
void OSStartHighRdy(void);										/*���ȵ�һ������*/
void OSCtxSw(void);												/*�����������л�*/
void OSIntCtxSw(void);											/*�жϼ������л�*/

#endif

