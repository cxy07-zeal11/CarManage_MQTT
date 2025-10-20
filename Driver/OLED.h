#ifndef __OLED_H
#define __OLED_H

#include "stm32f4xx.h"
#include "Delay.h"
#include <stdio.h>
#include "Soft_IIC.h"

// OLED I2C地址（根据实际模块修改，0x78或0x3C）
#define OLED_ADDR 0x78

// 命令/数据区分
#define OLED_CMD  0x00
#define OLED_DATA 0x40

// 函数声明
void OLED_Config(void);                  // 初始化OLED
void OLED_Clear(void);                 // 清屏
void OLED_WriteByte(uint8_t dat, uint8_t cmd);  // 写入字节（命令/数据）
void OLED_SetPos(uint8_t x, uint8_t y); // 设置显示位置
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr); // 显示单个字符
void OLED_ShowString(uint8_t x, uint8_t y, uint8_t *str); // 显示字符串

#endif
