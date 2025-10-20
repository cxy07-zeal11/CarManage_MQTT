#ifndef __LIGHT_ADC_H__
#define __LIGHT_ADC_H__

/* 包含官方头文件 */
#include "stm32f4xx.h"

/* 相关变量声明 */
typedef struct
{
	unsigned short ohm; // 光敏电阻电阻值

	unsigned short lux; // 光照度

} PhotoRes_TypeDef;

extern uint16_t LUX; // 光照度

/* 外部接口函数声明 */
uint16_t GetLux(float PhotoResistor);

#endif
