#include "RED.h"

void RED_Config(void)
{
    // ʹ��GPIOBʱ��
    RCC_AHB1PeriphClockCmd(RED_CLK, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStruct;
    
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_Pin = RED_PIN;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz; // ʹ�ñ�׼�ٶȳ���
    
    GPIO_Init(RED_GROUP, &GPIO_InitStruct);
}