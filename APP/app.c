#include "include.h"

/***************************************************************************************************************
*����ԭ�ͣ�		void Task_LED1(void *p_arg)
*����������		p_arg
*��������ֵ��  ��
*�������ܣ�	    LED1ÿ����˸һ�ε����� ����ͬ��
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
