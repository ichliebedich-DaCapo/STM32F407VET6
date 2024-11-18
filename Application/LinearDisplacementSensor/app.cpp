//
// Created by DV on 2024/11/17.
//
//PA0 -->ADC1
//PA9 -->TXD
//GND -->GND
#include <cstdio>
#include "app.hpp"

#include "timer.h"
#include "dac.h"
#include "adc.h"
#include "key.hpp"
#include "usart.h"


void app_init()
{
   adc1_init();
   timer2_init(FREQ_84M_to_100);
   usart1_init();
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

//            } else
//            {
//                   adc1_stop_it();
//
//            }

            break;
        case keyk1://开始采集数据
               if (Key::stateHandler(KEY_STATE_NONE))
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
    // 获取ADC值
    uint16_t adcValue = HAL_ADC_GetValue(&hadc1);

   // 打印ADC值
    printf("%f\r\n",adcValue/4095.0f*3.3*30/3.3);//真实电压*电压密度=长度

}