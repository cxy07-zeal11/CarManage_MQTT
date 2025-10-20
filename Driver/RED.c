#include "RED.h"

void RED_Config(void)
{
    // 使能GPIOB时钟
    RCC_AHB1PeriphClockCmd(RED_CLK, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStruct;
    
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_Pin = RED_PIN;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz; // 使用标准速度常量
    
    GPIO_Init(RED_GROUP, &GPIO_InitStruct);
}