#ifndef _DELAY_H
#define _DELAY_H

#include "stm32f4xx.h"

void delay(volatile uint32_t cnt);

void delay_us(uint32_t num);
void delay_ms(uint32_t num);
void delay_s(uint32_t num);
void SysTick_Init(void);
uint32_t GetTick(void);

#endif
