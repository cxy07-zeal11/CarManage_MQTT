#ifndef __UART_H
#define	__UART_H

#include "stm32f4xx.h"
#include <string.h>
#include <stdio.h>

void UART1_Config(uint32_t BaudRate);
void UART2_Config(uint32_t BaudRate);
void UART3_Config(uint32_t BaudRate);
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch);

void Usart_SendString(USART_TypeDef *pUSARTx, char *str);
void Usart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num);
void Usart_SendBytes(USART_TypeDef * pUSARTx, uint8_t *buf,uint32_t len);
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);

#endif
