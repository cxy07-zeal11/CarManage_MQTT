#include "Delay.h"

#include "stm32f4xx.h"

static __IO uint32_t uwTick;

void SysTick_Init(void)
{
    /* SystemFrequency / 1000    1ms中断一次 */
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

//封装一个延时函数   整一个没有任何意义的事   数数 

void delay(volatile uint32_t cnt)
{
	while (cnt > 0) {
		cnt --;
	}
}

//微妙级别的延时     delay_us(2)
void delay_us(uint32_t num)
{

	//初始化 
	SysTick->CTRL = 0;  //  00000000 10000000 00000000
	//装载倒计时的数字  1us 能数 21个数    
	SysTick->LOAD = 21 * num - 1;
	//防止受到上一次数数的影响
	SysTick->VAL = 0; 
	//使能
	SysTick->CTRL = 1;
	
	//数数的过程中  代码卡住 
	while((SysTick->CTRL & 0x10000) == 0);
	SysTick->CTRL = 0;
}

//毫秒   700多毫秒  
void delay_ms(uint32_t num)
{

	//初始化 
	SysTick->CTRL = 0;  //  00000000 10000000 00000000
	//装载倒计时的数字  1us 能数 21个数    
	SysTick->LOAD = 21 * 1000 * num - 1;
	//防止受到上一次数数的影响
	SysTick->VAL = 0; 
	//使能
	SysTick->CTRL = 1;
	
	//数数的过程中  代码卡住 
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
//  按位与
//  00 1 0 0000
//  00 1 0 0000
