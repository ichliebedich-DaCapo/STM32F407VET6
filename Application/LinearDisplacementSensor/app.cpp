//
// Created by DV on 2024/11/17.
//

#include "app.hpp"

#include "timer.h"
#include "dac.h"
#include "adc.h"
#include "key.hpp"


void app_init()
{
   adc1_init();
   timer2_init(FREQ_84M_to_100);
}

void key_handler()
{
  switch (Key::getCode())
    {
        case keyk0://开始采集数据
               if (Key::stateHandler(KEY_STATE_TWO))
            {
                   adc1_start_it();

            } else
            {
                   adc1_stop_it();

            }
            break;

        default:
            break;
    }
}
/**实现中断服务例程*/
// 用于采集ADC数据
void adc1_isr()
{
    HAL_ADC_GetValue(&hadc1);
}