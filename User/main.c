#include "stm32f4xx.h"
#include "Delay.h"
#include "KEY.h"
#include "OLED.h"
#include "MQ7.h"
#include "ADC.h"
#include "UART.h"
#include "DHT11.h"
#include "LED.h"
#include "light_adc.h"
#include "HallSensor.h"
#include "esp8266.h"
#include "mqtt.h"
#include "SERVO.h"
#include "RED.h"
#include "VoiceControl.h"

#include <stdio.h>
#include <string.h>

#define HALL_DEBUG 0

typedef enum {
    STATE_WELCOME = 0,
    STATE_MENU_MAIN,
    STATE_SAFETY_MONITOR,
    STATE_SEAT_MENU,
    STATE_SEAT_FRONTBACK,
    STATE_SEAT_TILT
} AppState_t;

extern volatile uint8_t voice_cmd;
static int seat_position = 50;
static int seat_tilt = 10;
static const uint16_t tilt_sequence[] = {90, 150, 180, 150};
static int tilt_index = 0;

static int g_temp = 0;
static uint8_t co_motor_running = 0;
static uint8_t person_motor_running = 0;

// -------------------------------- 红外人体检测 --------------------------------
static inline uint8_t RED_ReadRaw(void)
{
    return (GPIO_ReadInputDataBit(RED_GROUP, RED_PIN) == Bit_RESET) ? 0 : 1;
}

static uint8_t RED_ReadStable(void)
{
    uint8_t cnt = 0;
    const int samples = 5;
    for (int i = 0; i < samples; i++) {
        if (RED_ReadRaw()) cnt++;
        delay_ms(5);
    }
    return (cnt >= (samples/2 + 1)) ? 1 : 0;
}

// -------------------------------- 门状态显示 --------------------------------
static const char* DoorStatusToStr(uint8_t status)
{
    return (status == DOOR_CLOSED) ? "Closed" : "Not Closed";
}

// -------------------------------- OLED 显示函数 --------------------------------
static void ShowWelcome(void)
{
    OLED_Clear();
    OLED_ShowString(16, 2, (uint8_t *)"Welcome to");
    OLED_ShowString(8, 4, (uint8_t *)"Car System");
}

static void ShowMainMenu(void)
{
    OLED_Clear();
    OLED_ShowString(0, 1, (uint8_t *)"1.Safety Monitor");
    OLED_ShowString(0, 3, (uint8_t *)"2.Seat Adjust");
    OLED_ShowString(0, 6, (uint8_t *)"K3:Back");
}

static void ShowSeatMenu(void)
{
    OLED_Clear();
    OLED_ShowString(0, 1, (uint8_t *)"1.Front/Back");
    OLED_ShowString(0, 3, (uint8_t *)"2.Tilt Adjust");
    OLED_ShowString(0, 6, (uint8_t *)"K3:Back");
}

static void ShowFrontBack(void)
{
    char buf[32];
    OLED_Clear();
    OLED_ShowString(0, 0, (uint8_t *)"Seat Position");
    snprintf(buf, sizeof(buf), "Pos: %3d       ", seat_position);
    OLED_ShowString(0, 2, (uint8_t *)buf);
    OLED_ShowString(0, 6, (uint8_t *)"K1:+  K2:-  K3:Back   ");
}

static void ShowTilt(void)
{
    char buf[32];
    OLED_Clear();
    OLED_ShowString(0, 0, (uint8_t *)"  Seat Tilt");
    snprintf(buf, sizeof(buf), "  Angle: %3d       ", seat_tilt);
    OLED_ShowString(0, 2, (uint8_t *)buf);
    OLED_ShowString(0, 6, (uint8_t *)" K1:+  K2:-  K3:Back   ");
}

static uint8_t ShowSafetyOnce(void)
{
    char buf[48];
    float v, r;
    uint16_t lux;
    struct DHT11_Complete_Data_TypeDef DHT11_Data;

    OLED_Clear();
    if (MQ7_ReadDO()) OLED_ShowString(0, 0, (uint8_t *)"CO: Danger");
    else OLED_ShowString(0, 0, (uint8_t *)"CO: Normal");

    uint8_t door_state = HallSensor_ReadStableStatus();
    OLED_ShowString(0, 2, (uint8_t *)"Door:");
    OLED_ShowString(40, 2, (uint8_t *)DoorStatusToStr(door_state));

    v = Get_Last_Value(ADC3, ADC_Channel_5, 10);
    r = (3.3 - v) * 10000 / v;
    lux = GetLux(r);
    snprintf(buf, sizeof(buf), "Light:%dLux  ", lux);
    OLED_ShowString(0, 4, (uint8_t *)buf);

    DHT11_Data = Get_DHT11_Data();
    snprintf(buf, sizeof(buf), "T:%dC H:%d%%  ", (int)DHT11_Data.temp, (int)DHT11_Data.humi);
    OLED_ShowString(0, 6, (uint8_t *)buf);

    if (KEY_Scanf(KEY_GROUP, KEY3_PIN)) return 1;
    return 0;
}

// -------------------------------- 主函数 --------------------------------
int main(void)
{
	
    struct DHT11_Complete_Data_TypeDef DHT11_Data;
    bool LED2;
    AppState_t state = STATE_WELCOME;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    SERVO_Config();

    UART1_Config(115200);   // 调试串口
    UART2_Config(115200);   // 语音模块串口
    UART3_Config(115200);   // 备用

    KEY_Config();
    OLED_Config();
    MQ7_Init();
    HallSensor_Init();
    RED_Config();
    ADC3_Config();
		LED_Config();

    ShowWelcome();
    delay_ms(2000);

    state = STATE_MENU_MAIN;
    ShowMainMenu();

    SERVO_TEXT(90);
    SERVO360_Stop();

    // 发送启动成功消息
    printf("System Started! Voice Control Ready!\r\n");

    while (1) {
		
        // ---------------- 处理语音指令 ----------------
        if (voice_cmd != 0)
        {
            printf("Received voice command: 0x%02X\r\n", voice_cmd);
            VoiceControl_HandleCommand(voice_cmd);
            voice_cmd = 0;  // 清除命令标志
        }

        // ---------------- 读取传感器 ----------------
        DHT11_Data = Get_DHT11_Data();
        g_temp = (int)DHT11_Data.temp;

        if (g_temp >= 29) {
            GPIO_ResetBits(GPIOG, LED2_PIN);
            LED2 = 0;
        } else {
            GPIO_SetBits(GPIOG, LED2_PIN);
            LED2 = 1;
        }

        const char *co_status = MQ7_ReadDO() ? "Danger" : "Normal";
        uint8_t door_state = HallSensor_ReadStableStatus();
        const char *door_status = DoorStatusToStr(door_state);

        float v = Get_Last_Value(ADC3, ADC_Channel_5, 10);
        float r = (3.3 - v) * 10000 / v;
        uint16_t lux = GetLux(r);

        uint8_t person_present = RED_ReadStable();
        const char *person_str = person_present ? "Yes" : "No";

        // MQTT发布数据
        MQTT_PublicTopic(DHT11_Data.temp, DHT11_Data.humi, co_status, door_status, lux, person_str);
        delay_ms(300);

        // 串口打印调试信息
        printf("Temp:%.1fC, Humi:%.1f%%, CO:%s, Door:%s, Light:%dLux, Person:%s\r\n",
               DHT11_Data.temp, DHT11_Data.humi, co_status, door_status, lux, person_str);

        // 人体检测开门逻辑
        if (person_present && (g_temp > 26) && !person_motor_running) {
            person_motor_running = 1;
            SERVO360_SetSpeed(60); 
            delay_s(2);            
            SERVO360_Stop();
        }
        if (!person_present) person_motor_running = 0;

        // ---------------- 状态机 ----------------
        switch (state) {
            case STATE_MENU_MAIN:
                if (KEY_Scanf(KEY_GROUP, KEY1_PIN)) { state = STATE_SAFETY_MONITOR; ShowSafetyOnce(); }
                else if (KEY_Scanf(KEY_GROUP, KEY2_PIN)) { state = STATE_SEAT_MENU; ShowSeatMenu(); }
                break;

            case STATE_SAFETY_MONITOR:
                if (ShowSafetyOnce()) { state = STATE_MENU_MAIN; ShowMainMenu(); }
                if (MQ7_ReadDO() && !co_motor_running) {
                    co_motor_running = 1;
                    SERVO360_SetSpeed(60);
                    delay_s(5);
                    SERVO360_Stop();
                    co_motor_running = 0;
                }
                delay_ms(200);
                break;

            case STATE_SEAT_MENU:
                if (KEY_Scanf(KEY_GROUP, KEY1_PIN)) { state = STATE_SEAT_FRONTBACK; ShowFrontBack(); }
                else if (KEY_Scanf(KEY_GROUP, KEY2_PIN)) { state = STATE_SEAT_TILT; ShowTilt(); }
                else if (KEY_Scanf(KEY_GROUP, KEY3_PIN)) { state = STATE_MENU_MAIN; ShowMainMenu(); }
                break;

            case STATE_SEAT_FRONTBACK:
                if (KEY_Scanf(KEY_GROUP, KEY1_PIN) && seat_position < 100) seat_position++;
                else if (KEY_Scanf(KEY_GROUP, KEY2_PIN) && seat_position > 0) seat_position--;
                else if (KEY_Scanf(KEY_GROUP, KEY3_PIN)) { state = STATE_SEAT_MENU; ShowSeatMenu(); }
                ShowFrontBack();
                delay_ms(50);
                break;

            case STATE_SEAT_TILT:
                if (KEY_Scanf(KEY_GROUP, KEY1_PIN)) { tilt_index++; if (tilt_index >= (int)(sizeof(tilt_sequence)/sizeof(tilt_sequence[0]))) tilt_index = 0; SERVO_TEXT(tilt_sequence[tilt_index]); seat_tilt = tilt_sequence[tilt_index]; ShowTilt(); delay_ms(400); }
                else if (KEY_Scanf(KEY_GROUP, KEY2_PIN)) { tilt_index--; if (tilt_index < 0) tilt_index = (int)(sizeof(tilt_sequence)/sizeof(tilt_sequence[0])) - 1; SERVO_TEXT(tilt_sequence[tilt_index]); seat_tilt = tilt_sequence[tilt_index]; ShowTilt(); delay_ms(400); }
                else if (KEY_Scanf(KEY_GROUP, KEY3_PIN)) { state = STATE_SEAT_MENU; ShowSeatMenu(); }
                break;

            default: state = STATE_MENU_MAIN; ShowMainMenu(); break;
        }
    }
}