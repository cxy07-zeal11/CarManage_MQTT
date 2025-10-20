#ifndef __DHT11_H
#define __DHT11_H

//����ͷ�ļ�
#include "stm32f4xx.h"
#include "DELAY.h"
#include "uart.h"
#include <stdio.h>
#include <stdbool.h>

//�궨��


//��������
//typedef struct
//{
//	uint8_t  humi_int;			//ʪ�ȵ���������
//	uint8_t  humi_deci;	 		//ʪ�ȵ�С������
//	uint8_t  temp_int;	 		//�¶ȵ���������
//	uint8_t  temp_deci;	 		//�¶ȵ�С������
//	uint8_t  check_sum;	 		//У���
//}DHT11_Data_TypeDef;


struct DHT11_Complete_Data_TypeDef
{
	float humi;    //ʪ�ȵ���������
	float temp;	   //�¶ȵ���������
};



extern struct DHT11_Complete_Data_TypeDef DHT11_Data;	//��ʪ�Ƚṹ��

//��������
void DHT11_Read_Data(void);
struct DHT11_Complete_Data_TypeDef Get_DHT11_Data(void);

#endif
