#include "DHT11.h"

// DHT11-输出配置
void DHT11_OUT_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
    
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOG, &GPIO_InitStruct);
}

// DHT11-输入配置
void DHT11_IN(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOG, &GPIO_InitStruct);
}

// 启动DHT11，拉低电平20ms后拉高
void Start_DHT11(void)
{
    // 配置为输出模式
    DHT11_OUT_Config();
    
    // 拉低电平20ms
    GPIO_ResetBits(GPIOG, GPIO_Pin_9);
    delay_ms(20);
    
    // 拉高电平30us
    GPIO_SetBits(GPIOG, GPIO_Pin_9);
    delay_us(30);
}

// DHT11应答信号检查
// 返回1表示应答信号错误，返回0表示应答信号正确
uint8_t DHT11_ACK(void)
{
    uint16_t count;
    DHT11_IN();
    
    // 应答信号应该是低电平开始
    if (GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_9) == 0)
    {
        count = 0;
        
        // 判断低电平持续时间是否在正常范围内
        while (GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_9) == 0)
        {
            count++;
            if (count > 10)
            {
                printf("低电平持续时间太长\n");
                return 1;
            }
            delay_us(10);
        }
        
        count = 0;
        
        // 判断高电平持续时间是否在正常范围内
        while (GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_9) == 1)
        {
            count++;
            if (count > 9)
            {
                printf("高电平持续时间太长\n");
                return 1;
            }
            delay_us(10);
        }
        
        // 如果经过上述步骤，返回0表示应答信号正确
        return 0;
    }
    
    printf("读取的应答信号电平不是低电平\n");
    return 1;
}

// 读取一个字节的数据
uint8_t Read_Byte(void)
{
    uint8_t i, temp = 0;
    
    for (i = 0; i < 8; i++)
    {
        // 等待数据线变为高电平
        while (GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_9) == 0);
    
        delay_us(35);
        
        // 读取到高电平表示数据位为1
        if (GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_9) == 1)
        {
            while (GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_9) == 1);
            temp |= (uint8_t)(0x01 << (7 - i));
        }
        else
        {
            temp &= (uint8_t)~(0x01 << (7 - i));
        }
    }
    
    return temp;
}


uint8_t humi_int = 0;
uint8_t humi_deci = 0;
uint8_t temp_int = 0;
uint8_t temp_deci = 0;
uint8_t check_sum = 0;



//void DHT11_Read_Data(void)
//{
//    Start_DHT11();
//    
//    if( DHT11_ACK() == 0)
//    {
//        uint8_t humi_int = Read_Byte();
//        uint8_t humi_deci = Read_Byte();
//        uint8_t temp_int = Read_Byte();
//        uint8_t temp_deci = Read_Byte();
//        uint8_t check_sum = Read_Byte();
//        
//        
//        DHT11_OUT_Config();
//        GPIO_SetBits( GPIOG, GPIO_Pin_9 );
//        
//        if( check_sum == humi_int + humi_deci + temp_int +temp_deci )
//        {
//             printf("Read DHT11 正确!\r\n");
//             printf("湿度：%d.%dRH,温度：%d.%d°C.\r\n", humi_int, humi_deci, temp_int, temp_deci);

//        }
//        
//        //如果温度高于27度亮1个灯、温度高于28度亮2个灯、温度高于29度亮三个灯
//        
//    }
//    else
//    {
//        printf("Read DHT11 ERROR!\r\n");
//    }
//    
//    delay_ms(200);
//}



// 获取DHT11温湿度数据
struct DHT11_Complete_Data_TypeDef Get_DHT11_Data(void)
{
    struct DHT11_Complete_Data_TypeDef DHT11_Complete_Data;
    
    // 启动DHT11并读取数据
    Start_DHT11();
    
    if (DHT11_ACK() == 0)
    {
        uint8_t humi_int = Read_Byte();
        uint8_t humi_deci = Read_Byte();
        uint8_t temp_int = Read_Byte();
        uint8_t temp_deci = Read_Byte();
        uint8_t check_sum = Read_Byte();
        
        DHT11_OUT_Config();
        GPIO_SetBits(GPIOG, GPIO_Pin_9);
        
        // 校验和判断
        if (check_sum == humi_int + humi_deci + temp_int + temp_deci)
        {
            DHT11_Complete_Data.humi = humi_int + humi_deci / 100.0;
            DHT11_Complete_Data.temp = temp_int + temp_deci / 100.0;
         //   printf("读取DHT11成功！湿度为%0.2lf%%，温度为%0.2lf°C。\r\n", DHT11_Complete_Data.humi, DHT11_Complete_Data.temp);
        }
        else
        {
            printf("DHT11数据校验和错误！\r\n");
            DHT11_Complete_Data.humi = 0.0;
            DHT11_Complete_Data.temp = 0.0;
        }
    }
    else
    {
        printf("读取DHT11失败！\r\n");
        DHT11_Complete_Data.humi = 0.0;
        DHT11_Complete_Data.temp = 0.0;
    }
    
    // DHT11的通讯时间最大3ms，采样间隔不少于100ms
    delay_ms(1000);
    
    return DHT11_Complete_Data;
}
