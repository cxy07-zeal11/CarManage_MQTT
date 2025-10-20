#include "ADC.h"

void ADC3_Config(void)
{

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE); 
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~配置ADC引脚 PF7~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;  							  
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;      
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;						      				
	
	GPIO_Init(GPIOF,&GPIO_InitStruct);	
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~配置ADC 初始化~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	ADC_InitTypeDef ADC_InitStruct;
	
	//ADC的分辨率 , 把3.3v电压 分成多少分 (2的12次幂份)
	ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;
	//是否开启扫描模式 , 多通道 转完一个通道 , 自动转下一个通道
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;
	//是否开启连续转换
	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;
	//数据对齐方式 
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
	//转换通道个数
	ADC_InitStruct.ADC_NbrOfConversion = 1;
	
	ADC_Init(ADC3,&ADC_InitStruct);
	
	ADC_Cmd(ADC3,ENABLE);
}


//ADC转换   量化值    1 2 3 4 5 *  3.3/2的12次幂 = V
uint16_t Get_Adc(ADC_TypeDef* ADCx,uint8_t ADC_Channel)
{
	//配置ADC常规通道   参数:
	//1. ADC几
	//2. 通道号
	//3. 如果由多个通道内容需要转换 , 第三个参数就是排序
	//4. 每次转换等待几个周期
	ADC_RegularChannelConfig(ADCx,ADC_Channel,1,ADC_SampleTime_480Cycles);
	
	ADC_SoftwareStartConv(ADCx);

	//等待ADC转换结束 
	while(ADC_GetFlagStatus(ADCx,ADC_FLAG_EOC) != SET);
	
	//读取转换的值
  return	ADC_GetConversionValue(ADCx);
}

// 环境检测的时候 , 剔除 问题的数据 
// 10次 平均值 

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

// 返回电压值
float Get_Last_Value(ADC_TypeDef* ADCx,uint8_t ADC_Channel,uint8_t times)
{
		uint16_t adc_value;
		float adc_last;
		adc_value = Get_Adc_Average(ADCx,ADC_Channel,times);
		
		//    量化值 * derta = v
	  adc_last = (float)adc_value * (3.3/4096);
		
		return adc_last;
}






