#include "Soft_IIC.h"
// ����ļ� ���൱�� �Լ���� stm32f4xx_iic.c

// SCL ʱ���� ���ģʽ
void SCL_Output_Mode(void)
{
	//RCC_AHB1PeriphClockCmd(SCL_CLK, ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;   //�������ģʽ   (IN ����)
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;  //���ģʽ    PP ����   OD ��© 
	GPIO_InitStruct.GPIO_Pin = SCL_PIN;      //���ź�
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;					 //����ģʽ  ����  UP  ���� DOWN  ���� NOPULL
	GPIO_InitStruct.GPIO_Speed = GPIO_Fast_Speed;  //����ٶ� 
	
	GPIO_Init(SCL_GROUP,&GPIO_InitStruct);

}

// SDA  ���ģʽ 
void SDA_Output_Mode(void)
{
	RCC_AHB1PeriphClockCmd(SDA_CLK, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;   //�������ģʽ   (IN ����)
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;  //���ģʽ    PP ����   OD ��© 
	GPIO_InitStruct.GPIO_Pin = SDA_PIN;      //���ź�
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;					 //����ģʽ  ����  UP  ���� DOWN  ���� NOPULL
	GPIO_InitStruct.GPIO_Speed = GPIO_Fast_Speed;  //����ٶ� 
	
	GPIO_Init(SDA_GROUP,&GPIO_InitStruct);
}

// SDA  ����ģʽ
void SDA_Input_Mode(void)
{
	RCC_AHB1PeriphClockCmd(SDA_CLK, ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;   //�������ģʽ   (IN ����)
	GPIO_InitStruct.GPIO_Pin = SDA_PIN;      //���ź�
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;						 //����ģʽ  ����  UP  ���� DOWN  ���� NOPULL

	GPIO_Init(SDA_GROUP,&GPIO_InitStruct);	
}

//��ʼ����      
void Soft_IIC_Config(void)
{
	// ������ �����ó� ���ģʽ
	SDA_Output_Mode();
	SCL_Output_Mode();
	
	// Ĭ�ϸߵ�ƽ
	IIC_SDA_SET(1);
	IIC_SCL_SET(1);

}

//��ʼͨѶ
void Soft_IIC_Start(void)
{
	SDA_Output_Mode();
	
	// Ĭ�ϸߵ�ƽ
	IIC_SDA_SET(1);
	IIC_SCL_SET(1);
	delay_us(5);
	
	IIC_SDA_SET(0);
	delay_us(5);
	IIC_SCL_SET(0);

}
//����ͨѶ
void Soft_IIC_Stop(void) 
{
	SDA_Output_Mode();
	
	// Ĭ�ϵ͵�ƽ
	IIC_SDA_SET(0);
	IIC_SCL_SET(0);
	delay_us(5);
	
	IIC_SCL_SET(1);
	delay_us(5);
	IIC_SDA_SET(1);
}

//��������
void Soft_IIC_Send(uint8_t data)
{
	uint8_t i;   //ѭ��

	
	SDA_Output_Mode();
	
	// Ĭ�ϵ͵�ƽ
	IIC_SDA_SET(0);
	IIC_SCL_SET(0);
	delay_us(5);
	
	for (i = 0; i < 8; i++) {
		// 1010 1011
		// 1000 0000   ��λ &   
	  // ��� == 0   ���λ0   != 0 ���λ��1 
		if (data & 0x80) 
			//���η� 1
			IIC_SDA_SET(1);
		else 
			//���η� 0
			IIC_SDA_SET(0);
		
		delay_us(5);
		
		//ʱ�������� �ӻ� �����ȡ���� 
		IIC_SCL_SET(1);
		// ��������1λ 
		data <<= 1;
		delay_us(5);  //���ӻ� �������ݵ�ʱ��
		
		IIC_SCL_SET(0);
		delay_us(5);
	}	

}

//�жϴӻ��Ƿ�ɹ����յ����� (�ӻ�Ӧ��  ACK)
uint8_t Soft_IIC_SlaveAck(void)
{
	uint8_t ack;
	SDA_Input_Mode();
	
	//��ʼ ʱ��������
	IIC_SCL_SET(0);
	delay_us(5);
	
	IIC_SCL_SET(1);
	delay_us(5);
	
	//��ȡ SDA�ߵ�����
	
	if (IIC_SDA_READ == Bit_RESET) {
		ack = 0;  //Ӧ��ɹ�
	} else {
		ack = 1;  //Ӧ��ʧ��
	}
	
	IIC_SCL_SET(0);
	delay_us(5);
	
	return ack;	
}



//��������
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

//�������ӻ�Ӧ��  ������д  �Ѿ��ӵ��ӻ������� ,�ܶ�ʱ�� �ж������� 

void Soft_IIC_MasterAck(uint8_t ack)
{
	SDA_Output_Mode();
	
	IIC_SCL_SET(0);
	IIC_SDA_SET(0);
	delay_us(5);
	
	if (ack == 0) 
		IIC_SDA_SET(0);  //Ӧ��ɹ�
	else
		IIC_SDA_SET(1);  //Ӧ��ʧ��
	delay_us(5);
	
	IIC_SCL_SET(1);
	delay_us(5);
	
	IIC_SCL_SET(0);
	delay_us(5);
	
}



