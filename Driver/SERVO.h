#ifndef _SERVO_H
#define _SERVO_H

#include "stm32f4xx.h"
#include "Delay.h"

// ��ʼ�������� TIM3 �� PC8/PC9��
void SERVO_Config(void);

// ��ͨ������Ƕȣ����ӵ� PC8 TIM3_CH3��
void SERVO_TEXT(uint16_t angle);

// 360�� ����������ٶ� -100..+100�����ӵ� PC9 TIM3_CH4��
// speed:  negative -> reverse, positive -> forward, 0 -> stop
void SERVO360_SetSpeed(int16_t speed);
void SERVO360_Stop(void);

#endif
