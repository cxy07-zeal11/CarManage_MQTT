#include "mqtt.h"

uint8_t MQTT_RX_BUF[512];//MQTT数据包发送
uint16_t DataLen;

volatile uint16_t MQTT_Len;

/**
 * @brief 清理对应数据
 *
 * @return 没有返回参数
 */
void MQTT_Clear(void)
{
	DataLen = 0;   //数据
	MQTT_Len = 0;  //数组下标
	memset(MQTT_RX_BUF,0,sizeof(MQTT_RX_BUF)); //清空接收缓冲
}

/**
 * @brief 通过MQTT发送服务器数据
 *
 * @return 没有返回参数
 */
void MQTT_SendData(uint8_t* buf,uint16_t len)
{
	Usart_SendArray(USART3,buf,len);
}


/**
 * @brief MQTT发送心跳包指令
 *
 * @return 没有返回参数
 */
void MQTT_SendHeart(void)
{
	MQTT_Clear();
	/*
		这是在设置 MQTT 心跳请求（PINGREQ）报文的固定头部。
		心跳请求报文的固定头部第一个字节是 0xc0，
		表示消息类型为 PINGREQ（心跳请求），且 DUP 标志为 0，QoS 等级为 0，保留标志为 0。
	*/
	MQTT_RX_BUF[0] = 0xc0; 
	/*
	心跳请求报文的剩余长度。
	对于心跳请求报文，没有可变头部和有效载荷，所以剩余长度为 0。
	*/
	MQTT_RX_BUF[1] = 0x00; 
	MQTT_SendData(MQTT_RX_BUF,2);
}

/**
 * @brief MQTT指令登录云服务器
 *
 * @return 没有返回参数
 */
void MQTT_Connect(void)
{
	MQTT_Clear();
	/*******************三要素的长度计算*****************/
	uint8_t MQTT_ClientID_Len = strlen(MQTT_Client_ID);
	uint8_t MQTT_UserName_Len = strlen(MQTT_User_Name);
	uint8_t MQTT_Password_Len = strlen(MQTT_Password);


	
	/*固定报头*/
	MQTT_RX_BUF[MQTT_Len++] = 0x10;
	/*
		剩余长度计算  = 可变报头 + 有效载荷
		每个字节的最高位（第 7 位）是延续位（Continuation Bit），用于指示是否还有后续字节。若此位为 1，表明后面还有字节用于表示剩余长度；若为 0，则意味着这是表示剩余长度的最后一个字节。
		每个字节的低 7 位（第 0 - 6 位）用来存储实际的数值。
	*/
	//~~~~~~~~~~~~~~~~~~~~~剩余长度~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		//可变报头(长度固定10)+Payload  每个字段包含两个字节的长度标识
  DataLen = 10 + (MQTT_ClientID_Len+2) + (MQTT_UserName_Len+2) + (MQTT_Password_Len+2);
	//十六进制 和 2进制的关系 
	do
	{
		uint8_t encodedByte = DataLen % 128;  //FF   低8位   FF不够  +  高8位  
		DataLen = DataLen / 128;
		if ( DataLen > 0 )
			encodedByte = encodedByte | 128;
		MQTT_RX_BUF[MQTT_Len++] = encodedByte;
	}while ( DataLen > 0 );
	//~~~~~~~~~~~~~~~~~~~~~~剩余长度~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	
	/*************可变报头   10位 ****************/
	MQTT_RX_BUF[MQTT_Len++] = 0x00; //协议名长度的 高8位 MSB(0)
	MQTT_RX_BUF[MQTT_Len++] = 0x04; //协议名长度的 低8为 LSB(4)  MQTT就四个字母
	MQTT_RX_BUF[MQTT_Len++] = 0x4d; //M
	MQTT_RX_BUF[MQTT_Len++] = 0x51; //Q
	MQTT_RX_BUF[MQTT_Len++] = 0x54; //T
	MQTT_RX_BUF[MQTT_Len++] = 0x54; //T
	MQTT_RX_BUF[MQTT_Len++] = 0x04; //Level(4)  使用MQTT4   4是版本
	MQTT_RX_BUF[MQTT_Len++] = 0xc2; //开启User Name Flag,Password Flag,Clean Session
	MQTT_RX_BUF[MQTT_Len++] = 0x00; //保持连接 MSB (0)
	MQTT_RX_BUF[MQTT_Len++] = 0x64; //保持连接 LSB (100)  100s之内 会有联系 
	
	/*有效载荷*/
	/********************客户端标识符*********************/
	MQTT_RX_BUF[MQTT_Len++] = 0x00;                      // Client ID 的 MSB    
	MQTT_RX_BUF[MQTT_Len++] = BYTE0(MQTT_ClientID_Len);  // Client ID 的 LSB  	
	memcpy(&MQTT_RX_BUF[MQTT_Len],MQTT_Client_ID,MQTT_ClientID_Len); //Client ID 的数据 
	MQTT_Len += MQTT_ClientID_Len;
	
	/********************用户的数据*********************/
	MQTT_RX_BUF[MQTT_Len++] = 0x00;		                                //用户的MSB   
	MQTT_RX_BUF[MQTT_Len++] = BYTE0(MQTT_UserName_Len);    	          //用户的LSB    
	memcpy(&MQTT_RX_BUF[MQTT_Len],MQTT_User_Name,MQTT_UserName_Len);  //用户的数据
	MQTT_Len += MQTT_UserName_Len;
	
	/********************密码的数据*********************/
	MQTT_RX_BUF[MQTT_Len++] = 0x00;		                              //密码的MSB    
	MQTT_RX_BUF[MQTT_Len++] = BYTE0(MQTT_Password_Len);    	        //密码的LSB  
	memcpy(&MQTT_RX_BUF[MQTT_Len],MQTT_Password,MQTT_Password_Len); //密码的数据
	MQTT_Len += MQTT_Password_Len;   
	
	
	MQTT_SendData(MQTT_RX_BUF,MQTT_Len);//发送对应的数据到云端进行设别的登录
}

/**
 * @brief MQTT指令断开云服务器
 *
 * @return 没有返回参数
 */
void MQTT_Disconnect(void)
{
	MQTT_Clear();
	MQTT_RX_BUF[0] = 0xe0; 
	MQTT_RX_BUF[1] = 0x00; 
	MQTT_SendData(MQTT_RX_BUF,2);//发送对应的数据到云端进行设别的断开连接
}

/**
 * @brief 通过MQTT发布对应数据到云服务器
 * @param humi 湿度数据
 * @param temp 温度数据
 * @return 没有返回参数
 */
//void MQTT_PublicTopic(float temp,float humi) 
void MQTT_PublicTopic(float temp, float humi, const char* co_str, const char* door_str, uint16_t light_val, const char* people_str)
{
    char SendData[512];
    MQTT_Clear();
    uint8_t MQTT_PublishTopic_Len = strlen(PublishTopic); //计算发布主题的长度
    sprintf(SendData, MQTTPUBLISH(temp, humi, co_str, door_str, light_val, people_str));  //将发送数据整理成onejson格式        
    uint8_t MQTT_MQTTPUBLISH_Len = strlen(SendData);  //计算发送数据的长度
    
    MQTT_RX_BUF[MQTT_Len++] = 0x30;  //固定的报头
    
    DataLen = (MQTT_PublishTopic_Len + 2) + MQTT_MQTTPUBLISH_Len;
    /**计算剩余长度**/
    do
    {
        uint8_t encodedByte = DataLen % 128;
        DataLen = DataLen / 128;
        if ( DataLen > 0 )
            encodedByte = encodedByte | 128;
        MQTT_RX_BUF[MQTT_Len++] = encodedByte;
        
    }while ( DataLen > 0 );

    
    MQTT_RX_BUF[MQTT_Len++] = 0x00;  //发布主题MSB
    MQTT_RX_BUF[MQTT_Len++] = BYTE0(MQTT_PublishTopic_Len);  //发布主题的LSB
    memcpy(&MQTT_RX_BUF[MQTT_Len], PublishTopic, MQTT_PublishTopic_Len);  //添加发布主题到发送数据里面
    MQTT_Len += MQTT_PublishTopic_Len;
    
    
    //不用给长度
    
    memcpy(&MQTT_RX_BUF[MQTT_Len], SendData, MQTT_MQTTPUBLISH_Len);  //添加发布内容到数据里面
    MQTT_Len += MQTT_MQTTPUBLISH_Len;
    
    MQTT_SendData(MQTT_RX_BUF, MQTT_Len);  //发送MQTT的发布数据到服务器
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



/**
 * @brief MQTT指令订阅服务器
 *
 * @return 没有返回参数
 */
void MQTT_SubscribeTopic(void)
{
	MQTT_Clear();
	uint8_t MQTT_SubscribeTopic_Len = strlen(SubscribeTopic_Server);//计算订阅主题的长度
	
  DataLen = 2 + (MQTT_SubscribeTopic_Len+2) + 1;
	
	/*固定报头*/
	MQTT_RX_BUF[MQTT_Len++] = 0x82;
	/**剩余长度**/
	do
	{
		uint8_t encodedByte = DataLen % 128;
		DataLen = DataLen / 128;
		if ( DataLen > 0 )
			encodedByte = encodedByte | 128;
		MQTT_RX_BUF[MQTT_Len++] = encodedByte;
	}while ( DataLen > 0 );
	
	/*可变报头*/
	MQTT_RX_BUF[MQTT_Len++] = 0x00;  //用户自定义 
	MQTT_RX_BUF[MQTT_Len++] = 0x01;  //用户自定义
	MQTT_RX_BUF[MQTT_Len++] = 0x00;  //订阅的MSB

	MQTT_RX_BUF[MQTT_Len++] = MQTT_SubscribeTopic_Len;  //订阅的LSB
	memcpy(&MQTT_RX_BUF[MQTT_Len],SubscribeTopic_Server,MQTT_SubscribeTopic_Len);  //添加订阅主题的路径
	MQTT_Len += MQTT_SubscribeTopic_Len;
	MQTT_RX_BUF[MQTT_Len++] = 0x00; //订阅主题等级为Q0
	
	MQTT_SendData(MQTT_RX_BUF,MQTT_Len);  //发送MQTT订阅指令到云服务器
	
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/*
void MQTT_SubscribeTopic_web(void)
{
	MQTT_Clear();
	uint8_t MQTT_SubscribeTopic_Len = strlen(SubscribeTopic_Server_web);//计算订阅主题的长度
	
  DataLen = 2 + (MQTT_SubscribeTopic_Len+2) + 1;
	

	MQTT_RX_BUF[MQTT_Len++] = 0x82;
	
	do
	{
		uint8_t encodedByte = DataLen % 128;
		DataLen = DataLen / 128;
		if ( DataLen > 0 )
			encodedByte = encodedByte | 128;
		MQTT_RX_BUF[MQTT_Len++] = encodedByte;
	}while ( DataLen > 0 );
	
	
	MQTT_RX_BUF[MQTT_Len++] = 0x00;  //用户自定义 
	MQTT_RX_BUF[MQTT_Len++] = 0x01;  //用户自定义
	MQTT_RX_BUF[MQTT_Len++] = 0x00;  //订阅的MSB

	MQTT_RX_BUF[MQTT_Len++] = MQTT_SubscribeTopic_Len;  //订阅的LSB
	memcpy(&MQTT_RX_BUF[MQTT_Len],SubscribeTopic_Server_web,MQTT_SubscribeTopic_Len);  //添加订阅主题的路径
	MQTT_Len += MQTT_SubscribeTopic_Len;
	MQTT_RX_BUF[MQTT_Len++] = 0x00; //订阅主题等级为Q0
	
	MQTT_SendData(MQTT_RX_BUF,MQTT_Len);  //发送MQTT订阅指令到云服务器
	
}
*/
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/**
 * @brief MQTT指令取消订阅对应的主题
 *
 * @return 没有返回参数
 */
void MQTT_UNSubscribeTopic(void)
{
	MQTT_Clear();
	uint8_t MQTT_SubscribeTopic_Len = strlen(SubscribeTopic_Server_Reply);//计算取消订阅主题的长度

  DataLen = 2 + (MQTT_SubscribeTopic_Len+2);
	
	/*固定报头*/
	MQTT_RX_BUF[MQTT_Len++] = 0xA2;
	/*剩余长度*/
	do
	{
		uint8_t encodedByte = DataLen % 128;
		DataLen = DataLen / 128;
		if ( DataLen > 0 )
			encodedByte = encodedByte | 128;
		MQTT_RX_BUF[MQTT_Len++] = encodedByte;
	}while ( DataLen > 0 );
	
	/*可变报头*/
	MQTT_RX_BUF[MQTT_Len++] = 0x00;  //用户自定义
	MQTT_RX_BUF[MQTT_Len++] = 0x01;  //用户自定义
	
	MQTT_RX_BUF[MQTT_Len++] = 0x00;  //订阅的MSB
	
	MQTT_RX_BUF[MQTT_Len++] = MQTT_SubscribeTopic_Len;  //取消订阅的LSB
	memcpy(&MQTT_RX_BUF[MQTT_Len],SubscribeTopic_Server_Reply,MQTT_SubscribeTopic_Len);  //添加取消订阅主题的路径
	MQTT_Len += MQTT_SubscribeTopic_Len;
	
	MQTT_SendData(MQTT_RX_BUF,MQTT_Len);  //发送MQTT订阅指令到云服务器
}

