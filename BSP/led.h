#ifndef _LED_H
#define _LED_H

#include "stm32f10x.h"

#define RCC_GPIO_LED                    RCC_APB2Periph_GPIOF 
#define LEDn                            4            
#define GPIO_LED                        GPIOF      

#define DS1_PIN                         GPIO_Pin_6      
#define DS2_PIN                         GPIO_Pin_7		
#define DS3_PIN                         GPIO_Pin_8  	
#define DS4_PIN                         GPIO_Pin_9

typedef enum {LED1,LED2,LED3,LED4} LED;	

void GPIO_Config(void);
void LED_On(LED led);
void LED_Off(LED led);
void LED_Off_ALL(void);
void LED_On_ALL(void);

#endif
