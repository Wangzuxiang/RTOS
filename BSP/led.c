#include "stm32f10x.h"
#include"led.h"

void GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_GPIO_LED,ENABLE);
	GPIO_InitStructure.GPIO_Pin=DS1_PIN|DS2_PIN|DS3_PIN|DS4_PIN; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_LED,&GPIO_InitStructure);
}

void LED_On(LED led)
{
	switch (led)
	{
		case LED1 : 			GPIO_WriteBit(GPIOF,DS1_PIN,(BitAction)0x00);
								break;
		case LED2 : 		   	GPIO_WriteBit(GPIOF,DS2_PIN,(BitAction)0x00);	
								break;
		case LED3 : 			GPIO_WriteBit(GPIOF,DS3_PIN,(BitAction)0x00);
								break;
		case LED4 : 			GPIO_WriteBit(GPIOF,DS4_PIN,(BitAction)0x00);
								break;
	}
}
void LED_Off(LED led)
{
	switch(led)
	{
		case LED1 : 			GPIO_WriteBit(GPIOF,DS1_PIN,(BitAction)0x01);
								break;
		case LED2 : 			GPIO_WriteBit(GPIOF,DS2_PIN,(BitAction)0x01);
								break;
		case LED3 : 			GPIO_WriteBit(GPIOF,DS3_PIN,(BitAction)0x01);
								break;
		case LED4 : 			GPIO_WriteBit(GPIOF,DS4_PIN,(BitAction)0x01);
								break;
	}
}
void LED_Off_ALL(void)
{
		GPIO_WriteBit(GPIOF,DS1_PIN,(BitAction)0x01);
		GPIO_WriteBit(GPIOF,DS2_PIN,(BitAction)0x01);
		GPIO_WriteBit(GPIOF,DS3_PIN,(BitAction)0x01);
		GPIO_WriteBit(GPIOF,DS4_PIN,(BitAction)0x01);	
}
void LED_On_ALL(void)
{
		GPIO_WriteBit(GPIOF,DS1_PIN,(BitAction)0x00);
		GPIO_WriteBit(GPIOF,DS2_PIN,(BitAction)0x00);
		GPIO_WriteBit(GPIOF,DS3_PIN,(BitAction)0x00);
		GPIO_WriteBit(GPIOF,DS4_PIN,(BitAction)0x00);	
}
