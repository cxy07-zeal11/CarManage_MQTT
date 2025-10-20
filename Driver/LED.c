#include "LED.h"

void LED_Config(void)
{
	//点灯   GPIO的控制    PG14     低点亮   高熄灭    输出模式 
	//打开 GPIO的时钟信号 
	RCC_AHB1PeriphClockCmd(LED_CLK, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;   //配置输出模式   (IN 输入)
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;  //输出模式    PP 推挽   OD 开漏 
	GPIO_InitStruct.GPIO_Pin = LED1_PIN | LED2_PIN | LED3_PIN | LED4_PIN;      //引脚号
	//GPIO_InitStruct.GPIO_PuPd =   						 //输入模式  上拉  UP  下拉 DOWN  浮空 NOPULL
	GPIO_InitStruct.GPIO_Speed = GPIO_Fast_Speed;  //输出速度 
	
	
	GPIO_Init(LED_GROUP,&GPIO_InitStruct);
	
	//GPIO_SetBits()  高        GPIO_ResetBits()  低
	GPIO_SetBits(LED_GROUP,LED1_PIN | LED2_PIN | LED3_PIN | LED4_PIN);

}

//控制灯不同闪烁模式
void LED_Flash(void)
{
	GPIO_ResetBits(LED_GROUP,LED1_PIN  | LED3_PIN );
	GPIO_SetBits(LED_GROUP,LED2_PIN  | LED4_PIN );
	delay_s(1);
	GPIO_SetBits(LED_GROUP,LED1_PIN  | LED3_PIN );
	GPIO_ResetBits(LED_GROUP,LED2_PIN  | LED4_PIN );
	delay_s(1);
}

uint16_t leds[] = {LED1_PIN, LED2_PIN, LED3_PIN, LED4_PIN};

//跑马灯  4盏灯
void LED_Horse(void)
{
	//灯1 亮 灯1 灭 
	uint8_t i;
	for (i = 0; i < 4; i++) {
		GPIO_ResetBits(LED_GROUP,leds[i]);
		delay_ms(200);
		GPIO_SetBits(LED_GROUP,leds[i]);
	}
}


//流水灯
void LED_Water(void)
{
	
	//从头到尾的点亮
	uint8_t i;
	for (i = 0; i < 4; i++){
		delay_ms(100);
		GPIO_ResetBits(LED_GROUP,leds[i]);
	}
	
	//从头到尾的熄灭
	for (i = 0; i < 4; i++){
		delay_ms(100);
		GPIO_SetBits(LED_GROUP,leds[i]);
	}

}

