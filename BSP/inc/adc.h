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

static inline void adc1_start_it()
{
    timer2_start();
    HAL_ADC_Start_IT(&hadc1);
}

static inline void adc1_stop_it()
{
    timer2_stop();
    HAL_ADC_Stop_IT(&hadc1);
}
/**
 * @brief 配置ADC通道
 * @param hadc:ADC句柄
 * @param channel:通道值
 */
static inline void ADC_ConfigChannel(ADC_HandleTypeDef *hadc, uint32_t channel) {
    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = channel;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
    if (HAL_ADC_ConfigChannel(hadc, &sConfig) != HAL_OK) {
        Error_Handler();
    }
}
/**
 * @brief 获取ADC值
 * @param hadc
 * @param channel
 * @return
 */
static inline uint16_t Get_Adc(ADC_HandleTypeDef *hadc, uint32_t channel) {
    ADC_ConfigChannel(hadc, channel);
    HAL_ADC_Start(hadc);
    HAL_ADC_PollForConversion(hadc, HAL_MAX_DELAY);
    return HAL_ADC_GetValue(hadc);
}

/**
 * @brief 获取ADC平均值
 * @param hadc
 * @param channel
 * @param times
 * @return
 */
static inline uint16_t Get_Adc_Average(ADC_HandleTypeDef *hadc, uint32_t channel, uint8_t times) {
    uint32_t temp_val = 0;
    for (uint8_t t = 0; t < times; t++) {
        temp_val += Get_Adc(hadc, channel);
        HAL_Delay(5); // 延时5ms
    }
    return temp_val / times;
}

#ifdef __cplusplus
}
#endif
#endif
#endif //FURINA_ADC_H
