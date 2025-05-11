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
import hw_registers;
import ads1115;

using AsyncDelay_HAL = utils::async_delay<HAL_GetTick>;
AsyncDelay_HAL async_delay(500);
namespace FPGA
{
    constexpr uint32_t FREQ_10 = 429;
    constexpr uint32_t FREQ_100 = 4295;
    constexpr uint32_t FREQ_1K = 42950;
    constexpr uint32_t FREQ_10K = 429497;
    constexpr uint32_t FREQ_100K = 4294967;
    constexpr uint32_t FREQ_1M = 42949673;
}
using FREQ_WORD = Register<0x60000000>;

static volatile uint16_t adc_value = 0;
static volatile float adc_mv = 0;
namespace app
{
    void Control::init()
    {
        using namespace bsp;
        adc::init_temperature_sensor();
        rng::init();
        usart::init();
        adc::ADS1115::init();

        GPIO_InitTypeDef GPIO_InitStruct = {};
        GPIO_InitStruct.Pin = GPIO_PIN_5;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        // 第一阶段：复位信号激活（低电平有效时省略此步）
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET); // 释放复位
        HAL_Delay(10); // 保证稳定

        // 正式复位脉冲
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET); // 拉低复位
        HAL_Delay(50); // 维持复位状态时间（典型值20-100ms）

        // 结束复位
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET); // 释放复位
    }

    void Control::background_process() {    // 获取ADC值
        if(async_delay.is_timeout())
        {
            adc_value = bsp::adc::ADS1115::read(bsp::adc::ads1115::MuxConfig::Single_1);
            adc_mv = adc_value*2.048/65535;
        }
    }
}




namespace utils
{
    void Key::handler()
    {
        switch (PlatformKey::getCode())
        {
            case keyK0:
                // 1K
                FREQ_WORD::write(FPGA::FREQ_1K);
                break;


            case keyK1:
                // 10K
                FREQ_WORD::write(FPGA::FREQ_10K);
                break;

            case keyK2:
                // 100K
                FREQ_WORD::write(FPGA::FREQ_100K);
                break;

            case keyK3:
                // 1M
                FREQ_WORD::write(FPGA::FREQ_1M);
                break;

            case keyK5:

                break;

            case keyKD:
                break;

            case keyKF:
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

}
