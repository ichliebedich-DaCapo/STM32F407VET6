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
        case keyk0://开始采集数据
            if (Key::stateHandler(KEY_STATE_NONE))
            {
                LCD_direction(0);
                LCD_Clear(0x36ff);
            }
            break;


        case keyk1://开始采集数据
            if (Key::stateHandler(KEY_STATE_NONE))
            {
                LCD_direction(1);
                LCD_Clear(0xffff);
            }
            break;


        case keyk2://开始采集数据

            LCD_direction(1);
            LCD_Clear(0xff36);

            break;

        case keyk3:
            LCD_direction(1);
            LCD_Clear(0x4242);
            break;

        case keyk4:
            LCD_Clear(0x304f);
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




