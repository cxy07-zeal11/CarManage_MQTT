#include "Delay.h"

#include "stm32f4xx.h"

static __IO uint32_t uwTick;

void SysTick_Init(void)
{
    /* SystemFrequency / 1000    1ms�ж�һ�� */
    if (SysTick_Config(SystemCoreClock / 1000))
    {
        while (1);
    }
}

void SysTick_Handler(void)
{
    uwTick++;
}

uint32_t GetTick(void)
{
    return uwTick;
}

//��װһ����ʱ����   ��һ��û���κ��������   ���� 

void delay(volatile uint32_t cnt)
{
	while (cnt > 0) {
		cnt --;
	}
}

//΢������ʱ     delay_us(2)
void delay_us(uint32_t num)
{

	//��ʼ�� 
	SysTick->CTRL = 0;  //  00000000 10000000 00000000
	//װ�ص���ʱ������  1us ���� 21����    
	SysTick->LOAD = 21 * num - 1;
	//��ֹ�ܵ���һ��������Ӱ��
	SysTick->VAL = 0; 
	//ʹ��
	SysTick->CTRL = 1;
	
	//�����Ĺ�����  ���뿨ס 
	while((SysTick->CTRL & 0x10000) == 0);
	SysTick->CTRL = 0;
}

//����   700�����  
void delay_ms(uint32_t num)
{

	//��ʼ�� 
	SysTick->CTRL = 0;  //  00000000 10000000 00000000
	//װ�ص���ʱ������  1us ���� 21����    
	SysTick->LOAD = 21 * 1000 * num - 1;
	//��ֹ�ܵ���һ��������Ӱ��
	SysTick->VAL = 0; 
	//ʹ��
	SysTick->CTRL = 1;
	
	//�����Ĺ�����  ���뿨ס 
	while((SysTick->CTRL & 0x10000) == 0);
	SysTick->CTRL = 0;
}


void delay_s(uint32_t num)
{

	while (num--) {
		delay_ms(500);
		delay_ms(500);
	}
}

//  10 1 0 1010
//  ��λ��
//  00 1 0 0000
//  00 1 0 0000
