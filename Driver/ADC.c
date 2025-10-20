#include "ADC.h"

void ADC3_Config(void)
{

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE); 
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~����ADC���� PF7~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;  							  
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;      
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;						      				
	
	GPIO_Init(GPIOF,&GPIO_InitStruct);	
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~����ADC ��ʼ��~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	ADC_InitTypeDef ADC_InitStruct;
	
	//ADC�ķֱ��� , ��3.3v��ѹ �ֳɶ��ٷ� (2��12���ݷ�)
	ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;
	//�Ƿ���ɨ��ģʽ , ��ͨ�� ת��һ��ͨ�� , �Զ�ת��һ��ͨ��
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;
	//�Ƿ�������ת��
	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;
	//���ݶ��뷽ʽ 
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
	//ת��ͨ������
	ADC_InitStruct.ADC_NbrOfConversion = 1;
	
	ADC_Init(ADC3,&ADC_InitStruct);
	
	ADC_Cmd(ADC3,ENABLE);
}


//ADCת��   ����ֵ    1 2 3 4 5 *  3.3/2��12���� = V
uint16_t Get_Adc(ADC_TypeDef* ADCx,uint8_t ADC_Channel)
{
	//����ADC����ͨ��   ����:
	//1. ADC��
	//2. ͨ����
	//3. ����ɶ��ͨ��������Ҫת�� , ������������������
	//4. ÿ��ת���ȴ���������
	ADC_RegularChannelConfig(ADCx,ADC_Channel,1,ADC_SampleTime_480Cycles);
	
	ADC_SoftwareStartConv(ADCx);

	//�ȴ�ADCת������ 
	while(ADC_GetFlagStatus(ADCx,ADC_FLAG_EOC) != SET);
	
	//��ȡת����ֵ
  return	ADC_GetConversionValue(ADCx);
}

// ��������ʱ�� , �޳� ��������� 
// 10�� ƽ��ֵ 

uint16_t Get_Adc_Average(ADC_TypeDef* ADCx,uint8_t ADC_Channel,uint8_t times)
{
	uint8_t i = 0;
	uint16_t aver_value = 0;
	for (i = 0; i < times; i++) {
		aver_value += Get_Adc(ADCx,ADC_Channel);
		delay_ms(5);
	}
	return aver_value/times;
	
}

// ���ص�ѹֵ
float Get_Last_Value(ADC_TypeDef* ADCx,uint8_t ADC_Channel,uint8_t times)
{
		uint16_t adc_value;
		float adc_last;
		adc_value = Get_Adc_Average(ADCx,ADC_Channel,times);
		
		//    ����ֵ * derta = v
	  adc_last = (float)adc_value * (3.3/4096);
		
		return adc_last;
}






