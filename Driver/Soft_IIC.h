#ifndef _SOFT_IIC_H
#define _SOFT_IIC_H

#include "stm32f4xx.h"
#include "Delay.h"
#include <stdio.h>

#define SCL_CLK RCC_AHB1Periph_GPIOB
#define SCL_PIN GPIO_Pin_8
#define SCL_GROUP GPIOB


#define SDA_CLK RCC_AHB1Periph_GPIOB
#define SDA_PIN GPIO_Pin_9
#define SDA_GROUP GPIOB

//修改SDA电平
#define IIC_SDA_SET(n)   (n) ? (GPIO_SetBits(SDA_GROUP,SDA_PIN)) : (GPIO_ResetBits(SDA_GROUP,SDA_PIN))

//修改SCL电平
#define IIC_SCL_SET(n)   (n) ? (GPIO_SetBits(SCL_GROUP,SCL_PIN)) : (GPIO_ResetBits(SCL_GROUP,SCL_PIN))

//读取SDA的电平
#define IIC_SDA_READ GPIO_ReadInputDataBit(SDA_GROUP,SDA_PIN)

void SCL_Output_Mode(void);
void SDA_Output_Mode(void);
void SDA_Input_Mode(void);
void Soft_IIC_Config(void);
void Soft_IIC_Start(void);
void Soft_IIC_Stop(void);
void Soft_IIC_Send(uint8_t data);
uint8_t Soft_IIC_SlaveAck(void);
uint8_t Soft_IIC_Receive(void);
void Soft_IIC_MasterAck(uint8_t ack);

#endif
