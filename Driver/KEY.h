#ifndef _KEY_H
#define _KEY_H
#include "stm32f4xx.h"
#include "Delay.h"

#define KEY_CLK RCC_AHB1Periph_GPIOG

#define KEY1_PIN GPIO_Pin_2
#define KEY2_PIN GPIO_Pin_3
#define KEY3_PIN GPIO_Pin_4
#define KEY4_PIN GPIO_Pin_5

#define KEY_GROUP GPIOG

void KEY_Config(void);
uint8_t KEY_Scanf(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

#endif
