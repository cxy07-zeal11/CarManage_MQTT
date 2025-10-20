#include "EXTI.h"

// 引脚 高低电平的切换 产生中断   ----- EXTI中断线控制 
// 今天 , 按键的中断 
void EXTI_Config(void)
{
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~GPIO配置~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	RCC_AHB1PeriphClockCmd(KEY_CLK, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;   //配置输出模式   (IN 输入)
	GPIO_InitStruct.GPIO_Pin = KEY1_PIN | KEY2_PIN | KEY3_PIN | KEY4_PIN;      //引脚号
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;						 //输入模式  上拉  UP  下拉 DOWN  浮空 NOPULL

	GPIO_Init(KEY_GROUP,&GPIO_InitStruct);	

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~把引脚链接道中断线上~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOG,EXTI_PinSource2);  // PG2 连接到 EXTI2
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOG, EXTI_PinSource3);  // PG2 连接到 EXTI2
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOG,EXTI_PinSource4);  // PG2 连接到 EXTI2
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOG,EXTI_PinSource5);  // PG2 连接到 EXTI2
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~把引脚链接道中断线上~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	EXTI_InitTypeDef EXTI_InitStruct;
	
	EXTI_InitStruct.EXTI_Line = EXTI_Line2;									//哪根中断线
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;									//使能中断
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;				//触发中断 / 事件(Event)
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;    //下降沿触发
	
	EXTI_Init(&EXTI_InitStruct);
	
	EXTI_InitStruct.EXTI_Line = EXTI_Line3;									//哪根中断线
	EXTI_Init(&EXTI_InitStruct);
	
	EXTI_InitStruct.EXTI_Line = EXTI_Line4;									//哪根中断线
	EXTI_Init(&EXTI_InitStruct);
	
	EXTI_InitStruct.EXTI_Line = EXTI_Line5;									//哪根中断线
	EXTI_Init(&EXTI_InitStruct);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~利用NVIC配置中断的优先级~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	
	//设置NVIC的分组 , 道 misc.c里面找到的
	
	
	NVIC_InitTypeDef NVIC_InitStruct;
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI2_IRQn;						//中断通道   stm32f4xx.h 里面找到
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;						//使能
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;   //抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;					 //响应优先级 
	
	NVIC_Init(&NVIC_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI3_IRQn;						//中断通道   stm32f4xx.h 里面找到
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;						//使能
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;   //抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;					 //响应优先级 
	
	NVIC_Init(&NVIC_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI4_IRQn;						//中断通道   stm32f4xx.h 里面找到
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;						//使能
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;   //抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;					 //响应优先级 
	
	NVIC_Init(&NVIC_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI9_5_IRQn;						//中断通道   stm32f4xx.h 里面找到
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;						//使能
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;   //抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;					 //响应优先级 
	
	NVIC_Init(&NVIC_InitStruct);
	
}


//所有的中断  对应 一个 中断函数 
//当触发了中断的那一时刻 , 自动调用对应中断函数 
//所有中断函数 都在 startup_stm32f40.....c
//闹钟 手动关闭闹钟
uint8_t key1_flag = 0;
uint8_t key2_flag = 0;
void EXTI2_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line2)) {
		//清除中断
		EXTI_ClearITPendingBit(EXTI_Line2);
		key1_flag = 1;
	}
}

void EXTI3_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line3)) {
		//清除中断
		EXTI_ClearITPendingBit(EXTI_Line3);
		key2_flag = 1;
	}
}

