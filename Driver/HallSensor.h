#ifndef __HALL_SENSOR_H
#define __HALL_SENSOR_H

#include "stm32f4xx.h"
#include "Delay.h"
// 霍尔传感器状态定义
#define DOOR_CLOSED        0  // 检测到磁铁（低电平）
#define DOOR_NOT_CLOSED    1  // 未检测到磁铁（高电平）

// 初始化霍尔传感器
void HallSensor_Init(void);

// 读取霍尔传感器状态（原始读取）
uint8_t HallSensor_ReadStatus(void);

// 读取稳定的霍尔传感器状态（带去抖动）
uint8_t HallSensor_ReadStableStatus(void);

#endif /* __HALL_SENSOR_H */