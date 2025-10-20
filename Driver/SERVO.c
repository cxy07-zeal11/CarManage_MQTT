// SERVO.c
#include "SERVO.h"
#include <string.h>

void SERVO_Config(void)
{
    // 打开时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

    // PC8 -> TIM3_CH3, PC9 -> TIM3_CH4
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_TIM3);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_TIM3);

    // 配置 PC8 和 PC9
    GPIO_InitTypeDef GPIO_InitStruct;
    memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitStruct));
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStruct);

    // 定时器基础配置（生成 1MHz tick -> ARR=20000 -> 20ms 周期）
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Period = 20000 - 1;   // 20ms
    TIM_TimeBaseInitStruct.TIM_Prescaler = 84 - 1;   // 168MHz core -> APB1 timer = 84MHz -> PSC后 1MHz
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);

    // 输出比较（PWM）通用设置
    TIM_OCInitTypeDef TIM_OCInitStruct;
    TIM_OCStructInit(&TIM_OCInitStruct);
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStruct.TIM_Pulse = 1500;              // 1.5ms 中位（安全初始）
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;

    // 初始化 CH3 (PC8) 和 CH4 (PC9)
    TIM_OC3Init(TIM3, &TIM_OCInitStruct);
    TIM_OC4Init(TIM3, &TIM_OCInitStruct);

    TIM_OC3PreloadConfig(TIM3, ENABLE);
    TIM_OC4PreloadConfig(TIM3, ENABLE);
    TIM_ARRPreloadConfig(TIM3, ENABLE);

    // 不要调用 TIM_CtrlPWMOutputs(TIM3, ...)，该接口用于高级定时器（TIM1/TIM8）
    TIM_Cmd(TIM3, ENABLE);
}

/* 普通舵机：角度 0..180 映射到 1.0ms..2.0ms （CH3: PC8） */
void SERVO_TEXT(uint16_t angle)
{
    if (angle > 180) angle = 180;
  uint32_t count = 500 + (uint32_t)angle * 2000 / 180; // 500~2500us
    TIM_SetCompare3(TIM3, (uint16_t)count);
}

/* 360°（连续旋转）舵机：CH4 (PC9)
   speed: -100 .. 0 .. +100
     -100 -> 最大反转（pulse 1000）
       0  -> 停止/中立（pulse 1500）
     +100 -> 最大正转（pulse 2000）
   注意：不同舵机中立/死区略有差异，建议校准。
*/
void SERVO360_SetSpeed(int16_t speed)
{
    if (speed > 100) speed = 100;
    if (speed < -100) speed = -100;
    int32_t pulse = 1500 + (int32_t)speed * 5; // speed *5 -> -500..+500
    TIM_SetCompare4(TIM3, (uint16_t)pulse);
}

/* 便捷停止（把脉冲回到中立） */
void SERVO360_Stop(void)
{
    TIM_SetCompare4(TIM3, 1500);
}
