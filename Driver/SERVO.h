#ifndef _SERVO_H
#define _SERVO_H

#include "stm32f4xx.h"
#include "Delay.h"

// 初始化（配置 TIM3 和 PC8/PC9）
void SERVO_Config(void);

// 普通舵机（角度，连接到 PC8 TIM3_CH3）
void SERVO_TEXT(uint16_t angle);

// 360° 连续舵机（速度 -100..+100，连接到 PC9 TIM3_CH4）
// speed:  negative -> reverse, positive -> forward, 0 -> stop
void SERVO360_SetSpeed(int16_t speed);
void SERVO360_Stop(void);

#endif
