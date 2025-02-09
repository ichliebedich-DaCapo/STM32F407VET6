//
// Created by DV on 2024/11/17.
//
//PA0 -->ADC1
//PA9 -->TXD
//GND -->GND
#include <cstdio>

#include "timer.h"
#include "dac.h"
#include "adc.h"
#include "key_adapter.hpp"
#include "usart.h"

extern UART_HandleTypeDef huart1;
uint8_t rxBuffer[1];
void app_init()
{
   adc1_init(ADC_CHANNEL_0);
   timer2_init(FREQ_84M_to_200);
   usart1_init();
   HAL_UART_Receive_IT(&huart1, rxBuffer, 1);
}

void key_handler()
{
    switch (PlatformKey::getCode())
    {
        case keyK0://开始采集数据
            if (PlatformKey::handle_state(KEY_STATE_NONE))
            {
                adc1_start_it();
            }

//            } else
//            {
//                   adc1_stop_it();
//
//            }

            break;
        case keyK1://开始采集数据
            if (PlatformKey::handle_state(KEY_STATE_NONE))
            {
                adc1_stop_it();

            }

            break;


        case keyK2://开始采集数据


            break;

        default:
            break;

    }
}
/**实现中断服务例程*/
// 用于采集ADC数据
void adc1_isr() {
    // 获取ADC值
    uint16_t adcValue = get_ADC1_value(ADC_CHANNEL_0);
    // 打印ADC值
    //printf("%f\r\n", (4095-adcValue) / 4095.0f * 3.3 * 5 / 3.3);//方案1：传输给单片机处理 真实电压*电压密度=长度
//    printf("%d\r\n", adcValue);//方案2：传输给上位机处理
    printf("%f\r\n", adcValue / 4095.0f * 3.3);
}

/*实现串口的接收完成中断回调函数*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        // 启动接收中断
        HAL_UART_Transmit(&huart1, rxBuffer, 1, 100);
        //重新开始
        HAL_UART_Receive_IT(&huart1, rxBuffer, 1);
    }
}