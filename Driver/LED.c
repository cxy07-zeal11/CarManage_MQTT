#include "LED.h"

void LED_Config(void)
{
	//���   GPIO�Ŀ���    PG14     �͵���   ��Ϩ��    ���ģʽ 
	//�� GPIO��ʱ���ź� 
	RCC_AHB1PeriphClockCmd(LED_CLK, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;   //�������ģʽ   (IN ����)
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;  //���ģʽ    PP ����   OD ��© 
	GPIO_InitStruct.GPIO_Pin = LED1_PIN | LED2_PIN | LED3_PIN | LED4_PIN;      //���ź�
	//GPIO_InitStruct.GPIO_PuPd =   						 //����ģʽ  ����  UP  ���� DOWN  ���� NOPULL
	GPIO_InitStruct.GPIO_Speed = GPIO_Fast_Speed;  //����ٶ� 
	
	
	GPIO_Init(LED_GROUP,&GPIO_InitStruct);
	
	//GPIO_SetBits()  ��        GPIO_ResetBits()  ��
	GPIO_SetBits(LED_GROUP,LED1_PIN | LED2_PIN | LED3_PIN | LED4_PIN);

}

//���ƵƲ�ͬ��˸ģʽ
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

//�����  4յ��
void LED_Horse(void)
{
	//��1 �� ��1 �� 
	uint8_t i;
	for (i = 0; i < 4; i++) {
		GPIO_ResetBits(LED_GROUP,leds[i]);
		delay_ms(200);
		GPIO_SetBits(LED_GROUP,leds[i]);
	}
}


//��ˮ��
void LED_Water(void)
{
	
	//��ͷ��β�ĵ���
	uint8_t i;
	for (i = 0; i < 4; i++){
		delay_ms(100);
		GPIO_ResetBits(LED_GROUP,leds[i]);
	}
	
	//��ͷ��β��Ϩ��
	for (i = 0; i < 4; i++){
		delay_ms(100);
		GPIO_SetBits(LED_GROUP,leds[i]);
	}

}

