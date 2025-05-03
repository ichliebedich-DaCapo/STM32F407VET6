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


#define FREQ_WORD (*((volatile unsigned int *)(0x60000000)))

namespace {}


namespace app
{
    void Control::init()
    {
        using namespace bsp;
        adc::init_temperature_sensor();
        rng::init();
        usart::init();

        // 第一阶段：复位信号激活（低电平有效时省略此步）
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET); // 释放复位
        HAL_Delay(10); // 保证稳定

        // 正式复位脉冲
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET); // 拉低复位
        HAL_Delay(50); // 维持复位状态时间（典型值20-100ms）

        // 结束复位
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET); // 释放复位
    }

    void Control::background_process() {}
}


static volatile uint32_t temp;

namespace utils
{
    void Key::handler()
    {
        switch (PlatformKey::getCode())
        {
            case keyK0:
                // 1K
                FREQ_WORD = 171799;
                break;


            case keyK1:
                // 10K
                FREQ_WORD = 1717987;
                break;

            case keyK2:
                // 100K
                FREQ_WORD = 17179869;
                break;

            case keyK3:
                // 1M
                FREQ_WORD = 171798692;
                break;

            case keyK5:
                break;

            case keyK8:
                // 10
                FREQ_WORD = 1718;
                break;

            case keyK7:
                // 100
                FREQ_WORD = 17180;
                break;

            case keyKF:
                temp = FREQ_WORD;
                __BKPT(0);
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
