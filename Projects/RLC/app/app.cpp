//
// Created by fairy on 2025/1/9 13:31.
//
#include <etl/array.h>
#include <app.hpp>
#ifdef GUI_ENABLE

import gui;

#endif
#ifdef FREERTOS_ENABLE
#include "cmsis_os2.h"
#endif
// 头文件


import key_adapter;

import async_delay;
import delay;
import adc;
import rng;
import usart;
import key;

using AsyncDelay_HAL = utils::async_delay<HAL_GetTick>;
AsyncDelay_HAL async_delay(500);



namespace
{
    volatile uint16_t read_reg;
    volatile uint16_t write_reg;
    volatile uint32_t pre_tick;
    volatile uint32_t current_tick;
    uint8_t arr_error_fpga[1000];
    uint16_t error_fpga_count = 0;
    float error_fpga_rate = 0;
}




namespace app
{
    void Control::init()
    {
        using namespace bsp;
        adc::init_temperature_sensor();
        rng::init();
        usart::init();

    }

    void Control::background_process()
    {

    }
}



namespace utils
{
    void Key::handler()
    {
        switch (PlatformKey::getCode())
        {
            case keyK0:
                // 测试错误率

                break;


            case keyK1:
                break;

            case keyK2:

                break;

            case keyK5:
                break;

            default:
                break;
        }
    }
}



/**实现中断服务例程*/
// 用于采集ADC数据
void adc1_isr()
{
    // 获取ADC值
}

