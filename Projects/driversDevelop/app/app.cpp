//
// Created by fairy on 2025/1/9 13:31.
//
#include <project_config.h>
#ifdef GUI_ENABLE
#include "GUI.hpp"
#endif
#ifdef FREERTOS_ENABLE
#include "cmsis_os2.h"
#endif
// 头文件
#include "adc.h"
#include "key.hpp"
#include "timer.h"
#include "RNG.h"
#include "lcd.h"
#include "key_adapter.hpp"

#include "RCC.h"
#include "debug.h"

import async_delay;
#include <cstdio>

using AsyncDelay_HAL = AsyncDelay<HAL_GetTick>;
AsyncDelay_HAL async_delay(500);

// 函数

// 数组
const uint16_t color[120 * 120]={};
void app_init()
{
    adc1_temperature_sensor_init();
    RNG_Init();
    ITM_Init();

}

void key_handler()
{
    switch (PlatformKey::getCode())
    {

        case keyK0:
            if (PlatformKey ::handle_state(KEY_STATE_NONE))
            {
//                LCD_Clear(0x36ff);
                for(uint16_t i=10;i<200;i++)LCD_Set_Pixel(i,20,0x0000);
//                for(uint16_t i=20;i<200;i++)LCD_Set_Pixel(i,100,0x0000);
//                for(uint16_t i=30;i<100;i++)LCD_Set_Pixel(i,200,0x0000);
//                for(uint16_t i=40;i<50;i++)LCD_Set_Pixel(i,250,0x0000);
            }
            break;


        case keyK1:
            if (PlatformKey ::handle_state(KEY_STATE_NONE))
            {
                LCD_Clear(0xffff);
            }
            break;
        case keyK2:
            LCD_Clear(0xFF36); // 填充颜色 0xFF36
            lcd_flush(20,20,100,100,color);

            break;

        case keyK3:
            LCD_Clear(0x4242); // 填充颜色 0x4242
            break;

        case keyK4:
            LCD_Clear(0x1234); // 填充颜色 0x1234
            break;

        case keyK5:
            LCD_Clear(0x5678); // 填充颜色 0x5678
            break;

        case keyK6:
            LCD_Clear(0x9ABC); // 填充颜色 0x9ABC
            break;

        case keyK7:

            LCD_Clear(0xDEF0); // 填充颜色 0xDEF0
            break;

        case keyK8:
            LCD_Clear(0x304F); // 填充颜色 0x304F
            break;

        case keyK9:
            LCD_Clear(0x1A2B); // 填充颜色 0x1A2B
            break;

        case keyKA:
            LCD_Clear(0x3C4D); // 填充颜色 0x3C4D
            break;

        case keyKB:
            LCD_Clear(0x5E6F); // 填充颜色 0x5E6F
            break;

        case keyKC:
            LCD_Clear(0x7A8B); // 填充颜色 0x7A8B
            break;

        case keyKD:
            LCD_Clear(0x9CAD); // 填充颜色 0x9CAD
            break;

        case keyKE:
            LCD_Clear(0xBCDE); // 填充颜色 0xBCDE
            break;

        case keyKF:
            LCD_Clear(0xFEDC); // 填充颜色 0xFEDC
            break;

        default:
            break;

}
}
/**实现中断服务例程*/
// 用于采集ADC数据
void adc1_isr()
{
    // 获取ADC值


}

float temp;
uint32_t rand;

void background_handler()
{
    if (async_delay.is_timeout())
    {
        printf("%f\r\n", get_adc1_temperature());

    }
//    CPU::print<get_adc1_temperature>();
}




