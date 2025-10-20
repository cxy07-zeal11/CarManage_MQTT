#include "EXTI.h"

// ���� �ߵ͵�ƽ���л� �����ж�   ----- EXTI�ж��߿��� 
// ���� , �������ж� 
void EXTI_Config(void)
{
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~GPIO����~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	RCC_AHB1PeriphClockCmd(KEY_CLK, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;   //�������ģʽ   (IN ����)
	GPIO_InitStruct.GPIO_Pin = KEY1_PIN | KEY2_PIN | KEY3_PIN | KEY4_PIN;      //���ź�
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;						 //����ģʽ  ����  UP  ���� DOWN  ���� NOPULL

	GPIO_Init(KEY_GROUP,&GPIO_InitStruct);	

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~���������ӵ��ж�����~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOG,EXTI_PinSource2);  // PG2 ���ӵ� EXTI2
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOG, EXTI_PinSource3);  // PG2 ���ӵ� EXTI2
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOG,EXTI_PinSource4);  // PG2 ���ӵ� EXTI2
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOG,EXTI_PinSource5);  // PG2 ���ӵ� EXTI2
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~���������ӵ��ж�����~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	EXTI_InitTypeDef EXTI_InitStruct;
	
	EXTI_InitStruct.EXTI_Line = EXTI_Line2;									//�ĸ��ж���
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;									//ʹ���ж�
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;				//�����ж� / �¼�(Event)
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;    //�½��ش���
	
	EXTI_Init(&EXTI_InitStruct);
	
	EXTI_InitStruct.EXTI_Line = EXTI_Line3;									//�ĸ��ж���
	EXTI_Init(&EXTI_InitStruct);
	
	EXTI_InitStruct.EXTI_Line = EXTI_Line4;									//�ĸ��ж���
	EXTI_Init(&EXTI_InitStruct);
	
	EXTI_InitStruct.EXTI_Line = EXTI_Line5;									//�ĸ��ж���
	EXTI_Init(&EXTI_InitStruct);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~����NVIC�����жϵ����ȼ�~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	
	//����NVIC�ķ��� , �� misc.c�����ҵ���
	
	
	NVIC_InitTypeDef NVIC_InitStruct;
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI2_IRQn;						//�ж�ͨ��   stm32f4xx.h �����ҵ�
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;						//ʹ��
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;   //��ռ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;					 //��Ӧ���ȼ� 
	
	NVIC_Init(&NVIC_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI3_IRQn;						//�ж�ͨ��   stm32f4xx.h �����ҵ�
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;						//ʹ��
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;   //��ռ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;					 //��Ӧ���ȼ� 
	
	NVIC_Init(&NVIC_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI4_IRQn;						//�ж�ͨ��   stm32f4xx.h �����ҵ�
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;						//ʹ��
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;   //��ռ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;					 //��Ӧ���ȼ� 
	
	NVIC_Init(&NVIC_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI9_5_IRQn;						//�ж�ͨ��   stm32f4xx.h �����ҵ�
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;						//ʹ��
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;   //��ռ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;					 //��Ӧ���ȼ� 
	
	NVIC_Init(&NVIC_InitStruct);
	
}


//���е��ж�  ��Ӧ һ�� �жϺ��� 
//���������жϵ���һʱ�� , �Զ����ö�Ӧ�жϺ��� 
//�����жϺ��� ���� startup_stm32f40.....c
//���� �ֶ��ر�����
uint8_t key1_flag = 0;
uint8_t key2_flag = 0;
void EXTI2_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line2)) {
		//����ж�
		EXTI_ClearITPendingBit(EXTI_Line2);
		key1_flag = 1;
	}
}

void EXTI3_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line3)) {
		//����ж�
		EXTI_ClearITPendingBit(EXTI_Line3);
		key2_flag = 1;
	}
}

