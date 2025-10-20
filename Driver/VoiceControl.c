#include "VoiceControl.h"
#include "SERVO.h"
#include "LED.h"
#include "Delay.h"

// 假设你的座椅角度数组和索引在这里维护
static int tilt_sequence[] = {30, 60, 90};  
static int tilt_index = 0;

// 添加灯的状态变量
static uint8_t top_light_status = 0;    // 0:关, 1:开
static uint8_t ambient_light_status = 0; // 0:关, 1:开

void VoiceControl_HandleCommand(uint8_t cmd)
{
    switch (cmd)
    {
        case 0x01:  // 开车门
            SERVO360_SetSpeed(60); 
            delay_s(2);
            SERVO360_Stop();
            break;

        case 0x02:  // 关车门
            SERVO360_SetSpeed(-60); 
            delay_s(2);
            SERVO360_Stop();
            break;

        case 0x03:  // 打开顶灯
            if (top_light_status == 0) {  // 只有关着的时候才开
                GPIO_ResetBits(LED_GROUP, LED1_PIN);
                top_light_status = 1;
            }
            break;

        case 0x04:  // 打开氛围灯
            if (ambient_light_status == 0) {  // 只有关着的时候才开
                LED_Water();
                ambient_light_status = 1;
            }
            break;

        case 0x05:  // 调节座椅
            SERVO_TEXT(tilt_sequence[tilt_index]); 
            tilt_index = (tilt_index + 1) % (sizeof(tilt_sequence) / sizeof(tilt_sequence[0]));
            break;

        case 0x06:  // 关闭顶灯
            if (top_light_status == 1) {  // 只有开着的时候才关
                GPIO_SetBits(LED_GROUP, LED1_PIN);
                top_light_status = 0;
            }
            break;

        case 0x07:  // 关闭氛围灯
            if (ambient_light_status == 1) {  // 只有开着的时候才关
                GPIO_SetBits(LED_GROUP, LED2_PIN);
                ambient_light_status = 0;
            }
            break;

        default:
            // 未知指令，忽略
            break;
    }
}