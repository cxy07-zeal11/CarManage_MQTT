#ifndef __FINGER_H
#define __FINGER_H

#include "stm32f4xx.h"

// 指纹模块返回值定义
#define FINGERPRINT_OK 0x00
#define FINGERPRINT_PACKETRECIEVEERR 0x01
#define FINGERPRINT_NOFINGER 0x02
#define FINGERPRINT_IMAGEFAIL 0x03
#define FINGERPRINT_IMAGEMESS 0x06
#define FINGERPRINT_FEATUREFAIL 0x07
#define FINGERPRINT_NOMATCH 0x08
#define FINGERPRINT_NOTFOUND 0x09
#define FINGERPRINT_ENROLLMISMATCH 0x0A
#define FINGERPRINT_BADLOCATION 0x0B
#define FINGERPRINT_DBRANGEFAIL 0x0C
#define FINGERPRINT_UPLOADFEATUREFAIL 0x0D
#define FINGERPRINT_PACKETRESPONSEFAIL 0x0E
#define FINGERPRINT_UPLOADFAIL 0x0F
#define FINGERPRINT_DELETEFAIL 0x10
#define FINGERPRINT_DBCLEARFAIL 0x11
#define FINGERPRINT_PASSFAIL 0x13
#define FINGERPRINT_INVALIDIMAGE 0x15
#define FINGERPRINT_FLASHERR 0x18

#define FINGERPRINT_STARTCODE 0xEF01
#define FINGERPRINT_COMMANDPACKET 0x1
#define FINGERPRINT_ACKPACKET 0x7
#define FINGERPRINT_TIMEOUT 0xFF
#define FINGERPRINT_BADPACKET 0xFE

// 外部变量
extern uint16_t fingerID;
extern uint16_t confidence;

// 函数声明
uint8_t verifyPassword(void);
uint8_t getImage(void);
uint8_t image2Tz(uint8_t slot);
uint8_t createModel(void);
uint8_t storeModel(uint16_t id);
uint8_t emptyDatabase(void);
uint8_t fingerFastSearch(void);
uint8_t getTemplateCount(void);
void writePacket(uint32_t addr, uint8_t packettype, uint16_t len, uint8_t *packet);
uint8_t getReply(uint8_t *packet, uint16_t timeout);

#endif /* __FINGER_H */