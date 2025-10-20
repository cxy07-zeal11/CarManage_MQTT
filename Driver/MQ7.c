#include "MQ7.h"


void MQ7_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    

    RCC_AHB1PeriphClockCmd(MQ7_DO_GPIO_CLK, ENABLE);
    

    GPIO_InitStructure.GPIO_Pin = MQ7_DO_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;       
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;     
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(MQ7_DO_GPIO_PORT, &GPIO_InitStructure);
}


uint8_t MQ7_ReadDO(void) {
  
    uint8_t pinState = GPIO_ReadInputDataBit(MQ7_DO_GPIO_PORT, MQ7_DO_GPIO_PIN);
    

    return (pinState == 0) ? 1 : 0;
}