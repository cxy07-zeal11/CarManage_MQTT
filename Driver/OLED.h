#ifndef __OLED_H
#define __OLED_H

#include "stm32f4xx.h"
#include "Delay.h"
#include <stdio.h>
#include "Soft_IIC.h"

// OLED I2C��ַ������ʵ��ģ���޸ģ�0x78��0x3C��
#define OLED_ADDR 0x78

// ����/��������
#define OLED_CMD  0x00
#define OLED_DATA 0x40

// ��������
void OLED_Config(void);                  // ��ʼ��OLED
void OLED_Clear(void);                 // ����
void OLED_WriteByte(uint8_t dat, uint8_t cmd);  // д���ֽڣ�����/���ݣ�
void OLED_SetPos(uint8_t x, uint8_t y); // ������ʾλ��
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr); // ��ʾ�����ַ�
void OLED_ShowString(uint8_t x, uint8_t y, uint8_t *str); // ��ʾ�ַ���

#endif
