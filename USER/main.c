#include "include.h"

static OS_STK led1_task_stk[LED1_TASK_STK_SIZE];													 /*��������1��ջ*/
static OS_STK led2_task_stk[LED2_TASK_STK_SIZE];													 /*��������2��ջ*/
static OS_STK led3_task_stk[LED3_TASK_STK_SIZE];													 /*��������3��ջ*/

int main()
{
	BSP_Init();																   						 /*Ӳ����ʼ��*/
	SysTick_Init();															  						 /*ϵͳʱ�ӳ�ʼ��*/
	OSTaskCreate(Task_LED1,(void*)0,&led1_task_stk[LED1_TASK_STK_SIZE],LED1_TASK_PRIO );			 /*��������1�����ȼ�Ϊ5*/
	OSTaskCreate(Task_LED2,(void*)0,&led2_task_stk[LED2_TASK_STK_SIZE],LED2_TASK_PRIO);				 /*��������2�����ȼ�Ϊ8*/
	OSTaskCreate(Task_LED3,(void*)0,&led3_task_stk[LED3_TASK_STK_SIZE],LED3_TASK_PRIO);	             /*��������3�����ȼ�Ϊ2*/
	OSStart();																  						 /*����ϵͳ����ʼ������ȣ���������*/
	return 0;
}




