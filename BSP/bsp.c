#include "include.h"

void BSP_Init()
{
	SystemInit();							  /*ϵͳȫ��ʱ���趨���趨SysTickʱ��Դ72M*/
	GPIO_Config();							  /*GPIO�ܽų�ʼ��*/
	LED_Off_ALL();							  /*ϵͳ����ǰ��LED��ȫ���ر�*/
}

void SysTick_Init()
{
	SysTick_Config(72000000/1000);			  /*ϵͳʱ���趨SysTick��1msһ������*/
}


