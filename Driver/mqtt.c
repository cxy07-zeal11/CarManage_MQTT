#include "mqtt.h"

uint8_t MQTT_RX_BUF[512];//MQTT���ݰ�����
uint16_t DataLen;

volatile uint16_t MQTT_Len;

/**
 * @brief �����Ӧ����
 *
 * @return û�з��ز���
 */
void MQTT_Clear(void)
{
	DataLen = 0;   //����
	MQTT_Len = 0;  //�����±�
	memset(MQTT_RX_BUF,0,sizeof(MQTT_RX_BUF)); //��ս��ջ���
}

/**
 * @brief ͨ��MQTT���ͷ���������
 *
 * @return û�з��ز���
 */
void MQTT_SendData(uint8_t* buf,uint16_t len)
{
	Usart_SendArray(USART3,buf,len);
}


/**
 * @brief MQTT����������ָ��
 *
 * @return û�з��ز���
 */
void MQTT_SendHeart(void)
{
	MQTT_Clear();
	/*
		���������� MQTT ��������PINGREQ�����ĵĹ̶�ͷ����
		���������ĵĹ̶�ͷ����һ���ֽ��� 0xc0��
		��ʾ��Ϣ����Ϊ PINGREQ���������󣩣��� DUP ��־Ϊ 0��QoS �ȼ�Ϊ 0��������־Ϊ 0��
	*/
	MQTT_RX_BUF[0] = 0xc0; 
	/*
	���������ĵ�ʣ�೤�ȡ�
	�������������ģ�û�пɱ�ͷ������Ч�غɣ�����ʣ�೤��Ϊ 0��
	*/
	MQTT_RX_BUF[1] = 0x00; 
	MQTT_SendData(MQTT_RX_BUF,2);
}

/**
 * @brief MQTTָ���¼�Ʒ�����
 *
 * @return û�з��ز���
 */
void MQTT_Connect(void)
{
	MQTT_Clear();
	/*******************��Ҫ�صĳ��ȼ���*****************/
	uint8_t MQTT_ClientID_Len = strlen(MQTT_Client_ID);
	uint8_t MQTT_UserName_Len = strlen(MQTT_User_Name);
	uint8_t MQTT_Password_Len = strlen(MQTT_Password);


	
	/*�̶���ͷ*/
	MQTT_RX_BUF[MQTT_Len++] = 0x10;
	/*
		ʣ�೤�ȼ���  = �ɱ䱨ͷ + ��Ч�غ�
		ÿ���ֽڵ����λ���� 7 λ��������λ��Continuation Bit��������ָʾ�Ƿ��к����ֽڡ�����λΪ 1���������滹���ֽ����ڱ�ʾʣ�೤�ȣ���Ϊ 0������ζ�����Ǳ�ʾʣ�೤�ȵ����һ���ֽڡ�
		ÿ���ֽڵĵ� 7 λ���� 0 - 6 λ�������洢ʵ�ʵ���ֵ��
	*/
	//~~~~~~~~~~~~~~~~~~~~~ʣ�೤��~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		//�ɱ䱨ͷ(���ȹ̶�10)+Payload  ÿ���ֶΰ��������ֽڵĳ��ȱ�ʶ
  DataLen = 10 + (MQTT_ClientID_Len+2) + (MQTT_UserName_Len+2) + (MQTT_Password_Len+2);
	//ʮ������ �� 2���ƵĹ�ϵ 
	do
	{
		uint8_t encodedByte = DataLen % 128;  //FF   ��8λ   FF����  +  ��8λ  
		DataLen = DataLen / 128;
		if ( DataLen > 0 )
			encodedByte = encodedByte | 128;
		MQTT_RX_BUF[MQTT_Len++] = encodedByte;
	}while ( DataLen > 0 );
	//~~~~~~~~~~~~~~~~~~~~~~ʣ�೤��~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	
	/*************�ɱ䱨ͷ   10λ ****************/
	MQTT_RX_BUF[MQTT_Len++] = 0x00; //Э�������ȵ� ��8λ MSB(0)
	MQTT_RX_BUF[MQTT_Len++] = 0x04; //Э�������ȵ� ��8Ϊ LSB(4)  MQTT���ĸ���ĸ
	MQTT_RX_BUF[MQTT_Len++] = 0x4d; //M
	MQTT_RX_BUF[MQTT_Len++] = 0x51; //Q
	MQTT_RX_BUF[MQTT_Len++] = 0x54; //T
	MQTT_RX_BUF[MQTT_Len++] = 0x54; //T
	MQTT_RX_BUF[MQTT_Len++] = 0x04; //Level(4)  ʹ��MQTT4   4�ǰ汾
	MQTT_RX_BUF[MQTT_Len++] = 0xc2; //����User Name Flag,Password Flag,Clean Session
	MQTT_RX_BUF[MQTT_Len++] = 0x00; //�������� MSB (0)
	MQTT_RX_BUF[MQTT_Len++] = 0x64; //�������� LSB (100)  100s֮�� ������ϵ 
	
	/*��Ч�غ�*/
	/********************�ͻ��˱�ʶ��*********************/
	MQTT_RX_BUF[MQTT_Len++] = 0x00;                      // Client ID �� MSB    
	MQTT_RX_BUF[MQTT_Len++] = BYTE0(MQTT_ClientID_Len);  // Client ID �� LSB  	
	memcpy(&MQTT_RX_BUF[MQTT_Len],MQTT_Client_ID,MQTT_ClientID_Len); //Client ID ������ 
	MQTT_Len += MQTT_ClientID_Len;
	
	/********************�û�������*********************/
	MQTT_RX_BUF[MQTT_Len++] = 0x00;		                                //�û���MSB   
	MQTT_RX_BUF[MQTT_Len++] = BYTE0(MQTT_UserName_Len);    	          //�û���LSB    
	memcpy(&MQTT_RX_BUF[MQTT_Len],MQTT_User_Name,MQTT_UserName_Len);  //�û�������
	MQTT_Len += MQTT_UserName_Len;
	
	/********************���������*********************/
	MQTT_RX_BUF[MQTT_Len++] = 0x00;		                              //�����MSB    
	MQTT_RX_BUF[MQTT_Len++] = BYTE0(MQTT_Password_Len);    	        //�����LSB  
	memcpy(&MQTT_RX_BUF[MQTT_Len],MQTT_Password,MQTT_Password_Len); //���������
	MQTT_Len += MQTT_Password_Len;   
	
	
	MQTT_SendData(MQTT_RX_BUF,MQTT_Len);//���Ͷ�Ӧ�����ݵ��ƶ˽������ĵ�¼
}

/**
 * @brief MQTTָ��Ͽ��Ʒ�����
 *
 * @return û�з��ز���
 */
void MQTT_Disconnect(void)
{
	MQTT_Clear();
	MQTT_RX_BUF[0] = 0xe0; 
	MQTT_RX_BUF[1] = 0x00; 
	MQTT_SendData(MQTT_RX_BUF,2);//���Ͷ�Ӧ�����ݵ��ƶ˽������ĶϿ�����
}

/**
 * @brief ͨ��MQTT������Ӧ���ݵ��Ʒ�����
 * @param humi ʪ������
 * @param temp �¶�����
 * @return û�з��ز���
 */
//void MQTT_PublicTopic(float temp,float humi) 
void MQTT_PublicTopic(float temp, float humi, const char* co_str, const char* door_str, uint16_t light_val, const char* people_str)
{
    char SendData[512];
    MQTT_Clear();
    uint8_t MQTT_PublishTopic_Len = strlen(PublishTopic); //���㷢������ĳ���
    sprintf(SendData, MQTTPUBLISH(temp, humi, co_str, door_str, light_val, people_str));  //���������������onejson��ʽ        
    uint8_t MQTT_MQTTPUBLISH_Len = strlen(SendData);  //���㷢�����ݵĳ���
    
    MQTT_RX_BUF[MQTT_Len++] = 0x30;  //�̶��ı�ͷ
    
    DataLen = (MQTT_PublishTopic_Len + 2) + MQTT_MQTTPUBLISH_Len;
    /**����ʣ�೤��**/
    do
    {
        uint8_t encodedByte = DataLen % 128;
        DataLen = DataLen / 128;
        if ( DataLen > 0 )
            encodedByte = encodedByte | 128;
        MQTT_RX_BUF[MQTT_Len++] = encodedByte;
        
    }while ( DataLen > 0 );

    
    MQTT_RX_BUF[MQTT_Len++] = 0x00;  //��������MSB
    MQTT_RX_BUF[MQTT_Len++] = BYTE0(MQTT_PublishTopic_Len);  //���������LSB
    memcpy(&MQTT_RX_BUF[MQTT_Len], PublishTopic, MQTT_PublishTopic_Len);  //��ӷ������⵽������������
    MQTT_Len += MQTT_PublishTopic_Len;
    
    
    //���ø�����
    
    memcpy(&MQTT_RX_BUF[MQTT_Len], SendData, MQTT_MQTTPUBLISH_Len);  //��ӷ������ݵ���������
    MQTT_Len += MQTT_MQTTPUBLISH_Len;
    
    MQTT_SendData(MQTT_RX_BUF, MQTT_Len);  //����MQTT�ķ������ݵ�������
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



/**
 * @brief MQTTָ��ķ�����
 *
 * @return û�з��ز���
 */
void MQTT_SubscribeTopic(void)
{
	MQTT_Clear();
	uint8_t MQTT_SubscribeTopic_Len = strlen(SubscribeTopic_Server);//���㶩������ĳ���
	
  DataLen = 2 + (MQTT_SubscribeTopic_Len+2) + 1;
	
	/*�̶���ͷ*/
	MQTT_RX_BUF[MQTT_Len++] = 0x82;
	/**ʣ�೤��**/
	do
	{
		uint8_t encodedByte = DataLen % 128;
		DataLen = DataLen / 128;
		if ( DataLen > 0 )
			encodedByte = encodedByte | 128;
		MQTT_RX_BUF[MQTT_Len++] = encodedByte;
	}while ( DataLen > 0 );
	
	/*�ɱ䱨ͷ*/
	MQTT_RX_BUF[MQTT_Len++] = 0x00;  //�û��Զ��� 
	MQTT_RX_BUF[MQTT_Len++] = 0x01;  //�û��Զ���
	MQTT_RX_BUF[MQTT_Len++] = 0x00;  //���ĵ�MSB

	MQTT_RX_BUF[MQTT_Len++] = MQTT_SubscribeTopic_Len;  //���ĵ�LSB
	memcpy(&MQTT_RX_BUF[MQTT_Len],SubscribeTopic_Server,MQTT_SubscribeTopic_Len);  //��Ӷ��������·��
	MQTT_Len += MQTT_SubscribeTopic_Len;
	MQTT_RX_BUF[MQTT_Len++] = 0x00; //��������ȼ�ΪQ0
	
	MQTT_SendData(MQTT_RX_BUF,MQTT_Len);  //����MQTT����ָ��Ʒ�����
	
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/*
void MQTT_SubscribeTopic_web(void)
{
	MQTT_Clear();
	uint8_t MQTT_SubscribeTopic_Len = strlen(SubscribeTopic_Server_web);//���㶩������ĳ���
	
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
	
	
	MQTT_RX_BUF[MQTT_Len++] = 0x00;  //�û��Զ��� 
	MQTT_RX_BUF[MQTT_Len++] = 0x01;  //�û��Զ���
	MQTT_RX_BUF[MQTT_Len++] = 0x00;  //���ĵ�MSB

	MQTT_RX_BUF[MQTT_Len++] = MQTT_SubscribeTopic_Len;  //���ĵ�LSB
	memcpy(&MQTT_RX_BUF[MQTT_Len],SubscribeTopic_Server_web,MQTT_SubscribeTopic_Len);  //��Ӷ��������·��
	MQTT_Len += MQTT_SubscribeTopic_Len;
	MQTT_RX_BUF[MQTT_Len++] = 0x00; //��������ȼ�ΪQ0
	
	MQTT_SendData(MQTT_RX_BUF,MQTT_Len);  //����MQTT����ָ��Ʒ�����
	
}
*/
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/**
 * @brief MQTTָ��ȡ�����Ķ�Ӧ������
 *
 * @return û�з��ز���
 */
void MQTT_UNSubscribeTopic(void)
{
	MQTT_Clear();
	uint8_t MQTT_SubscribeTopic_Len = strlen(SubscribeTopic_Server_Reply);//����ȡ����������ĳ���

  DataLen = 2 + (MQTT_SubscribeTopic_Len+2);
	
	/*�̶���ͷ*/
	MQTT_RX_BUF[MQTT_Len++] = 0xA2;
	/*ʣ�೤��*/
	do
	{
		uint8_t encodedByte = DataLen % 128;
		DataLen = DataLen / 128;
		if ( DataLen > 0 )
			encodedByte = encodedByte | 128;
		MQTT_RX_BUF[MQTT_Len++] = encodedByte;
	}while ( DataLen > 0 );
	
	/*�ɱ䱨ͷ*/
	MQTT_RX_BUF[MQTT_Len++] = 0x00;  //�û��Զ���
	MQTT_RX_BUF[MQTT_Len++] = 0x01;  //�û��Զ���
	
	MQTT_RX_BUF[MQTT_Len++] = 0x00;  //���ĵ�MSB
	
	MQTT_RX_BUF[MQTT_Len++] = MQTT_SubscribeTopic_Len;  //ȡ�����ĵ�LSB
	memcpy(&MQTT_RX_BUF[MQTT_Len],SubscribeTopic_Server_Reply,MQTT_SubscribeTopic_Len);  //���ȡ�����������·��
	MQTT_Len += MQTT_SubscribeTopic_Len;
	
	MQTT_SendData(MQTT_RX_BUF,MQTT_Len);  //����MQTT����ָ��Ʒ�����
}

