#include "Soft_IIC.h"
// 这个文件 就相当于 自己封的 stm32f4xx_iic.c

// SCL 时钟线 输出模式
void SCL_Output_Mode(void)
{
	//RCC_AHB1PeriphClockCmd(SCL_CLK, ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;   //配置输出模式   (IN 输入)
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;  //输出模式    PP 推挽   OD 开漏 
	GPIO_InitStruct.GPIO_Pin = SCL_PIN;      //引脚号
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;					 //输入模式  上拉  UP  下拉 DOWN  浮空 NOPULL
	GPIO_InitStruct.GPIO_Speed = GPIO_Fast_Speed;  //输出速度 
	
	GPIO_Init(SCL_GROUP,&GPIO_InitStruct);

}

// SDA  输出模式 
void SDA_Output_Mode(void)
{
	RCC_AHB1PeriphClockCmd(SDA_CLK, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;   //配置输出模式   (IN 输入)
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;  //输出模式    PP 推挽   OD 开漏 
	GPIO_InitStruct.GPIO_Pin = SDA_PIN;      //引脚号
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;					 //输入模式  上拉  UP  下拉 DOWN  浮空 NOPULL
	GPIO_InitStruct.GPIO_Speed = GPIO_Fast_Speed;  //输出速度 
	
	GPIO_Init(SDA_GROUP,&GPIO_InitStruct);
}

// SDA  输入模式
void SDA_Input_Mode(void)
{
	RCC_AHB1PeriphClockCmd(SDA_CLK, ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;   //配置输出模式   (IN 输入)
	GPIO_InitStruct.GPIO_Pin = SDA_PIN;      //引脚号
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;						 //输入模式  上拉  UP  下拉 DOWN  浮空 NOPULL

	GPIO_Init(SDA_GROUP,&GPIO_InitStruct);	
}

//初始配置      
void Soft_IIC_Config(void)
{
	// 两根线 都设置成 输出模式
	SDA_Output_Mode();
	SCL_Output_Mode();
	
	// 默认高电平
	IIC_SDA_SET(1);
	IIC_SCL_SET(1);

}

//开始通讯
void Soft_IIC_Start(void)
{
	SDA_Output_Mode();
	
	// 默认高电平
	IIC_SDA_SET(1);
	IIC_SCL_SET(1);
	delay_us(5);
	
	IIC_SDA_SET(0);
	delay_us(5);
	IIC_SCL_SET(0);

}
//结束通讯
void Soft_IIC_Stop(void) 
{
	SDA_Output_Mode();
	
	// 默认低电平
	IIC_SDA_SET(0);
	IIC_SCL_SET(0);
	delay_us(5);
	
	IIC_SCL_SET(1);
	delay_us(5);
	IIC_SDA_SET(1);
}

//发送数据
void Soft_IIC_Send(uint8_t data)
{
	uint8_t i;   //循环

	
	SDA_Output_Mode();
	
	// 默认低电平
	IIC_SDA_SET(0);
	IIC_SCL_SET(0);
	delay_us(5);
	
	for (i = 0; i < 8; i++) {
		// 1010 1011
		// 1000 0000   按位 &   
	  // 结果 == 0   最高位0   != 0 最高位是1 
		if (data & 0x80) 
			//本次发 1
			IIC_SDA_SET(1);
		else 
			//本次发 0
			IIC_SDA_SET(0);
		
		delay_us(5);
		
		//时钟线拉高 从机 允许读取数据 
		IIC_SCL_SET(1);
		// 数据左移1位 
		data <<= 1;
		delay_us(5);  //给从机 接收数据的时间
		
		IIC_SCL_SET(0);
		delay_us(5);
	}	

}

//判断从机是否成功接收到数据 (从机应答  ACK)
uint8_t Soft_IIC_SlaveAck(void)
{
	uint8_t ack;
	SDA_Input_Mode();
	
	//初始 时钟线拉低
	IIC_SCL_SET(0);
	delay_us(5);
	
	IIC_SCL_SET(1);
	delay_us(5);
	
	//读取 SDA线的数据
	
	if (IIC_SDA_READ == Bit_RESET) {
		ack = 0;  //应答成功
	} else {
		ack = 1;  //应答失败
	}
	
	IIC_SCL_SET(0);
	delay_us(5);
	
	return ack;	
}



//接收数据
uint8_t Soft_IIC_Receive(void)
{
	uint8_t i;
	uint8_t data = 0;  //   00101101    1010 1001
	
	SDA_Input_Mode();
	
	IIC_SCL_SET(0);
	delay_us(5);
	
	for (i = 0; i < 8; i++) {
		
		IIC_SCL_SET(1);
		delay_us(5);
		
		data <<= 1;
		if (IIC_SDA_READ == Bit_SET){
			data |= 0x01;
		}
		
		IIC_SCL_SET(0);
		delay_us(5);
	}
	
	return data;
}

//主机给从机应答  经常不写  已经接到从机数据了 ,很多时候 中断链接了 

void Soft_IIC_MasterAck(uint8_t ack)
{
	SDA_Output_Mode();
	
	IIC_SCL_SET(0);
	IIC_SDA_SET(0);
	delay_us(5);
	
	if (ack == 0) 
		IIC_SDA_SET(0);  //应答成功
	else
		IIC_SDA_SET(1);  //应答失败
	delay_us(5);
	
	IIC_SCL_SET(1);
	delay_us(5);
	
	IIC_SCL_SET(0);
	delay_us(5);
	
}



