#include "OLED.h"
#include "OLED_Font.h"

// 初始化OLED
void OLED_Config(void) 
{
	Soft_IIC_Config();  // 初始化自定义I2C
	
	delay_ms(100);      // 上电延时
    
  // 发送初始化命令
  OLED_WriteByte(0xAE, OLED_CMD);	//关闭显示
	
	OLED_WriteByte(0xD5, OLED_CMD);	//设置显示时钟分频比/振荡器频率
	OLED_WriteByte(0x80, OLED_CMD);
	
	OLED_WriteByte(0xA8, OLED_CMD);	//设置多路复用率
	OLED_WriteByte(0x3F, OLED_CMD);
	
	OLED_WriteByte(0xD3, OLED_CMD);	//设置显示偏移
	OLED_WriteByte(0x00, OLED_CMD);
	
	OLED_WriteByte(0x40, OLED_CMD);	//设置显示开始行
	
	OLED_WriteByte(0xA1, OLED_CMD);	//设置左右方向，0xA1正常 0xA0左右反置
	
	OLED_WriteByte(0xC8, OLED_CMD);	//设置上下方向，0xC8正常 0xC0上下反置

	OLED_WriteByte(0xDA, OLED_CMD);	//设置COM引脚硬件配置
	OLED_WriteByte(0x12, OLED_CMD);
	
	OLED_WriteByte(0x81, OLED_CMD);	//设置对比度控制
	OLED_WriteByte(0xCF, OLED_CMD);

	OLED_WriteByte(0xD9, OLED_CMD);	//设置预充电周期
	OLED_WriteByte(0xF1, OLED_CMD);

	OLED_WriteByte(0xDB, OLED_CMD);	//设置VCOMH取消选择级别
	OLED_WriteByte(0x30, OLED_CMD);

	OLED_WriteByte(0xA4, OLED_CMD);	//设置整个显示打开/关闭

	OLED_WriteByte(0xA6, OLED_CMD);	//设置正常/倒转显示

	OLED_WriteByte(0x8D, OLED_CMD);	//设置充电泵
	OLED_WriteByte(0x14, OLED_CMD);

	OLED_WriteByte(0xAF, OLED_CMD);	//开启显示
    
    OLED_Clear();       // 清屏
}

// 写入一个字节（命令或数据）
void OLED_WriteByte(uint8_t dat, uint8_t cmd) {
    Soft_IIC_Start();               // 开始I2C通信
    Soft_IIC_Send(OLED_ADDR);       // 发送OLED地址
    Soft_IIC_SlaveAck();            // 等待从机应答
    
    Soft_IIC_Send(cmd);             // 发送命令/数据标志
    Soft_IIC_SlaveAck();            // 等待应答
    
    Soft_IIC_Send(dat);             // 发送实际数据
    Soft_IIC_SlaveAck();            // 等待应答
    
    Soft_IIC_Stop();                // 结束通信
}

// 设置显示位置（x:0-127, y:0-7，y代表页地址）
void OLED_SetPos(uint8_t x, uint8_t y) {
    OLED_WriteByte(0xB0 + y, OLED_CMD);                  // 设置页地址
    OLED_WriteByte(((x & 0xF0) >> 4) | 0x10, OLED_CMD);   // 设置高列地址
    OLED_WriteByte(x & 0x0F, OLED_CMD);                   // 设置低列地址
}

// 清屏
void OLED_Clear(void) {
    uint8_t x, y;
    for(y = 0; y < 8; y++) {
        OLED_SetPos(0, y);
        for(x = 0; x < 128; x++) {
            OLED_WriteByte(0x00, OLED_DATA);  // 填充黑色
        }
    }
}

// 显示单个字符（x:0-127, y:0-7，8x16点阵）
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr) {
    uint8_t i;
    chr -= ' ';  // 计算偏移量（从空格开始）
    
    OLED_SetPos(x, y);          // 设置页地址（上半部分）
    for(i = 0; i < 8; i++) {
        OLED_WriteByte(OLED_F8x16[chr][i], OLED_DATA);
    }
    
    OLED_SetPos(x, y + 1);      // 设置页地址（下半部分）
    for(i = 8; i < 16; i++) {
        OLED_WriteByte(OLED_F8x16[chr][i], OLED_DATA);
    }
}

// 显示字符串
void OLED_ShowString(uint8_t x, uint8_t y, uint8_t *str) {
    uint8_t i = 0;
    while(str[i] != '\0') {
        OLED_ShowChar(x + i * 8, y, str[i]);  // 每个字符占8列
        i++;
    }
}
    
