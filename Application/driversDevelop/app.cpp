//
// Created by fairy on 2025/1/9 13:31.
//
#include "app.hpp"

// 头文件
#include "adc.h"
#include "key.hpp"

// 函数
void app_init()
{
    adc1_init();
    timer2_init(FREQ_84M_to_200);

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
    uint16_t adcValue = HAL_ADC_GetValue(&hadc1);

}