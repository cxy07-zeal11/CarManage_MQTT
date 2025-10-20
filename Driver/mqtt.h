#ifndef __MQTT_H
#define __MQTT_H

#include "esp8266.h"
#include "DELAY.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "UART.h"

#define BYTE0(dwTemp)       (*( char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))
	
/************************	ONENET服务器***************************************/
//#define MQTT_Client_ID  "DHT11" //设备名字
//#define MQTT_User_Name "G7NqVv6le8" //设备ID
//#define MQTT_Password  "version=2018-10-31&res=products%2FG7NqVv6le8%2Fdevices%2FDHT11&et=1729607740&method=md5&sign=VRwAjJjquF6I%2FyCFP%2FnnFQ%3D%3D" //MQTT连接ONENET的密码
//#define SubscribeTopic_Attribute "$sys/G7NqVv6le8/DHT11/thing/property/post/reply" //订阅的属性主题
//#define SubscribeTopic_Server "$sys/G7NqVv6le8/DHT11/thing/service/LED_CONTROL/invoke" //订阅的服务主题
//#define PublishTopic   "$sys/G7NqVv6le8/DHT11/thing/property/post" //发布的主题
//#define MQTTPUBLISH(temp, humi) "{\"id\":\"123\",\"params\":{\"temp\":{\"value\":%.2lf},\"humi\":{\"value\":%.2lf}}}",temp, humi //发送onejson数据

/************************	华为云服务器***************************************/
#define MQTT_Client_ID  "68cb65bd32771f177b6134de_car_system_0_0_2025091802" 
#define MQTT_User_Name  "68cb65bd32771f177b6134de_car_system" //设备ID
#define MQTT_Password  "7da0fa7b7b1f94439e790e3a09e2700dd955a3504240a0bd865ac8f409df44c5" //MQTT连接ONENET的密码
//                                   $oc/devices/687767df32771f177b4ca472_shebei/sys/commands/request_id={request_id}
#define SubscribeTopic_Server_Reply "$oc/devices/68cb65bd32771f177b6134de_car_system/sys/commands/request_id={request_id}" //订阅的属性主题
#define SubscribeTopic_Server "$oc/devices/68cb65bd32771f177b6134de_car_system/sys/messages/down" 
//#define SubscribeTopic_Server_web "/k2877qwTkuz/shebei1/user/get" 
//                      $oc/devices/68c8bb21d582f200184fd55a_kongtiao/sys/properties/report
#define PublishTopic   "$oc/devices/{68cb65bd32771f177b6134de_car_system}/sys/properties/report" //发布的主题
//#define PublishTopic_web   "/k2877qwTkuz/shebei32/user/update" //发布的主题
// #define MQTTPUBLISH(temp, humi) "{\"method\":\"thing.service.property.set\",\"id\":\"1229117953\",\"params\":{\"CurrentHumidity\":%2lf,\"CurrentTemperature\":%2lf,\"LED\":1,\"GeoLocation\":{\"CoordinateSystem\":10,\"Latitude\":10,\"Longitude\":10,\"Altitude\":10}},\"version\":\"1.0.0\"}",humi,temp


//#define MQTTPUBLISH(temp, humi, LED) "{\"method\":\"thing.service.property.set\",\"id\":\"1774038050\",\"params\":{\"CurrentHumidity\":%2lf,\"CurrentTemperature\":%2lf,\"LED\":%d},\"version\":\"1.0.0\"}",humi,temp,LED

// JSON字符串  "{\"name\":\"张三\","age":"18","hobby":"看电视","child":{"name":"xiaoming","age":"11"}}"  ----  几乎所有语言  都可以直接操作 JSON字符串

#define MQTTPUBLISH(temp, humi, co_str, door_str, light_val, people_str) \
    "{\"services\":[{\"service_id\":\"car\",\"properties\":{\"humi\":%f,\"temp\":%f,\"CO\":\"%s\",\"Door\":\"%s\",\"light\":%d,\"people\":\"%s\"},\"event_time\":\"20151212T121212Z\"}]}", \
    humi, temp, co_str, door_str, light_val, people_str
		
void MQTT_Clear(void);
void MQTT_SendData(uint8_t* buf,uint16_t len);
void MQTT_SendHeart(void);
void MQTT_Init(void);
void MQTT_Connect(void);
void MQTT_Disconnect(void);
//void MQTT_PublicTopic(float humi,float temp);

void MQTT_PublicTopic(float temp, float humi, const char* co_str, const char* door_str, uint16_t light_val, const char* people_str);
void MQTT_PublicTopic_web(float temp, float humi, const char* co_str, const char* door_str, uint16_t light_val, const char* people_str);void MQTT_SubscribeTopic(void);
void MQTT_SubscribeTopic_web(void);
void MQTT_UNSubscribeTopic(void);

#endif

