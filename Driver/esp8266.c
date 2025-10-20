#include "esp8266.h"
// 将这些放在 esp8266.c 顶部（全局）
#include <string.h>
volatile char recv_buf[512];
volatile uint16_t cnt = 0;
volatile uint16_t esp8266_cntPre = 0;



//usart3发送和接收数组
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
 * @brief ESP8266初始化
 * 通过AT指令设置esp8266模式并连接云端
 *
 * @return 没有返回参数

	Wifi模块
  蓝牙模块
	4G模块   
  
  串口通讯  ---  支持 AT 指令      AT\r\n   
	
  一堆 由  AT开头字符串  
	  
 */
void ESP8266_Init(void)
{

	ESP8266_memset_RecvBuff();
	/*退出透传模式*/
	while(ESP8266_SendCmd("+++",""));
	delay_ms(500);
	/*测试ESP8266是否能正常进行AT控制*/
	printf("AT \r\n");  //USART1 
	while(ESP8266_SendCmd("AT\r\n","OK"));
	delay_ms(500);
	/*ESP8266进行重新启动*/
	
	printf("AT+RST \r\n");
	while(ESP8266_SendCmd("AT+RST\r\n",""));
	//while(ESP8266_SendCmd("AT+RESTORE\r\n",""));
	delay_ms(500);
	/*清除数据回传显示输入命令*/
	//printf("ATE0\r\n");
	//while(ESP8266_SendCmd("ATE0\r\n","OK"));
	//Delay_ms(500);
	printf("ESP8266 Init OK\r\n");
	
	
	/*******************以下是连接wifi设备*******************************/
	/*设置ESP8266为STA模式（客户端模式）*/
	printf("AT+CWMODE_CUR\r\n");
	while(ESP8266_SendCmd("AT+CWMODE_CUR=1\r\n", "OK"))   //第二个参数   判断 本次AT指令执行后  , WIFI模块的返回值是不是ok
	delay_ms(3000);
	/*设置对应wifi的账号和密码 */
	printf("AT+CWJAP\r\n");
	while(ESP8266_SendCmd(ESP8266_WIFI_INFO, "GOT IP"))
	delay_ms(3000);
	printf("ESP8266 连接WIFI成功！！！\r\n");
	
	/*****************连接云服务器******************************/
	ESP8266_ConnectServer();
	printf("ESP8266 连接云服务器成功！！！\r\n");
	
	/*********************登录操作******************************/
	do{
		ESP8266_memset_RecvBuff();
		MQTT_Connect();
		delay_ms(500);
	/****************通过串口返回判断是否订阅成功  固定的******************/
		if(recv_buf[0] == 0x20 && recv_buf[1] == 0x02)
		{
			printf("登录云服务器的MQTT成功！！！\r\n");
			connect_ack = 0;
		}
	}while(connect_ack != 0);
	connect_ack = 1;

	/*****************订阅云服务操作******************************/
	do{
		ESP8266_memset_RecvBuff();
		
		MQTT_SubscribeTopic();
		delay_ms(500);
		//MQTT_SubscribeTopic_web();
		//Delay_ms(500);
		
		
		/****************通过串口返回判断是否订阅成功******************/
		if((recv_buf[0] == 0x90 && recv_buf[1] == 0x03 && recv_buf[2] == 0x00 && recv_buf[3] == 0x01 && recv_buf[4] == 0x00)||(recv_buf[0] == 0x90 && recv_buf[1] == 0x03 && recv_buf[2] == 0x00 && recv_buf[3] == 0x01 && recv_buf[4] == 0x01))
		{
			printf("订阅MQTT主题成功！！！\r\n");
			connect_ack = 0;
		}
	}while(connect_ack != 0);
	connect_ack = 1;
	/*********************取消订阅********************************/
//	MQTT_UNSubscribeTopic();
	/*****************断开云服务操作******************************/
//	MQTT_Disconnect();
//  printf("MQTT断开服务器成功！！！\r\n");
//	printf("\r\n");
}


/**
 * @brief 清除缓冲区数据内容
 * 
 *
 * @return 没有返回参数
 */
void ESP8266_memset_RecvBuff(void)
{
    __disable_irq(); // 禁用中断，防止 race
    memset(usart3_txbuf, 0, sizeof(usart3_txbuf));
    memset(usart3_rxbuf, 0, sizeof(usart3_rxbuf));
    memset((void*)recv_buf, 0, sizeof(recv_buf));
    cnt = 0;
    esp8266_cntPre = 0;
    __enable_irq();
}

/**
 * @brief ESP8266发送命令
 * 通过串口向ESP8266传输控制命令并核对返回结果是否正确
 *
 * @return 成功返回0，失败返回1
 *
*/_Bool ESP8266_SendCmd(char *str, char* rev)
{
    uint16_t timeOut = 200; // 200 * 10ms = ~2s 超时，根据需要调整
    ESP8266_memset_RecvBuff();  // 清空缓存
    Usart_SendString(USART3, str); // 发送指令

    while (timeOut--)
    {
        if (ESP8266_WaitRecive() == REV_OK)
        {
            __disable_irq(); // 禁中断，确保 recv_buf 不被修改（临界区要短）
            char *p = strstr((char *)recv_buf, rev);
            __enable_irq();
            if (p != NULL)
            {
                ESP8266_memset_RecvBuff();
                return 0; // 找到期待应答
            }
        }
        delay_ms(10);
    }
    return 1; // 超时或失败
}


/**
 * @brief ESP8266打开透传模式
 * 
 *
 * @return 没有返回结果
 */
void ESP8266_OpenTransmission(void)
{
	//设置透传模式
		memset(usart3_rxbuf,0,sizeof(usart3_rxbuf));    
		while(ESP8266_SendCmd("AT+CIPMODE=1\r\n","OK"))
		delay_ms(500);  
}


/**
 * @brief ESP8266退出透传模式
 * 
 *
 * @return 没有返回结果
 */
void ESP8266_ExitUnvarnishedTrans(void)
{
	while(ESP8266_SendCmd("+++",""))
	delay_ms(500);
}



/**
 * @brief ESP8266连接云服务器
 * 
 *
 * @return 没有返回结果
 */
void ESP8266_ConnectServer(void)
{
	//连接服务器
	printf("AT+CIPSTART");
	while(ESP8266_SendCmd(ESP8266_SERVER,"CONNECT"))
	delay_ms(500);

	//设置透传模式
	ESP8266_OpenTransmission();
	
	//开启发送状态
	printf("AT+CIPSEND"); 
	while(ESP8266_SendCmd("AT+CIPSEND\r\n",">"))
	delay_ms(500);//开始处于透传发送状态

}

/**
 * @brief 串口3的中断回调函数
 * 接收ESP8266发送的消息打印到PC端，并对服务器下发数据进行处理
 *
 * @return 没有返回结果
 */
void USART3_IRQHandler(void)
{
    if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
        uint8_t data = (uint8_t)USART_ReceiveData(USART3); // 只读一次 DR
        // 边界检查并写入（给末尾留一个位置用于 '\0'）
        if (cnt < (sizeof(recv_buf) - 1))
        {
            recv_buf[cnt++] = (char)data;
            recv_buf[cnt] = '\0'; // 立即保持字符串终止，strstr 安全
        }
        else
        {
            // 缓冲区满了：重置计数，避免溢出（也可选择丢弃数据）
            cnt = 0;
            recv_buf[cnt] = '\0';
        }
        USART_ClearITPendingBit(USART3, USART_IT_RXNE);

        // **重要**：不要在中断里面做复杂处理（如 MQTT_RX_DATE_DEAL 或 printf）
        // 如果需要上报，设置一个标志位，在主循环处理
    }
}



/**
 * @brief 等待接收到的数据
 * 
 *
 * @return 返回接收完成标志/未完成标志
 */_Bool ESP8266_WaitRecive(void)
{
    if (cnt == 0) // 还没收到任何字节
        return REV_WAIT;

    if (cnt == esp8266_cntPre) // 两次检查到相同长度，认为接收稳定
    {
        // 已经在 IRQ 中保证了 recv_buf 的 '\0'，这里再保险处理：
        if (cnt >= sizeof(recv_buf)) recv_buf[sizeof(recv_buf)-1] = '\0';
        esp8266_cntPre = 0; // 重置历史计数
        return REV_OK;
    }

    esp8266_cntPre = cnt;
    return REV_WAIT;
}


/**
 * @brief 接收数据处理
 * 
 *
 * @return 没有返回参数
 */
void MQTT_RX_DATE_DEAL(char *buf)   // {"xxxx":"xxx","LED":1}
{
	//printf("iszoulema");
	char *ptr;
	ptr = strstr(buf,"LED");
 // 使用strstr函数找到"LED"的起始位置
  ptr = strstr(buf, "\"LED\"");
  if (ptr != NULL) {
    // 找到了"LED"的起始位置，向后查找":"的位置
    ptr = strchr(ptr, ':');
    if (ptr != NULL) {
      // 向后查找数字的位置
      ptr++;
      while (*ptr == ' ') {
        ptr++;
      }
      // 判断数字是否为0或1
      if (*ptr == '0') {
        printf("LED的值为0\n");
				memset(recv_buf,0,sizeof(recv_buf));
				GPIO_ResetBits(GPIOG, GPIO_Pin_14); //点亮led灯
      } else if (*ptr == '1') {
        printf("LED的值为1\n");
				memset(recv_buf,0,sizeof(recv_buf));
				GPIO_SetBits(GPIOG, GPIO_Pin_14); //熄灭led灯
      } 
    }
  }
		ptr = NULL;
}

