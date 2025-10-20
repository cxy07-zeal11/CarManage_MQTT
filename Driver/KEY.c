#include "KEY.h"

/* Configure key GPIOs */
void KEY_Config(void)
{
    RCC_AHB1PeriphClockCmd(KEY_CLK, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;   // input
    GPIO_InitStruct.GPIO_Pin = KEY1_PIN | KEY2_PIN | KEY3_PIN | KEY4_PIN;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;   // pull-up
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(KEY_GROUP, &GPIO_InitStruct);
}

/* Scan one key (blocking, with debounce). Returns 1 if pressed, else 0 */
uint8_t KEY_Scanf(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    if (GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == Bit_RESET) {
        delay_ms(20); // short debounce (you had 100ms; 20ms is usually enough)
        if (GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == Bit_RESET) {
            // wait for release to avoid repeated triggers
            while (GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == Bit_RESET);
            delay_ms(20); // post-release debounce
            return 1;
        }
    }
    return 0;
}
