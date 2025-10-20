#ifndef _EEPROM_H
#define _EEPROM_H

#include "stm32f4xx.h"
#include "Delay.h"
#include <stdio.h>
#include "Soft_IIC.h"


void EEPROM_Init();
uint8_t EEPROM_WriteByte(uint8_t address,uint8_t data);
uint8_t EEPROM_WritePage(uint8_t address, uint8_t DataSize, uint8_t *DataPtr);
uint8_t EEPROM_ReadByte(uint8_t address);
void EEPROM_RW(void);

#endif
