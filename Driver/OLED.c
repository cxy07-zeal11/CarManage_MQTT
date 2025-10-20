#include "OLED.h"
#include "OLED_Font.h"

// ��ʼ��OLED
void OLED_Config(void) 
{
	Soft_IIC_Config();  // ��ʼ���Զ���I2C
	
	delay_ms(100);      // �ϵ���ʱ
    
  // ���ͳ�ʼ������
  OLED_WriteByte(0xAE, OLED_CMD);	//�ر���ʾ
	
	OLED_WriteByte(0xD5, OLED_CMD);	//������ʾʱ�ӷ�Ƶ��/����Ƶ��
	OLED_WriteByte(0x80, OLED_CMD);
	
	OLED_WriteByte(0xA8, OLED_CMD);	//���ö�·������
	OLED_WriteByte(0x3F, OLED_CMD);
	
	OLED_WriteByte(0xD3, OLED_CMD);	//������ʾƫ��
	OLED_WriteByte(0x00, OLED_CMD);
	
	OLED_WriteByte(0x40, OLED_CMD);	//������ʾ��ʼ��
	
	OLED_WriteByte(0xA1, OLED_CMD);	//�������ҷ���0xA1���� 0xA0���ҷ���
	
	OLED_WriteByte(0xC8, OLED_CMD);	//�������·���0xC8���� 0xC0���·���

	OLED_WriteByte(0xDA, OLED_CMD);	//����COM����Ӳ������
	OLED_WriteByte(0x12, OLED_CMD);
	
	OLED_WriteByte(0x81, OLED_CMD);	//���öԱȶȿ���
	OLED_WriteByte(0xCF, OLED_CMD);

	OLED_WriteByte(0xD9, OLED_CMD);	//����Ԥ�������
	OLED_WriteByte(0xF1, OLED_CMD);

	OLED_WriteByte(0xDB, OLED_CMD);	//����VCOMHȡ��ѡ�񼶱�
	OLED_WriteByte(0x30, OLED_CMD);

	OLED_WriteByte(0xA4, OLED_CMD);	//����������ʾ��/�ر�

	OLED_WriteByte(0xA6, OLED_CMD);	//��������/��ת��ʾ

	OLED_WriteByte(0x8D, OLED_CMD);	//���ó���
	OLED_WriteByte(0x14, OLED_CMD);

	OLED_WriteByte(0xAF, OLED_CMD);	//������ʾ
    
    OLED_Clear();       // ����
}

// д��һ���ֽڣ���������ݣ�
void OLED_WriteByte(uint8_t dat, uint8_t cmd) {
    Soft_IIC_Start();               // ��ʼI2Cͨ��
    Soft_IIC_Send(OLED_ADDR);       // ����OLED��ַ
    Soft_IIC_SlaveAck();            // �ȴ��ӻ�Ӧ��
    
    Soft_IIC_Send(cmd);             // ��������/���ݱ�־
    Soft_IIC_SlaveAck();            // �ȴ�Ӧ��
    
    Soft_IIC_Send(dat);             // ����ʵ������
    Soft_IIC_SlaveAck();            // �ȴ�Ӧ��
    
    Soft_IIC_Stop();                // ����ͨ��
}

// ������ʾλ�ã�x:0-127, y:0-7��y����ҳ��ַ��
void OLED_SetPos(uint8_t x, uint8_t y) {
    OLED_WriteByte(0xB0 + y, OLED_CMD);                  // ����ҳ��ַ
    OLED_WriteByte(((x & 0xF0) >> 4) | 0x10, OLED_CMD);   // ���ø��е�ַ
    OLED_WriteByte(x & 0x0F, OLED_CMD);                   // ���õ��е�ַ
}

// ����
void OLED_Clear(void) {
    uint8_t x, y;
    for(y = 0; y < 8; y++) {
        OLED_SetPos(0, y);
        for(x = 0; x < 128; x++) {
            OLED_WriteByte(0x00, OLED_DATA);  // ����ɫ
        }
    }
}

// ��ʾ�����ַ���x:0-127, y:0-7��8x16����
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr) {
    uint8_t i;
    chr -= ' ';  // ����ƫ�������ӿո�ʼ��
    
    OLED_SetPos(x, y);          // ����ҳ��ַ���ϰ벿�֣�
    for(i = 0; i < 8; i++) {
        OLED_WriteByte(OLED_F8x16[chr][i], OLED_DATA);
    }
    
    OLED_SetPos(x, y + 1);      // ����ҳ��ַ���°벿�֣�
    for(i = 8; i < 16; i++) {
        OLED_WriteByte(OLED_F8x16[chr][i], OLED_DATA);
    }
}

// ��ʾ�ַ���
void OLED_ShowString(uint8_t x, uint8_t y, uint8_t *str) {
    uint8_t i = 0;
    while(str[i] != '\0') {
        OLED_ShowChar(x + i * 8, y, str[i]);  // ÿ���ַ�ռ8��
        i++;
    }
}
    
