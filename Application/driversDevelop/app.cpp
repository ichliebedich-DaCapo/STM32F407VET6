//
// Created by fairy on 2025/1/9 13:31.
//
#include "app.hpp"
#include "cmsis_os2.h"
// 头文件
#include "adc.h"
#include "key.hpp"
#include "timer.h"
#include "RNG.h"

import async_delay;

using AsyncDelay_HAL = AsyncDelay<HAL_GetTick>;
AsyncDelay_HAL temperature_delay(1000);

// 函数
void app_init()
{
    adc1_temperature_sensor_init();
    RNG_Init();

}

void key_handler()
{
    switch (Key::getCode())
    {
        case keyk0://开始采集数据
            if (Key::stateHandler(KEY_STATE_NONE))
            {
                adc1_start_it();
            }


            break;
        case keyk1://开始采集数据
            if (Key::stateHandler(KEY_STATE_NONE))
            {
                adc1_stop_it();

            }

            break;


        case keyk2://开始采集数据


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

    osDelay(1000);
    temp = get_adc1_temperature();


}


