//
// Created by fairy on 2025/1/9 13:31.
//
#include <project_config.h>
import app;
#ifdef GUI_ENABLE

#include "GUI.hpp"
#include "ui.hpp"

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

#define TEST_FPGA_REG (*((volatile unsigned short *)0x60020000))

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
        delay::init();
        usart::init();


    }

    void Control::background_process()
    {
        //    if (async_delay.is_timeout())
        //    {
        //        printf("%f\r\n", get_adc1_temperature());
        //    }
        //    等待上一次的数据发送完毕
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
                for (uint32_t i = 0; i < 100000; i++)
                {
                    // write_reg = Get_Random_Number() & 0xFFFF;
                    TEST_FPGA_REG = write_reg;
                    read_reg = TEST_FPGA_REG;
                    if (write_reg != read_reg)
                    {
                        arr_error_fpga[error_fpga_count++] = i;
                    }
                }
                error_fpga_rate = error_fpga_count / 100000.0f;
                error_fpga_count = 0;
                __BKPT(2);
                break;


            case keyK1:
                // 测试访问速度
                pre_tick = HAL_GetTick();
                for (uint32_t i = 0; i < 1000000; ++i)
                {
                    read_reg = TEST_FPGA_REG;
                }
                current_tick = HAL_GetTick();
                current_tick = current_tick - pre_tick; // 单位为ms
                __BKPT(0);
                break;

            case keyK2:

                break;

            case keyK5:
                // 测试写入速度
                // 1798ms -> 1.798us一次
                pre_tick = HAL_GetTick();
                for (uint32_t i = 0; i < 1000000; ++i)
                {
                    TEST_FPGA_REG = write_reg;
                }
                current_tick = HAL_GetTick();
                current_tick = current_tick - pre_tick; // 单位为ms
                __BKPT(1);
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

float temp;
