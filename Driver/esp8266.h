#ifndef __ESP8266_H
#define __ESP8266_H

#include "DELAY.h"
#include "stm32f4xx.h"
#include "DELAY.h"
#include "mqtt.h"


#include <stdio.h>
#include <string.h>
#include <stdarg.h>


#define REV_OK		0	//������ɱ�־
#define REV_WAIT	1	//����δ��ɱ�־


#define ESP8266_WIFI_INFO		"AT+CWJAP=\"Peach\",\"041117&02102\"\r\n" //wifi���˺ź�����

//#define ESP8266_SERVER "AT+CIPSTART=\"TCP\",\"mqtts.heclouds.com\",1883\r\n"  //(TCP/UDP),ģʽONENET Borker Address��Borker Port
#define ESP8266_SERVER "AT+CIPSTART=\"TCP\",\"9f2105f14e.st1.iotda-device.cn-north-4.myhuaweicloud.com\",1883\r\n"  //(TCP/UDP)ģʽ��ALIYUN Borker Address��Borker Port






_Bool ESP8266_WaitRecive(void);
void ESP8266_Init(void);
void ESP8266_memset_RecvBuff(void);
_Bool ESP8266_SendCmd(char *str, char* rev);
void ESP8266_ConnectServer(void);
void recv_data_control(char * data);
void MQTT_RX_DATE_DEAL(char *recv_buf);
#endif 
