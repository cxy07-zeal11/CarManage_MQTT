#include "BEEP.h"

void BEEP_Config(void) 
{
	// ռ�ձ�Խ��  ����Խ��   BEEP  PA5     TIM2  ͨ��1
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~����GPIO����~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	//���� PA5 ���ó� TIM2 ��ģʽ
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_TIM2);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;  							  //AF ���� **********
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;      //���ź�
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;						      //����
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;								//����    **********
	
	GPIO_Init(GPIOA,&GPIO_InitStruct);	
	
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~���ö�ʱ���Ļ����÷� ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	
	//����ģʽ
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	//���� ARR�Զ���װ��ֵ
	TIM_TimeBaseInitStruct.TIM_Period = 999;
	//���� PSC Ԥ��Ƶֵ
	TIM_TimeBaseInitStruct.TIM_Prescaler = 839;    //0.005���ӵĶ�ʱ��      10s �ж� ?? 
	//PSC ֮ǰ����һ�η�Ƶ
	//TIM_TimeBaseInitStruct.TIM_ClockDivision =   
	//��ʱ���ظ������Ĵ���   �߼���ʱ����Ч
	//TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 
	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~��������Ƚ� ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	
	TIM_OCInitTypeDef TIM_OCInitStruct;
	
	//PWM1ģʽ
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	//ʹ��
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	//����CCR�Ƚ�ֵ
	TIM_OCInitStruct.TIM_Pulse = 0;
	//�ߵ�ƽ��Ч
	TIM_OCInitStruct.TIM_OCPolarity= TIM_OCPolarity_High;
	
	TIM_OC1Init(TIM2, &TIM_OCInitStruct);
	
	//CCRԤװ��   ��Ҫ������λ�ö�̬�޸� CCR��ֵ��ʱ�� ,�����ȿ��� Ԥװ�� 
	TIM_OC1PreloadConfig(TIM2,ENABLE);
	
	TIM_Cmd(TIM2,ENABLE);
	
}