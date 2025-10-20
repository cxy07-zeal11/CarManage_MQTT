#ifndef _EXTI_H
#define _EXTI_H
#include "stm32f4xx.h"
#include "Delay.h"
#include "KEY.h"
#include "LED.h"

void EXTI_Config(void);

extern uint8_t key1_flag;
extern uint8_t key2_flag;
#endif
