#ifndef __HALL_SENSOR_H
#define __HALL_SENSOR_H

#include "stm32f4xx.h"
#include "Delay.h"
// ����������״̬����
#define DOOR_CLOSED        0  // ��⵽�������͵�ƽ��
#define DOOR_NOT_CLOSED    1  // δ��⵽�������ߵ�ƽ��

// ��ʼ������������
void HallSensor_Init(void);

// ��ȡ����������״̬��ԭʼ��ȡ��
uint8_t HallSensor_ReadStatus(void);

// ��ȡ�ȶ��Ļ���������״̬����ȥ������
uint8_t HallSensor_ReadStableStatus(void);

#endif /* __HALL_SENSOR_H */