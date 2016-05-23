#include "include.h"

/***************************************************************************************************************
*函数原型：		void Task_LED1(void *p_arg)
*函数参数：		p_arg
*函数返回值：  无
*函数功能：	    LED1每秒闪烁一次的任务 （下同）
*/
void Task_LED1(void *p_arg)
{
	while(1)
	{
		LED_On(LED1);
		OSTimeDly(1000);
		LED_Off(LED1);
		OSTimeDly(1000);
	}
}
void Task_LED2(void *p_arg)
{
	while(1)
	{
		LED_On(LED2);
		OSTimeDly(500);
		LED_Off(LED2);
		OSTimeDly(500);
	}
}

void Task_LED3(void *p_arg)
{
	while(1)
	{
		LED_On(LED3);
		OSTimeDly(250);
		LED_Off(LED3);
		OSTimeDly(250);
	}
}
