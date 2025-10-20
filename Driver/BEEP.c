#include "BEEP.h"

void BEEP_Config(void) 
{
	// 占空比越大  声音越大   BEEP  PA5     TIM2  通道1
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~配置GPIO引脚~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	//设置 PA5 复用成 TIM2 的模式
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_TIM2);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;  							  //AF 复用 **********
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;      //引脚号
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;						      //上拉
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;								//推挽    **********
	
	GPIO_Init(GPIOA,&GPIO_InitStruct);	
	
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~配置定时器的基本用法 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	
	//计数模式
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	//设置 ARR自动重装载值
	TIM_TimeBaseInitStruct.TIM_Period = 999;
	//设置 PSC 预分频值
	TIM_TimeBaseInitStruct.TIM_Prescaler = 839;    //0.005秒钟的定时器      10s 中断 ?? 
	//PSC 之前先做一次分频
	//TIM_TimeBaseInitStruct.TIM_ClockDivision =   
	//定时器重复计数的次数   高级定时器生效
	//TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 
	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~配置输出比较 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	
	TIM_OCInitTypeDef TIM_OCInitStruct;
	
	//PWM1模式
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	//使能
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	//设置CCR比较值
	TIM_OCInitStruct.TIM_Pulse = 0;
	//高电平有效
	TIM_OCInitStruct.TIM_OCPolarity= TIM_OCPolarity_High;
	
	TIM_OC1Init(TIM2, &TIM_OCInitStruct);
	
	//CCR预装载   需要在其他位置动态修改 CCR的值的时候 ,必须先开启 预装载 
	TIM_OC1PreloadConfig(TIM2,ENABLE);
	
	TIM_Cmd(TIM2,ENABLE);
	
}