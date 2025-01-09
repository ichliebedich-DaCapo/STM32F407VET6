//
// Created by 34753 on 2024/10/10.
//

#ifndef FURINA_ADC_H
#define FURINA_ADC_H

#include "JYZQ_Conf.h"
#include "stm32f4xx_hal.h"
#include "timer.h"

#ifdef USE_ADC
#ifdef __cplusplus
extern "C" {
#endif

extern ADC_HandleTypeDef hadc1;

void adc1_init();

void adc1_start_it();

void adc1_stop_it();

void ADC_ConfigChannel(ADC_HandleTypeDef *hadc, uint32_t channel);// 配置ADC通道

uint16_t Get_Adc(ADC_HandleTypeDef *hadc, uint32_t channel);// 获取ADC值

uint16_t Get_Adc_Average(ADC_HandleTypeDef *hadc, uint32_t channel, uint8_t times);// 获取ADC平均值

#ifdef __cplusplus
}
#endif
#endif
#endif //FURINA_ADC_H
