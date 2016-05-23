#include "include.h"

void BSP_Init()
{
	SystemInit();							  /*系统全局时钟设定，设定SysTick时钟源72M*/
	GPIO_Config();							  /*GPIO管脚初始化*/
	LED_Off_ALL();							  /*系统启动前，LED灯全部关闭*/
}

void SysTick_Init()
{
	SysTick_Config(72000000/1000);			  /*系统时钟设定SysTick，1ms一个节拍*/
}


