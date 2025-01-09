//
// Created by 34753 on 2024/10/10.
//

#ifndef FURINA_ADC_H
#define FURINA_ADC_H

#include "JYZQ_Conf.h"
#include "stm32f4xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

void adc1_init(uint32_t Channel);

void adc1_temperature_sensor_init();// 内部温度传感器初始化

void adc1_start_it();

void adc1_stop_it();

void config_ADC1_Channel(uint32_t channel);// 配置ADC通道

uint16_t get_ADC1_value(uint32_t channel);// 获取ADC值

uint16_t get_ADC1_average(uint32_t channel, uint8_t times);// 获取ADC平均值

float get_adc1_temperature();// 获取温度

#ifdef __cplusplus
}
#endif

#endif //FURINA_ADC_H
