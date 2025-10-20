#include "EEPROM.h"

void EEPROM_Init()
{
	Soft_IIC_Config();
}


/*
* @brief 向EEPROM 存储一个字符
* @param 要往EEPROM 里面的哪个地址存入数据
* @param 要存入什么数据
* @retval 成功返回 0   寻址失败 1  写入地址失败 2  写入数据失败 3
*/
uint8_t EEPROM_WriteByte(uint8_t address,uint8_t data)
{
	//1. 主机先说开始
	Soft_IIC_Start();
	
	//2. 主机发送EEPROM地址
	Soft_IIC_Send(0xA0);       // 1010 0000
	if (Soft_IIC_SlaveAck() == 1) {
		//寻址失败
		Soft_IIC_Stop();
		printf("find address wrong \r\n");
		return 1;
	}
	
	//3. 向EEPROM 发送 往哪个地址里面写入内容
	Soft_IIC_Send(address);
	if (Soft_IIC_SlaveAck() == 1) {
		//寻址失败
		Soft_IIC_Stop();
		printf("find data address wrong \r\n");
		return 2;
	}
	
	//4. 发送真正的内容了
	Soft_IIC_Send(data);
	if (Soft_IIC_SlaveAck() == 1) {
		//寻址失败
		Soft_IIC_Stop();
		printf("write data wrong \r\n");
		return 3;
	}
	
	// 都ok  停止本次的通讯
	Soft_IIC_Stop();
	printf("write ok");
	
	return 0;
}

// 256字节   ----  平均分成若干份 ---- 8 

/*
* @brief 向EEPROM 页写入 数据
* @param 要往EEPROM 里面的哪个地址存入数据
* @param 要存入什么数据
* @retval 成功返回 0   寻址失败 1  写入地址失败 2  写入数据失败 3
*/

uint8_t EEPROM_WritePage(uint8_t address, uint8_t DataSize, uint8_t *DataPtr)
{
	//1. 主机先说开始
	Soft_IIC_Start();
	
	//2. 主机发送EEPROM地址
	Soft_IIC_Send(0xA0);       // 1010 0000
	if (Soft_IIC_SlaveAck() == 1) {
		//寻址失败
		Soft_IIC_Stop();
		printf("find address wrong \r\n");
		return 1;
	}
	
	//3. 向EEPROM 发送 往哪个地址里面写入内容
	Soft_IIC_Send(address);
	if (Soft_IIC_SlaveAck() == 1) {
		//寻址失败
		Soft_IIC_Stop();
		printf("find data address wrong \r\n");
		return 2;
	}
	
	while (DataSize--) {
		//发送一个数据 ,并让指针依次往下移动
		Soft_IIC_Send(*DataPtr++);
		if (Soft_IIC_SlaveAck() == 1) {
			//寻址失败
			Soft_IIC_Stop();
			printf("write data wrong \r\n");
			return 3;
		}
		
	}
	// 都ok  停止本次的通讯
	Soft_IIC_Stop();
	printf("write ok");
	
	return 0;
}

/*
* @brief 向EEPROM 指定地址读取数据
* @param 要从EEPROM 里面的哪个地址读取数据
* @retval 成功返回 数据   寻址失败 1  写入地址失败 2  写入数据失败 3
*/
 
uint8_t EEPROM_ReadByte(uint8_t address)
{
	uint8_t data = 0; //保存数据的变量
	
	//1. 主机先说开始
	Soft_IIC_Start();
	
	//2. 主机发送EEPROM地址
	Soft_IIC_Send(0xA0);       // 1010 0000
	if (Soft_IIC_SlaveAck() == 1) {
		//寻址失败
		Soft_IIC_Stop();
		printf("find address wrong \r\n");
		return 1;
	}
	
	//3. 向EEPROM 发送 往哪个地址里面写入内容
	Soft_IIC_Send(address);
	if (Soft_IIC_SlaveAck() == 1) {
		//寻址失败
		Soft_IIC_Stop();
		printf("find data address wrong \r\n");
		return 2;
	}
	
	Soft_IIC_Stop();
	Soft_IIC_Start();
	// I2C的协议规定 , 读写之间的切换 ,必须重启一下
	//2. 主机发送EEPROM地址
	Soft_IIC_Send(0xA1);       // 1010 0001
	if (Soft_IIC_SlaveAck() == 1) {
		//寻址失败
		Soft_IIC_Stop();
		printf("find address wrong \r\n");
		return 1;
	}
	
	//读取一个字符
	data  = Soft_IIC_Receive();
	
	//Soft_IIC_MasterAck(0x00);
	
	Soft_IIC_Stop();
	
	return data;
}

//测试 运行

void EEPROM_RW(void) 
{

	uint8_t data;
	/*if (EEPROM_WriteByte(0x00,'a') != 0) {
		printf("写入失败 \r\n");
	}
	delay_ms(300);
	*/
	data = EEPROM_ReadByte(0x00);
	printf("读取到的内容位: %c \r\n",data);
	
  delay_ms(300);

}







