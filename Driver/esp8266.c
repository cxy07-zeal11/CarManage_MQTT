#include "esp8266.h"
// ����Щ���� esp8266.c ������ȫ�֣�
#include <string.h>
volatile char recv_buf[512];
volatile uint16_t cnt = 0;
volatile uint16_t esp8266_cntPre = 0;



//usart3���ͺͽ�������
uint8_t usart3_txbuf[256];
uint8_t usart3_rxbuf[512];
uint8_t usart3_rxcounter;

uint8_t num = 0;

volatile uint32_t	Rx3Counter	= 0;
volatile uint8_t	Rx3Data 	= 0;
volatile uint8_t	Rx3End 		= 0;
volatile uint8_t	Tx3Buffer[512]={0};
volatile uint8_t	Rx3Buffer[512]={0};

_Bool connect_ack = 1;

/**
 * @brief ESP8266��ʼ��
 * ͨ��ATָ������esp8266ģʽ�������ƶ�
 *
 * @return û�з��ز���

	Wifiģ��
  ����ģ��
	4Gģ��   
  
  ����ͨѶ  ---  ֧�� AT ָ��      AT\r\n   
	
  һ�� ��  AT��ͷ�ַ���  
	  
 */
void ESP8266_Init(void)
{

	ESP8266_memset_RecvBuff();
	/*�˳�͸��ģʽ*/
	while(ESP8266_SendCmd("+++",""));
	delay_ms(500);
	/*����ESP8266�Ƿ�����������AT����*/
	printf("AT \r\n");  //USART1 
	while(ESP8266_SendCmd("AT\r\n","OK"));
	delay_ms(500);
	/*ESP8266������������*/
	
	printf("AT+RST \r\n");
	while(ESP8266_SendCmd("AT+RST\r\n",""));
	//while(ESP8266_SendCmd("AT+RESTORE\r\n",""));
	delay_ms(500);
	/*������ݻش���ʾ��������*/
	//printf("ATE0\r\n");
	//while(ESP8266_SendCmd("ATE0\r\n","OK"));
	//Delay_ms(500);
	printf("ESP8266 Init OK\r\n");
	
	
	/*******************����������wifi�豸*******************************/
	/*����ESP8266ΪSTAģʽ���ͻ���ģʽ��*/
	printf("AT+CWMODE_CUR\r\n");
	while(ESP8266_SendCmd("AT+CWMODE_CUR=1\r\n", "OK"))   //�ڶ�������   �ж� ����ATָ��ִ�к�  , WIFIģ��ķ���ֵ�ǲ���ok
	delay_ms(3000);
	/*���ö�Ӧwifi���˺ź����� */
	printf("AT+CWJAP\r\n");
	while(ESP8266_SendCmd(ESP8266_WIFI_INFO, "GOT IP"))
	delay_ms(3000);
	printf("ESP8266 ����WIFI�ɹ�������\r\n");
	
	/*****************�����Ʒ�����******************************/
	ESP8266_ConnectServer();
	printf("ESP8266 �����Ʒ������ɹ�������\r\n");
	
	/*********************��¼����******************************/
	do{
		ESP8266_memset_RecvBuff();
		MQTT_Connect();
		delay_ms(500);
	/****************ͨ�����ڷ����ж��Ƿ��ĳɹ�  �̶���******************/
		if(recv_buf[0] == 0x20 && recv_buf[1] == 0x02)
		{
			printf("��¼�Ʒ�������MQTT�ɹ�������\r\n");
			connect_ack = 0;
		}
	}while(connect_ack != 0);
	connect_ack = 1;

	/*****************�����Ʒ������******************************/
	do{
		ESP8266_memset_RecvBuff();
		
		MQTT_SubscribeTopic();
		delay_ms(500);
		//MQTT_SubscribeTopic_web();
		//Delay_ms(500);
		
		
		/****************ͨ�����ڷ����ж��Ƿ��ĳɹ�******************/
		if((recv_buf[0] == 0x90 && recv_buf[1] == 0x03 && recv_buf[2] == 0x00 && recv_buf[3] == 0x01 && recv_buf[4] == 0x00)||(recv_buf[0] == 0x90 && recv_buf[1] == 0x03 && recv_buf[2] == 0x00 && recv_buf[3] == 0x01 && recv_buf[4] == 0x01))
		{
			printf("����MQTT����ɹ�������\r\n");
			connect_ack = 0;
		}
	}while(connect_ack != 0);
	connect_ack = 1;
	/*********************ȡ������********************************/
//	MQTT_UNSubscribeTopic();
	/*****************�Ͽ��Ʒ������******************************/
//	MQTT_Disconnect();
//  printf("MQTT�Ͽ��������ɹ�������\r\n");
//	printf("\r\n");
}


/**
 * @brief �����������������
 * 
 *
 * @return û�з��ز���
 */
void ESP8266_memset_RecvBuff(void)
{
    __disable_irq(); // �����жϣ���ֹ race
    memset(usart3_txbuf, 0, sizeof(usart3_txbuf));
    memset(usart3_rxbuf, 0, sizeof(usart3_rxbuf));
    memset((void*)recv_buf, 0, sizeof(recv_buf));
    cnt = 0;
    esp8266_cntPre = 0;
    __enable_irq();
}

/**
 * @brief ESP8266��������
 * ͨ��������ESP8266�����������˶Է��ؽ���Ƿ���ȷ
 *
 * @return �ɹ�����0��ʧ�ܷ���1
 *
*/_Bool ESP8266_SendCmd(char *str, char* rev)
{
    uint16_t timeOut = 200; // 200 * 10ms = ~2s ��ʱ��������Ҫ����
    ESP8266_memset_RecvBuff();  // ��ջ���
    Usart_SendString(USART3, str); // ����ָ��

    while (timeOut--)
    {
        if (ESP8266_WaitRecive() == REV_OK)
        {
            __disable_irq(); // ���жϣ�ȷ�� recv_buf �����޸ģ��ٽ���Ҫ�̣�
            char *p = strstr((char *)recv_buf, rev);
            __enable_irq();
            if (p != NULL)
            {
                ESP8266_memset_RecvBuff();
                return 0; // �ҵ��ڴ�Ӧ��
            }
        }
        delay_ms(10);
    }
    return 1; // ��ʱ��ʧ��
}


/**
 * @brief ESP8266��͸��ģʽ
 * 
 *
 * @return û�з��ؽ��
 */
void ESP8266_OpenTransmission(void)
{
	//����͸��ģʽ
		memset(usart3_rxbuf,0,sizeof(usart3_rxbuf));    
		while(ESP8266_SendCmd("AT+CIPMODE=1\r\n","OK"))
		delay_ms(500);  
}


/**
 * @brief ESP8266�˳�͸��ģʽ
 * 
 *
 * @return û�з��ؽ��
 */
void ESP8266_ExitUnvarnishedTrans(void)
{
	while(ESP8266_SendCmd("+++",""))
	delay_ms(500);
}



/**
 * @brief ESP8266�����Ʒ�����
 * 
 *
 * @return û�з��ؽ��
 */
void ESP8266_ConnectServer(void)
{
	//���ӷ�����
	printf("AT+CIPSTART");
	while(ESP8266_SendCmd(ESP8266_SERVER,"CONNECT"))
	delay_ms(500);

	//����͸��ģʽ
	ESP8266_OpenTransmission();
	
	//��������״̬
	printf("AT+CIPSEND"); 
	while(ESP8266_SendCmd("AT+CIPSEND\r\n",">"))
	delay_ms(500);//��ʼ����͸������״̬

}

/**
 * @brief ����3���жϻص�����
 * ����ESP8266���͵���Ϣ��ӡ��PC�ˣ����Է������·����ݽ��д���
 *
 * @return û�з��ؽ��
 */
void USART3_IRQHandler(void)
{
    if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
        uint8_t data = (uint8_t)USART_ReceiveData(USART3); // ֻ��һ�� DR
        // �߽��鲢д�루��ĩβ��һ��λ������ '\0'��
        if (cnt < (sizeof(recv_buf) - 1))
        {
            recv_buf[cnt++] = (char)data;
            recv_buf[cnt] = '\0'; // ���������ַ�����ֹ��strstr ��ȫ
        }
        else
        {
            // ���������ˣ����ü��������������Ҳ��ѡ�������ݣ�
            cnt = 0;
            recv_buf[cnt] = '\0';
        }
        USART_ClearITPendingBit(USART3, USART_IT_RXNE);

        // **��Ҫ**����Ҫ���ж����������Ӵ����� MQTT_RX_DATE_DEAL �� printf��
        // �����Ҫ�ϱ�������һ����־λ������ѭ������
    }
}



/**
 * @brief �ȴ����յ�������
 * 
 *
 * @return ���ؽ�����ɱ�־/δ��ɱ�־
 */_Bool ESP8266_WaitRecive(void)
{
    if (cnt == 0) // ��û�յ��κ��ֽ�
        return REV_WAIT;

    if (cnt == esp8266_cntPre) // ���μ�鵽��ͬ���ȣ���Ϊ�����ȶ�
    {
        // �Ѿ��� IRQ �б�֤�� recv_buf �� '\0'�������ٱ��մ���
        if (cnt >= sizeof(recv_buf)) recv_buf[sizeof(recv_buf)-1] = '\0';
        esp8266_cntPre = 0; // ������ʷ����
        return REV_OK;
    }

    esp8266_cntPre = cnt;
    return REV_WAIT;
}


/**
 * @brief �������ݴ���
 * 
 *
 * @return û�з��ز���
 */
void MQTT_RX_DATE_DEAL(char *buf)   // {"xxxx":"xxx","LED":1}
{
	//printf("iszoulema");
	char *ptr;
	ptr = strstr(buf,"LED");
 // ʹ��strstr�����ҵ�"LED"����ʼλ��
  ptr = strstr(buf, "\"LED\"");
  if (ptr != NULL) {
    // �ҵ���"LED"����ʼλ�ã�������":"��λ��
    ptr = strchr(ptr, ':');
    if (ptr != NULL) {
      // ���������ֵ�λ��
      ptr++;
      while (*ptr == ' ') {
        ptr++;
      }
      // �ж������Ƿ�Ϊ0��1
      if (*ptr == '0') {
        printf("LED��ֵΪ0\n");
				memset(recv_buf,0,sizeof(recv_buf));
				GPIO_ResetBits(GPIOG, GPIO_Pin_14); //����led��
      } else if (*ptr == '1') {
        printf("LED��ֵΪ1\n");
				memset(recv_buf,0,sizeof(recv_buf));
				GPIO_SetBits(GPIOG, GPIO_Pin_14); //Ϩ��led��
      } 
    }
  }
		ptr = NULL;
}

