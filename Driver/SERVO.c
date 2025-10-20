// SERVO.c
#include "SERVO.h"
#include <string.h>

void SERVO_Config(void)
{
    // ��ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

    // PC8 -> TIM3_CH3, PC9 -> TIM3_CH4
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_TIM3);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_TIM3);

    // ���� PC8 �� PC9
    GPIO_InitTypeDef GPIO_InitStruct;
    memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitStruct));
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStruct);

    // ��ʱ���������ã����� 1MHz tick -> ARR=20000 -> 20ms ���ڣ�
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Period = 20000 - 1;   // 20ms
    TIM_TimeBaseInitStruct.TIM_Prescaler = 84 - 1;   // 168MHz core -> APB1 timer = 84MHz -> PSC�� 1MHz
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);

    // ����Ƚϣ�PWM��ͨ������
    TIM_OCInitTypeDef TIM_OCInitStruct;
    TIM_OCStructInit(&TIM_OCInitStruct);
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStruct.TIM_Pulse = 1500;              // 1.5ms ��λ����ȫ��ʼ��
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;

    // ��ʼ�� CH3 (PC8) �� CH4 (PC9)
    TIM_OC3Init(TIM3, &TIM_OCInitStruct);
    TIM_OC4Init(TIM3, &TIM_OCInitStruct);

    TIM_OC3PreloadConfig(TIM3, ENABLE);
    TIM_OC4PreloadConfig(TIM3, ENABLE);
    TIM_ARRPreloadConfig(TIM3, ENABLE);

    // ��Ҫ���� TIM_CtrlPWMOutputs(TIM3, ...)���ýӿ����ڸ߼���ʱ����TIM1/TIM8��
    TIM_Cmd(TIM3, ENABLE);
}

/* ��ͨ������Ƕ� 0..180 ӳ�䵽 1.0ms..2.0ms ��CH3: PC8�� */
void SERVO_TEXT(uint16_t angle)
{
    if (angle > 180) angle = 180;
  uint32_t count = 500 + (uint32_t)angle * 2000 / 180; // 500~2500us
    TIM_SetCompare3(TIM3, (uint16_t)count);
}

/* 360�㣨������ת�������CH4 (PC9)
   speed: -100 .. 0 .. +100
     -100 -> ���ת��pulse 1000��
       0  -> ֹͣ/������pulse 1500��
     +100 -> �����ת��pulse 2000��
   ע�⣺��ͬ�������/�������в��죬����У׼��
*/
void SERVO360_SetSpeed(int16_t speed)
{
    if (speed > 100) speed = 100;
    if (speed < -100) speed = -100;
    int32_t pulse = 1500 + (int32_t)speed * 5; // speed *5 -> -500..+500
    TIM_SetCompare4(TIM3, (uint16_t)pulse);
}

/* ���ֹͣ��������ص������� */
void SERVO360_Stop(void)
{
    TIM_SetCompare4(TIM3, 1500);
}
