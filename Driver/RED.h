#ifndef _RED_H
#define _RED_H
#include "stm32f4xx.h"

void RED_Config(void);

#define RED_CLK RCC_AHB1Periph_GPIOB
#define RED_GROUP GPIOB
#define RED_PIN GPIO_Pin_6

#endif