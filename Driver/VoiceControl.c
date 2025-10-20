#include "VoiceControl.h"
#include "SERVO.h"
#include "LED.h"
#include "Delay.h"

// ����������νǶ����������������ά��
static int tilt_sequence[] = {30, 60, 90};  
static int tilt_index = 0;

// ��ӵƵ�״̬����
static uint8_t top_light_status = 0;    // 0:��, 1:��
static uint8_t ambient_light_status = 0; // 0:��, 1:��

void VoiceControl_HandleCommand(uint8_t cmd)
{
    switch (cmd)
    {
        case 0x01:  // ������
            SERVO360_SetSpeed(60); 
            delay_s(2);
            SERVO360_Stop();
            break;

        case 0x02:  // �س���
            SERVO360_SetSpeed(-60); 
            delay_s(2);
            SERVO360_Stop();
            break;

        case 0x03:  // �򿪶���
            if (top_light_status == 0) {  // ֻ�й��ŵ�ʱ��ſ�
                GPIO_ResetBits(LED_GROUP, LED1_PIN);
                top_light_status = 1;
            }
            break;

        case 0x04:  // �򿪷�Χ��
            if (ambient_light_status == 0) {  // ֻ�й��ŵ�ʱ��ſ�
                LED_Water();
                ambient_light_status = 1;
            }
            break;

        case 0x05:  // ��������
            SERVO_TEXT(tilt_sequence[tilt_index]); 
            tilt_index = (tilt_index + 1) % (sizeof(tilt_sequence) / sizeof(tilt_sequence[0]));
            break;

        case 0x06:  // �رն���
            if (top_light_status == 1) {  // ֻ�п��ŵ�ʱ��Ź�
                GPIO_SetBits(LED_GROUP, LED1_PIN);
                top_light_status = 0;
            }
            break;

        case 0x07:  // �رշ�Χ��
            if (ambient_light_status == 1) {  // ֻ�п��ŵ�ʱ��Ź�
                GPIO_SetBits(LED_GROUP, LED2_PIN);
                ambient_light_status = 0;
            }
            break;

        default:
            // δָ֪�����
            break;
    }
}