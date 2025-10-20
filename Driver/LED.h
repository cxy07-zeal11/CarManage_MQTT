#ifndef _LED_H
#define _LED_H

#include "stm32f4xx.h"
#include "Delay.h"

#define LED_CLK RCC_AHB1Periph_GPIOG

#define LED1_PIN GPIO_Pin_14
#define LED2_PIN GPIO_Pin_13
#define LED3_PIN GPIO_Pin_6
#define LED4_PIN GPIO_Pin_11

#define LED_GROUP GPIOG


void LED_Config(void);

void LED_Flash(void);

void LED_Horse(void);

void LED_Water(void);
// 宏定义
// 引入其他头文件
// extern变量
// 声明函数


#endif
