#include "EEPROM.h"

void EEPROM_Init()
{
	Soft_IIC_Config();
}


/*
* @brief ��EEPROM �洢һ���ַ�
* @param Ҫ��EEPROM ������ĸ���ַ��������
* @param Ҫ����ʲô����
* @retval �ɹ����� 0   Ѱַʧ�� 1  д���ַʧ�� 2  д������ʧ�� 3
*/
uint8_t EEPROM_WriteByte(uint8_t address,uint8_t data)
{
	//1. ������˵��ʼ
	Soft_IIC_Start();
	
	//2. ��������EEPROM��ַ
	Soft_IIC_Send(0xA0);       // 1010 0000
	if (Soft_IIC_SlaveAck() == 1) {
		//Ѱַʧ��
		Soft_IIC_Stop();
		printf("find address wrong \r\n");
		return 1;
	}
	
	//3. ��EEPROM ���� ���ĸ���ַ����д������
	Soft_IIC_Send(address);
	if (Soft_IIC_SlaveAck() == 1) {
		//Ѱַʧ��
		Soft_IIC_Stop();
		printf("find data address wrong \r\n");
		return 2;
	}
	
	//4. ����������������
	Soft_IIC_Send(data);
	if (Soft_IIC_SlaveAck() == 1) {
		//Ѱַʧ��
		Soft_IIC_Stop();
		printf("write data wrong \r\n");
		return 3;
	}
	
	// ��ok  ֹͣ���ε�ͨѶ
	Soft_IIC_Stop();
	printf("write ok");
	
	return 0;
}

// 256�ֽ�   ----  ƽ���ֳ����ɷ� ---- 8 

/*
* @brief ��EEPROM ҳд�� ����
* @param Ҫ��EEPROM ������ĸ���ַ��������
* @param Ҫ����ʲô����
* @retval �ɹ����� 0   Ѱַʧ�� 1  д���ַʧ�� 2  д������ʧ�� 3
*/

uint8_t EEPROM_WritePage(uint8_t address, uint8_t DataSize, uint8_t *DataPtr)
{
	//1. ������˵��ʼ
	Soft_IIC_Start();
	
	//2. ��������EEPROM��ַ
	Soft_IIC_Send(0xA0);       // 1010 0000
	if (Soft_IIC_SlaveAck() == 1) {
		//Ѱַʧ��
		Soft_IIC_Stop();
		printf("find address wrong \r\n");
		return 1;
	}
	
	//3. ��EEPROM ���� ���ĸ���ַ����д������
	Soft_IIC_Send(address);
	if (Soft_IIC_SlaveAck() == 1) {
		//Ѱַʧ��
		Soft_IIC_Stop();
		printf("find data address wrong \r\n");
		return 2;
	}
	
	while (DataSize--) {
		//����һ������ ,����ָ�����������ƶ�
		Soft_IIC_Send(*DataPtr++);
		if (Soft_IIC_SlaveAck() == 1) {
			//Ѱַʧ��
			Soft_IIC_Stop();
			printf("write data wrong \r\n");
			return 3;
		}
		
	}
	// ��ok  ֹͣ���ε�ͨѶ
	Soft_IIC_Stop();
	printf("write ok");
	
	return 0;
}

/*
* @brief ��EEPROM ָ����ַ��ȡ����
* @param Ҫ��EEPROM ������ĸ���ַ��ȡ����
* @retval �ɹ����� ����   Ѱַʧ�� 1  д���ַʧ�� 2  д������ʧ�� 3
*/
 
uint8_t EEPROM_ReadByte(uint8_t address)
{
	uint8_t data = 0; //�������ݵı���
	
	//1. ������˵��ʼ
	Soft_IIC_Start();
	
	//2. ��������EEPROM��ַ
	Soft_IIC_Send(0xA0);       // 1010 0000
	if (Soft_IIC_SlaveAck() == 1) {
		//Ѱַʧ��
		Soft_IIC_Stop();
		printf("find address wrong \r\n");
		return 1;
	}
	
	//3. ��EEPROM ���� ���ĸ���ַ����д������
	Soft_IIC_Send(address);
	if (Soft_IIC_SlaveAck() == 1) {
		//Ѱַʧ��
		Soft_IIC_Stop();
		printf("find data address wrong \r\n");
		return 2;
	}
	
	Soft_IIC_Stop();
	Soft_IIC_Start();
	// I2C��Э��涨 , ��д֮����л� ,��������һ��
	//2. ��������EEPROM��ַ
	Soft_IIC_Send(0xA1);       // 1010 0001
	if (Soft_IIC_SlaveAck() == 1) {
		//Ѱַʧ��
		Soft_IIC_Stop();
		printf("find address wrong \r\n");
		return 1;
	}
	
	//��ȡһ���ַ�
	data  = Soft_IIC_Receive();
	
	//Soft_IIC_MasterAck(0x00);
	
	Soft_IIC_Stop();
	
	return data;
}

//���� ����

void EEPROM_RW(void) 
{

	uint8_t data;
	/*if (EEPROM_WriteByte(0x00,'a') != 0) {
		printf("д��ʧ�� \r\n");
	}
	delay_ms(300);
	*/
	data = EEPROM_ReadByte(0x00);
	printf("��ȡ��������λ: %c \r\n",data);
	
  delay_ms(300);

}







