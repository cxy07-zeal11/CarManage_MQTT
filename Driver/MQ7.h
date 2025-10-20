#ifndef __MQ7_H
#define __MQ7_H

#include "stm32f4xx.h"

// 定义DO引脚连接
#define MQ7_DO_GPIO_PORT    GPIOC
#define MQ7_DO_GPIO_PIN     GPIO_Pin_7
#define MQ7_DO_GPIO_CLK     RCC_AHB1Periph_GPIOC

// 初始化MQ7传感器(仅使用DO引脚)
void MQ7_Init(void);

// 读取DO引脚状态
// 返回值: 0-未超过阈值, 1-超过阈值
uint8_t MQ7_ReadDO(void);

#endif
