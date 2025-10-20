#ifndef __DHT11_H
#define __DHT11_H

//包含头文件
#include "stm32f4xx.h"
#include "DELAY.h"
#include "uart.h"
#include <stdio.h>
#include <stdbool.h>

//宏定义


//变量声明
//typedef struct
//{
//	uint8_t  humi_int;			//湿度的整数部分
//	uint8_t  humi_deci;	 		//湿度的小数部分
//	uint8_t  temp_int;	 		//温度的整数部分
//	uint8_t  temp_deci;	 		//温度的小数部分
//	uint8_t  check_sum;	 		//校验和
//}DHT11_Data_TypeDef;


struct DHT11_Complete_Data_TypeDef
{
	float humi;    //湿度的完整数据
	float temp;	   //温度的完整数据
};



extern struct DHT11_Complete_Data_TypeDef DHT11_Data;	//温湿度结构体

//函数声明
void DHT11_Read_Data(void);
struct DHT11_Complete_Data_TypeDef Get_DHT11_Data(void);

#endif
