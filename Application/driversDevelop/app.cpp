//
// Created by fairy on 2025/1/9 13:31.
//
#include "app.hpp"

#ifdef FreeRTOS_ENABLE
#include "cmsis_os2.h"
#endif
// 头文件
#include "adc.h"
#include "key.hpp"
#include "timer.h"
#include "RNG.h"
#include "lcd.h"
#ifndef GUI_DISABLE
#include "GUI.hpp"
#endif

#include "RCC.h"
#include "debug.h"

import async_delay;
#include <cstdio>

using AsyncDelay_HAL = AsyncDelay<HAL_GetTick>;
AsyncDelay_HAL async_delay(500);

// 函数



void app_init()
{
    adc1_temperature_sensor_init();
    RNG_Init();
    ITM_Init();

}

void key_handler()
{
    switch (Key::getCode())
    {

        case keyk0:
            if (Key::stateHandler(KEY_STATE_NONE))
            {
                LCD_Clear(0x36ff);
            }
            break;


        case keyk1:
            if (Key::stateHandler(KEY_STATE_NONE))
            {
                LCD_Clear(0xffff);
            }
            break;
        case keyk2:
            LCD_Clear(0xFF36); // 填充颜色 0xFF36
            break;

        case keyk3:
            LCD_Clear(0x4242); // 填充颜色 0x4242
            break;

        case keyk4:
            LCD_Clear(0x1234); // 填充颜色 0x1234
            break;

        case keyk5:
            LCD_Clear(0x5678); // 填充颜色 0x5678
            break;

        case keyk6:
            LCD_Clear(0x9ABC); // 填充颜色 0x9ABC
            break;

        case keyk7:

            LCD_Clear(0xDEF0); // 填充颜色 0xDEF0
            break;

        case keyk8:
            LCD_Clear(0x304F); // 填充颜色 0x304F
            break;

        case keyk9:
            LCD_Clear(0x1A2B); // 填充颜色 0x1A2B
            break;

        case keykA:
            LCD_Clear(0x3C4D); // 填充颜色 0x3C4D
            break;

        case keykB:
            LCD_Clear(0x5E6F); // 填充颜色 0x5E6F
            break;

        case keykC:
            LCD_Clear(0x7A8B); // 填充颜色 0x7A8B
            break;

        case keykD:
            LCD_Clear(0x9CAD); // 填充颜色 0x9CAD
            break;

        case keykE:
            LCD_Clear(0xBCDE); // 填充颜色 0xBCDE
            break;

        case keykF:
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




