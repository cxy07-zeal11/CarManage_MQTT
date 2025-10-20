#include "Finger.h"
#include "usart.h"
#include "delay.h"

uint32_t thePassword = 0x00000000; // 默认密码通常是0
uint32_t theAddress = 0xFFFFFFFF;
uint16_t fingerID = 0;
uint16_t confidence = 0;

void writePacket(uint32_t addr, uint8_t packettype, uint16_t len, uint8_t *packet)
{
    uint16_t i;
    uint16_t checksum = 0;
    uint8_t buf[len + 11];
    
    buf[0] = (FINGERPRINT_STARTCODE >> 8) & 0xFF;
    buf[1] = FINGERPRINT_STARTCODE & 0xFF;
    buf[2] = (addr >> 24) & 0xFF;
    buf[3] = (addr >> 16) & 0xFF;
    buf[4] = (addr >> 8) & 0xFF;
    buf[5] = addr & 0xFF;
    buf[6] = packettype;
    buf[7] = (len >> 8) & 0xFF;
    buf[8] = len & 0xFF;
    
    for(i = 0; i < len; i++)
    {
        buf[9 + i] = packet[i];
    }
    
    checksum = buf[6] + buf[7] + buf[8];
    for(i = 0; i < len; i++)
    {
        checksum += packet[i];
    }
    
    buf[9 + len] = (checksum >> 8) & 0xFF;
    buf[10 + len] = checksum & 0xFF;
    
    USART2_SendData(buf, len + 11);
}

uint8_t getReply(uint8_t *packet, uint16_t timeout)
{
    uint8_t byte;
    uint16_t idx = 0, timer = 0;
    uint16_t length;
    
    while(1)
    {
        while(!USART_GetFlagStatus(USART2, USART_FLAG_RXNE))
        {
            delay_ms(1);
            timer++;
            if(timer >= timeout)
                return FINGERPRINT_TIMEOUT;
        }
        
        // 重置超时计时器
        timer = 0;
        
        byte = USART_ReceiveData(USART2);
        
        if((idx == 0) && (byte != (FINGERPRINT_STARTCODE >> 8)))
            continue;
        if((idx == 1) && (byte != (FINGERPRINT_STARTCODE & 0xFF)))
        {
            idx = 0;
            continue;
        }
        
        packet[idx] = byte;
        idx++;
        
        if(idx == 9)
        {
            if((packet[0] != (FINGERPRINT_STARTCODE >> 8)) || 
               (packet[1] != (FINGERPRINT_STARTCODE & 0xFF)))
                return FINGERPRINT_BADPACKET;
            
            uint8_t type = packet[6];
            length = ((uint16_t)packet[7] << 8) | packet[8];
            
            if((idx + length + 2) > 128)
                return FINGERPRINT_BADPACKET;
            
            timer = 0;
            while(idx < (length + 10))
            {
                while(!USART_GetFlagStatus(USART2, USART_FLAG_RXNE))
                {
                    delay_ms(1);
                    timer++;
                    if(timer >= timeout)
                        return FINGERPRINT_TIMEOUT;
                }
                packet[idx] = USART_ReceiveData(USART2);
                idx++;
                timer = 0;
            }
            
            uint16_t checksum = ((uint16_t)packet[9 + length] << 8) | packet[10 + length];
            uint16_t sum = 0;
            for(uint8_t i = 6; i < (9 + length); i++)
            {
                sum += packet[i];
            }
            
            if(sum != checksum)
                return FINGERPRINT_BADPACKET;
            
            return type;
        }
    }
}
uint8_t verifyPassword(void)
{
    uint8_t packet[] = {0x13, 0x00, 0x00, 0x00, 0x00};
    writePacket(theAddress, FINGERPRINT_COMMANDPACKET, sizeof(packet), packet);
    
    // 添加发送数据的调试信息
    USART1_SendString("发送验证密码命令: ");
    for(int i = 0; i < sizeof(packet); i++) {
        char buf[4];
        sprintf(buf, "%02X ", packet[i]);
        USART1_SendString(buf);
    }
    USART1_SendString("\r\n");
    
    uint8_t data[64];
    uint8_t reply = getReply(data, 1000);
    
    char buf[32];
    sprintf(buf, "收到回复类型: %d\r\n", reply);
    USART1_SendString(buf);
    
    if(reply != FINGERPRINT_ACKPACKET) {
        sprintf(buf, "验证密码回复错误: %d\r\n", reply);
        USART1_SendString(buf);
        return FINGERPRINT_PACKETRECIEVEERR;
    }
    
    sprintf(buf, "验证结果: %d\r\n", data[9]);
    USART1_SendString(buf);
    
    return data[9];
}
uint8_t image2Tz(uint8_t slot)
{
    uint8_t packet[] = {0x02, slot};
    writePacket(theAddress, FINGERPRINT_COMMANDPACKET, sizeof(packet), packet);
    
    uint8_t data[64];
    if(getReply(data, 10000) != FINGERPRINT_ACKPACKET)
        return FINGERPRINT_PACKETRECIEVEERR;
    
    return data[9];
}

uint8_t createModel(void)
{
    uint8_t packet[] = {0x05};
    writePacket(theAddress, FINGERPRINT_COMMANDPACKET, sizeof(packet), packet);
    
    uint8_t data[64];
    if(getReply(data, 10000) != FINGERPRINT_ACKPACKET)
        return FINGERPRINT_PACKETRECIEVEERR;
    
    return data[9];
}

uint8_t storeModel(uint16_t id)
{
    uint8_t packet[] = {0x06, 0x01, (uint8_t)(id >> 8), (uint8_t)(id & 0xFF)};
    writePacket(theAddress, FINGERPRINT_COMMANDPACKET, sizeof(packet), packet);
    
    uint8_t data[64];
    if(getReply(data, 10000) != FINGERPRINT_ACKPACKET)
        return FINGERPRINT_PACKETRECIEVEERR;
    
    return data[9];
}

uint8_t emptyDatabase(void)
{
    uint8_t packet[] = {0x0D};
    writePacket(theAddress, FINGERPRINT_COMMANDPACKET, sizeof(packet), packet);
    
    uint8_t data[64];
    if(getReply(data, 10000) != FINGERPRINT_ACKPACKET)
        return FINGERPRINT_PACKETRECIEVEERR;
    
    return data[9];
}

uint8_t fingerFastSearch(void)
{
    uint8_t packet[] = {0x04, 0x01, 0x00, 0x00, 0x00, 0xA3};
    writePacket(theAddress, FINGERPRINT_COMMANDPACKET, sizeof(packet), packet);
    
    uint8_t data[64];
    if(getReply(data, 10000) != FINGERPRINT_ACKPACKET)
        return FINGERPRINT_PACKETRECIEVEERR;
    
    fingerID = ((uint16_t)data[10] << 8) | data[11];
    confidence = ((uint16_t)data[12] << 8) | data[13];
    
    return data[9];
}

uint8_t getTemplateCount(void)
{
    uint8_t packet[] = {0x1D};
    writePacket(theAddress, FINGERPRINT_COMMANDPACKET, sizeof(packet), packet);
    
    uint8_t data[64];
    if(getReply(data, 1000) != FINGERPRINT_ACKPACKET)
        return FINGERPRINT_PACKETRECIEVEERR;
    
    return (data[9] << 8) | data[10];
}
// 在 Finger.c 中添加 getImage 函数
uint8_t getImage(void)
{
    uint8_t packet[] = {0x01};
    writePacket(theAddress, FINGERPRINT_COMMANDPACKET, sizeof(packet), packet);
    
    uint8_t data[64];
    if(getReply(data, 10000) != FINGERPRINT_ACKPACKET)
        return FINGERPRINT_PACKETRECIEVEERR;
    
    return data[9];
}
