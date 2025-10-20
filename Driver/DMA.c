#include "DMA.h"

void DMA_Config(uint32_t memory_address,uint16_t data_size)
{
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	//通道4  数据流7  -> USART1_TX
	DMA_InitTypeDef DMA_InitStruct;
	
	//配置通道号
	DMA_InitStruct.DMA_Channel = DMA_Channel_4;
	//外设数据地址  -- 串口1的地址 (寄存器地址)   中文手册 26.3 寄存器中寻找的
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;
	//存储器(内存)地址 , 调用的时候传递进来的
	DMA_InitStruct.DMA_Memory0BaseAddr = memory_address;
	//搬运数据的方向 内存->内存  内存->外设  外设->内存
	DMA_InitStruct.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	//传输的数据的大小
	DMA_InitStruct.DMA_BufferSize = data_size;
	//外设地址是否开启自增模式
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	//内存地址是否开启自增模式
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	//一次传输数据的长度  8 / 16 / 32
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	//内存传输数据的长度
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	//普通模式(发完就停), 循环模式(发完继续发下一次)
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
	//设置优先级  
	DMA_InitStruct.DMA_Priority = DMA_Priority_VeryHigh;
	//是否开启FIFO缓存  
	DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;
	//当FIFO里面的数据,达到多少的时候 (1/4   1/2  3/4  1), 才开始传输
	//DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	//传送的节拍
	DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	
	DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	
	DMA_Init(DMA2_Stream7,&DMA_InitStruct);
	
	
}

