#include "HallSensor.h"

// �������������Ӷ���
#define HALL_SENSOR_GPIO_PORT  GPIOC
#define HALL_SENSOR_GPIO_PIN   GPIO_Pin_6
#define HALL_SENSOR_RCC        RCC_AHB1Periph_GPIOC

// ȥ��������
#define DEBOUNCE_DELAY_MS  10  // ȥ�����ӳ�(����)
#define CONFIRM_COUNT      3   // ״̬ȷ�ϴ���

// ����ʱ����
static void delay_ms(uint32_t ms)
{
    for(uint32_t i = 0; i < ms * 1000; i++);
}

void HallSensor_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // ʹ��GPIOCʱ��
    RCC_AHB1PeriphClockCmd(HALL_SENSOR_RCC, ENABLE);
    
    // ����PC6Ϊ����ģʽ���������裨ȷ���޴���ʱΪ�ߵ�ƽ��
    GPIO_InitStructure.GPIO_Pin = HALL_SENSOR_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  // ��������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(HALL_SENSOR_GPIO_PORT, &GPIO_InitStructure);
}

uint8_t HallSensor_ReadStatus(void)
{
    // ��ȡPC6����״̬
    // ע�⣺����������3144�ڼ�⵽����ʱ����͵�ƽ��δ��⵽ʱ����ߵ�ƽ
    if(GPIO_ReadInputDataBit(HALL_SENSOR_GPIO_PORT, HALL_SENSOR_GPIO_PIN) == Bit_RESET)
    {
        return DOOR_CLOSED;      // ��⵽�������Źر�
    }
    else
    {
        return DOOR_NOT_CLOSED;  // δ��⵽��������δ�ر�
    }
}

// �Ľ��Ķ�ȡ����������ȥ������״̬ȷ��
uint8_t HallSensor_ReadStableStatus(void)
{
    static uint8_t last_status = DOOR_NOT_CLOSED;
    uint8_t current_status;
    uint8_t confirm_count = 0;
    
    // ��ȡ��ǰ״̬
    current_status = HallSensor_ReadStatus();
    
    // ���״̬���ϴ���ͬ��ֱ�ӷ���
    if(current_status == last_status)
    {
        return current_status;
    }
    
    // ״̬�仯������ȥ������ȷ��
    for(int i = 0; i < CONFIRM_COUNT; i++)
    {
        delay_ms(DEBOUNCE_DELAY_MS);
        if(HallSensor_ReadStatus() == current_status)
        {
            confirm_count++;
        }
    }
    
    // ���ȷ�ϴ����㹻������״̬
    if(confirm_count >= CONFIRM_COUNT)
    {
        last_status = current_status;
    }
    
    return last_status;
}