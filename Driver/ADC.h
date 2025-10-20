#ifndef _ADC_H
#define _ADC_H

#include "stm32f4xx.h"
#include "Delay.h"
#include <stdio.h>

void ADC3_Config(void);
uint16_t Get_Adc(ADC_TypeDef* ADCx,uint8_t ADC_Channel);
uint16_t Get_Adc_Average(ADC_TypeDef* ADCx,uint8_t ADC_Channel,uint8_t times);
float Get_Last_Value(ADC_TypeDef* ADCx,uint8_t ADC_Channel,uint8_t times);

#endif
