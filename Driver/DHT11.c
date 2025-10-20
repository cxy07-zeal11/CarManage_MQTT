#include "DHT11.h"

// DHT11-�������
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

// DHT11-��������
void DHT11_IN(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOG, &GPIO_InitStruct);
}

// ����DHT11�����͵�ƽ20ms������
void Start_DHT11(void)
{
    // ����Ϊ���ģʽ
    DHT11_OUT_Config();
    
    // ���͵�ƽ20ms
    GPIO_ResetBits(GPIOG, GPIO_Pin_9);
    delay_ms(20);
    
    // ���ߵ�ƽ30us
    GPIO_SetBits(GPIOG, GPIO_Pin_9);
    delay_us(30);
}

// DHT11Ӧ���źż��
// ����1��ʾӦ���źŴ��󣬷���0��ʾӦ���ź���ȷ
uint8_t DHT11_ACK(void)
{
    uint16_t count;
    DHT11_IN();
    
    // Ӧ���ź�Ӧ���ǵ͵�ƽ��ʼ
    if (GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_9) == 0)
    {
        count = 0;
        
        // �жϵ͵�ƽ����ʱ���Ƿ���������Χ��
        while (GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_9) == 0)
        {
            count++;
            if (count > 10)
            {
                printf("�͵�ƽ����ʱ��̫��\n");
                return 1;
            }
            delay_us(10);
        }
        
        count = 0;
        
        // �жϸߵ�ƽ����ʱ���Ƿ���������Χ��
        while (GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_9) == 1)
        {
            count++;
            if (count > 9)
            {
                printf("�ߵ�ƽ����ʱ��̫��\n");
                return 1;
            }
            delay_us(10);
        }
        
        // ��������������裬����0��ʾӦ���ź���ȷ
        return 0;
    }
    
    printf("��ȡ��Ӧ���źŵ�ƽ���ǵ͵�ƽ\n");
    return 1;
}

// ��ȡһ���ֽڵ�����
uint8_t Read_Byte(void)
{
    uint8_t i, temp = 0;
    
    for (i = 0; i < 8; i++)
    {
        // �ȴ������߱�Ϊ�ߵ�ƽ
        while (GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_9) == 0);
    
        delay_us(35);
        
        // ��ȡ���ߵ�ƽ��ʾ����λΪ1
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
//             printf("Read DHT11 ��ȷ!\r\n");
//             printf("ʪ�ȣ�%d.%dRH,�¶ȣ�%d.%d��C.\r\n", humi_int, humi_deci, temp_int, temp_deci);

//        }
//        
//        //����¶ȸ���27����1���ơ��¶ȸ���28����2���ơ��¶ȸ���29����������
//        
//    }
//    else
//    {
//        printf("Read DHT11 ERROR!\r\n");
//    }
//    
//    delay_ms(200);
//}



// ��ȡDHT11��ʪ������
struct DHT11_Complete_Data_TypeDef Get_DHT11_Data(void)
{
    struct DHT11_Complete_Data_TypeDef DHT11_Complete_Data;
    
    // ����DHT11����ȡ����
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
        
        // У����ж�
        if (check_sum == humi_int + humi_deci + temp_int + temp_deci)
        {
            DHT11_Complete_Data.humi = humi_int + humi_deci / 100.0;
            DHT11_Complete_Data.temp = temp_int + temp_deci / 100.0;
         //   printf("��ȡDHT11�ɹ���ʪ��Ϊ%0.2lf%%���¶�Ϊ%0.2lf��C��\r\n", DHT11_Complete_Data.humi, DHT11_Complete_Data.temp);
        }
        else
        {
            printf("DHT11����У��ʹ���\r\n");
            DHT11_Complete_Data.humi = 0.0;
            DHT11_Complete_Data.temp = 0.0;
        }
    }
    else
    {
        printf("��ȡDHT11ʧ�ܣ�\r\n");
        DHT11_Complete_Data.humi = 0.0;
        DHT11_Complete_Data.temp = 0.0;
    }
    
    // DHT11��ͨѶʱ�����3ms���������������100ms
    delay_ms(1000);
    
    return DHT11_Complete_Data;
}
