#include "HallSensor.h"

// 霍尔传感器连接定义
#define HALL_SENSOR_GPIO_PORT  GPIOC
#define HALL_SENSOR_GPIO_PIN   GPIO_Pin_6
#define HALL_SENSOR_RCC        RCC_AHB1Periph_GPIOC

// 去抖动参数
#define DEBOUNCE_DELAY_MS  10  // 去抖动延迟(毫秒)
#define CONFIRM_COUNT      3   // 状态确认次数

// 简单延时函数
static void delay_ms(uint32_t ms)
{
    for(uint32_t i = 0; i < ms * 1000; i++);
}

void HallSensor_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // 使能GPIOC时钟
    RCC_AHB1PeriphClockCmd(HALL_SENSOR_RCC, ENABLE);
    
    // 配置PC6为输入模式，上拉电阻（确保无磁铁时为高电平）
    GPIO_InitStructure.GPIO_Pin = HALL_SENSOR_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  // 上拉电阻
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(HALL_SENSOR_GPIO_PORT, &GPIO_InitStructure);
}

uint8_t HallSensor_ReadStatus(void)
{
    // 读取PC6引脚状态
    // 注意：霍尔传感器3144在检测到磁铁时输出低电平，未检测到时输出高电平
    if(GPIO_ReadInputDataBit(HALL_SENSOR_GPIO_PORT, HALL_SENSOR_GPIO_PIN) == Bit_RESET)
    {
        return DOOR_CLOSED;      // 检测到磁铁，门关闭
    }
    else
    {
        return DOOR_NOT_CLOSED;  // 未检测到磁铁，门未关闭
    }
}

// 改进的读取函数，增加去抖动和状态确认
uint8_t HallSensor_ReadStableStatus(void)
{
    static uint8_t last_status = DOOR_NOT_CLOSED;
    uint8_t current_status;
    uint8_t confirm_count = 0;
    
    // 读取当前状态
    current_status = HallSensor_ReadStatus();
    
    // 如果状态与上次相同，直接返回
    if(current_status == last_status)
    {
        return current_status;
    }
    
    // 状态变化，进行去抖动和确认
    for(int i = 0; i < CONFIRM_COUNT; i++)
    {
        delay_ms(DEBOUNCE_DELAY_MS);
        if(HallSensor_ReadStatus() == current_status)
        {
            confirm_count++;
        }
    }
    
    // 如果确认次数足够，更新状态
    if(confirm_count >= CONFIRM_COUNT)
    {
        last_status = current_status;
    }
    
    return last_status;
}