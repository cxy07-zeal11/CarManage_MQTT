#include "UART.h"
#include "VoiceControl.h"


//串口printf打印端口的选择
#define USARTX USART1
//#define USARTX USART2
//#define USARTX USART3


volatile uint8_t voice_cmd = 0;  // 全局命令缓存

void UART1_Config(uint32_t BaudRate)
{
	GPIO_InitTypeDef GPIO_InitStruct;	
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
													   
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;	//UART1_TXD
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_High_Speed; 
	GPIO_Init(GPIOA, &GPIO_InitStruct);	

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;	//UART1_RXD
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);	//设置复用功能
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
	
	USART_InitStruct.USART_BaudRate = BaudRate;	//波特率
	USART_InitStruct.USART_Parity	= USART_Parity_No;	//无检验
	USART_InitStruct.USART_WordLength	= USART_WordLength_8b;	//8位字长
	USART_InitStruct.USART_StopBits	= USART_StopBits_1;	//1位停止位
	USART_InitStruct.USART_HardwareFlowControl	= USART_HardwareFlowControl_None;	//无硬件流控
	USART_InitStruct.USART_Mode	= USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART1, &USART_InitStruct);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	//开启串口接收中断
	
	NVIC_InitStruct.NVIC_IRQChannel	= USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd	= ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority	= 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority	= 1;
	NVIC_Init(&NVIC_InitStruct);	//配置NVIC
	
	USART_Cmd(USART1, ENABLE);	//开启串口
}

/***串口二****/
void UART2_Config(uint32_t BaudRate)
{
	/* 定义相关类型的结构体 */
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

	/* 开启端口引脚时钟 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	/* 开启串口端口时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	/* 配置通信引脚工作方式 */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;		  // UART1_TXD
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;	  // 复用功能
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;	  // 推完模式
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;	  // 上拉模式
	GPIO_InitStruct.GPIO_Speed = GPIO_High_Speed; // 输出速度为高
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3; // UART1_RXD
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	/* 配置引脚的复用功能 */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

	/* 配置串口接口的工作方式 */
	USART_InitStruct.USART_BaudRate = BaudRate;									 // 波特率
	USART_InitStruct.USART_Parity = USART_Parity_No;							 // 无检验
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;					 // 8位字长
	USART_InitStruct.USART_StopBits = USART_StopBits_1;							 // 1位停止位
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 无硬件流控
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;				 // 收发模式
	USART_Init(USART2, &USART_InitStruct);

	/* 开启串口接收中断 */
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

	/* 配置串口的中断优先级 */
	NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);

	/* 开启串口工作 */
	USART_Cmd(USART2, ENABLE);
}

//串口3初始化
void UART3_Config(uint32_t BaudRate)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable GPIO clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	/* Enable USART clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	/* Connect USART pins to PB10 \ PB11*/
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);

	/* Configure USART Tx and Rx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOB, &GPIO_InitStructure);


	USART_InitStructure.USART_BaudRate = BaudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);


	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Enable USART */
	USART_Cmd(USART3, ENABLE);
	
	/* Enable the USARTz Receive Interrupt */
	USART_ITConfig(USART3, USART_IT_RXNE , ENABLE);
}



//重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
	/* 发送一个字节数据到串口 */
	USART_SendData(USARTX, (uint8_t) ch);

	/* 等待发送完毕 */
	while (USART_GetFlagStatus(USARTX, USART_FLAG_TXE) == RESET);		

	return (ch);
}

//重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
	/* 等待串口输入数据 */
	while (USART_GetFlagStatus(USARTX, USART_FLAG_RXNE) == RESET);

	return (int)USART_ReceiveData(USARTX);
}


char rxd_data;	//定义变量用于存放接收数据
char rxd_num;	//计数值
char rxd_str[512];	//存放字符串

void USART1_IRQHandler(void)  
{
	if( SET == USART_GetITStatus(USART1, USART_IT_RXNE) )
		{
			//清楚接收中断标志位
			USART_ClearITPendingBit(USART1, USART_IT_RXNE);
			
			//接收数据
			rxd_data = USART_ReceiveData(USART1);
			
			//不断把接收到的单个字节数据存放在数组
			rxd_str[rxd_num++] = rxd_data;
		}
		
		//判断单个字节数据
		if(rxd_data == 'A')
		{
			GPIO_ToggleBits(GPIOH, GPIO_Pin_11);	
		}
		
		//判断整个数组
		if( rxd_str[rxd_num-1] == '.')
		{
			if(0 == strcmp(rxd_str, "LED1_ON."))
			{
				GPIO_ToggleBits(GPIOH, GPIO_Pin_11);
			}
			
			//计数值清零
			rxd_num = 0;
			//清空数组
			memset(rxd_str, 0, sizeof(rxd_str));
		}
		
		//发送接收到的数据
		USART_SendData(USARTX, rxd_data);
		
}
void USART2_IRQHandler(void)
{
    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        // 读取语音模块发来的字节
        uint8_t rx = (uint8_t)USART_ReceiveData(USART2);
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);

        // 存到全局变量，供主循环处理
        voice_cmd = rx;

        // 同时通过 USART1 打印到调试串口
        Usart_SendByte(USART1, 0xFE);      // 可加起始标识，方便调试
        Usart_SendByte(USART1, rx);        // 实际接收到的指令
        Usart_SendByte(USART1, 0xFF);      // 可加结束标识
    }
}

//volatile uint32_t	Rx3Counter	= 0;
//volatile uint8_t	Rx3Data 	= 0;
//volatile uint8_t	Rx3End 		= 0;
//volatile uint8_t	Tx3Buffer[512]={0};
//volatile uint8_t	Rx3Buffer[512]={0};

//void USART3_IRQHandler(void)
//{
//	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
//	{
//		//清楚接收中断标志位
//			USART_ClearITPendingBit(USART3, USART_IT_RXNE);
//		/* Read one byte from the receive data register */
//		Rx3Data = USART_ReceiveData(USART3);	
//	}

//	/* 回显，把接收到的单个字节数据发送到 发送方*/
//	USART_SendData(USART1, Rx3Data);
//}


/*****************  发送一个字节 **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* 发送一个字节数据到USART */
	USART_SendData(pUSARTx,ch);
		
	/* 等待发送数据寄存器为空 */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
	//USART_ClearFlag(pUSARTx,USART_FLAG_TXE);
}

/*****************  发送指定长度的字节 **********************/
void Usart_SendBytes(USART_TypeDef * pUSARTx, uint8_t *buf,uint32_t len)
{
	uint8_t *p = buf;
	
	while(len--)
	{
		USART_SendData(pUSARTx,*p);
		
		p++;
		
		//等待数据发送成功
		while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE)==RESET);
		USART_ClearFlag(pUSARTx, USART_FLAG_TXE);
	}
}

/****************** 发送数组 ***********************
	mqtt  发CONNECT
  arr[0] = CONNETC指令  (101010101001)
	arr[1] = Qos1  (1010101)
	arr[2] = 协议名字
  arr[3] = 用户名
  arr[4] = 密码

	arr[3] = {'a','b','c'}
*/
void Usart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num)
{
	uint16_t i;

	for(i=0; i<num; i++)
	{
		/* 发送一个字节数据到USART */
		Usart_SendByte(pUSARTx,array[i]);	
	}
	/* 等待发送完成 */
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
	//USART_ClearFlag(pUSARTx,USART_FLAG_TC);
}


/*****************  发送字符串 **********************/
void Usart_SendString(USART_TypeDef *pUSARTx, char *str)
{
	unsigned int k = 0;
	do
	{
		Usart_SendByte(pUSARTx, *(str + k));
		k++;
	} while (*(str + k) != '\0');

	/* 等待发送完成 */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET);
}


/*****************  发送一个16位数 **********************/
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
{
	uint8_t temp_h, temp_l;

	/* 取出高八位 */
	temp_h = (ch&0XFF00)>>8;
	/* 取出低八位 */
	temp_l = ch&0XFF;

	/* 发送高八位 */
	USART_SendData(pUSARTx,temp_h);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);

	/* 发送低八位 */
	USART_SendData(pUSARTx,temp_l);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

