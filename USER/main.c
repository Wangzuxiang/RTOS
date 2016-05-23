#include "include.h"

static OS_STK led1_task_stk[LED1_TASK_STK_SIZE];													 /*定义任务1堆栈*/
static OS_STK led2_task_stk[LED2_TASK_STK_SIZE];													 /*定义任务2堆栈*/
static OS_STK led3_task_stk[LED3_TASK_STK_SIZE];													 /*定义任务3堆栈*/

int main()
{
	BSP_Init();																   						 /*硬件初始化*/
	SysTick_Init();															  						 /*系统时钟初始化*/
	OSTaskCreate(Task_LED1,(void*)0,&led1_task_stk[LED1_TASK_STK_SIZE],LED1_TASK_PRIO );			 /*创建任务1，优先级为5*/
	OSTaskCreate(Task_LED2,(void*)0,&led2_task_stk[LED2_TASK_STK_SIZE],LED2_TASK_PRIO);				 /*创建任务2，优先级为8*/
	OSTaskCreate(Task_LED3,(void*)0,&led3_task_stk[LED3_TASK_STK_SIZE],LED3_TASK_PRIO);	             /*创建任务3，优先级为2*/
	OSStart();																  						 /*启动系统，开始任务调度，永不返回*/
	return 0;
}




