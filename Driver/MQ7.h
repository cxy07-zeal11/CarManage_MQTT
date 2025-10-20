#ifndef __MQ7_H
#define __MQ7_H

#include "stm32f4xx.h"

// ����DO��������
#define MQ7_DO_GPIO_PORT    GPIOC
#define MQ7_DO_GPIO_PIN     GPIO_Pin_7
#define MQ7_DO_GPIO_CLK     RCC_AHB1Periph_GPIOC

// ��ʼ��MQ7������(��ʹ��DO����)
void MQ7_Init(void);

// ��ȡDO����״̬
// ����ֵ: 0-δ������ֵ, 1-������ֵ
uint8_t MQ7_ReadDO(void);

#endif
