#include "DMA.h"

void DMA_Config(uint32_t memory_address,uint16_t data_size)
{
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	//ͨ��4  ������7  -> USART1_TX
	DMA_InitTypeDef DMA_InitStruct;
	
	//����ͨ����
	DMA_InitStruct.DMA_Channel = DMA_Channel_4;
	//�������ݵ�ַ  -- ����1�ĵ�ַ (�Ĵ�����ַ)   �����ֲ� 26.3 �Ĵ�����Ѱ�ҵ�
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;
	//�洢��(�ڴ�)��ַ , ���õ�ʱ�򴫵ݽ�����
	DMA_InitStruct.DMA_Memory0BaseAddr = memory_address;
	//�������ݵķ��� �ڴ�->�ڴ�  �ڴ�->����  ����->�ڴ�
	DMA_InitStruct.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	//��������ݵĴ�С
	DMA_InitStruct.DMA_BufferSize = data_size;
	//�����ַ�Ƿ�������ģʽ
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	//�ڴ��ַ�Ƿ�������ģʽ
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	//һ�δ������ݵĳ���  8 / 16 / 32
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	//�ڴ洫�����ݵĳ���
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	//��ͨģʽ(�����ͣ), ѭ��ģʽ(�����������һ��)
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
	//�������ȼ�  
	DMA_InitStruct.DMA_Priority = DMA_Priority_VeryHigh;
	//�Ƿ���FIFO����  
	DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;
	//��FIFO���������,�ﵽ���ٵ�ʱ�� (1/4   1/2  3/4  1), �ſ�ʼ����
	//DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	//���͵Ľ���
	DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	
	DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	
	DMA_Init(DMA2_Stream7,&DMA_InitStruct);
	
	
}

