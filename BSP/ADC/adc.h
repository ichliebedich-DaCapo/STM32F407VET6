//
// Created by 34753 on 2024/10/10.
//

#ifndef FURINA_ADC_H
#define FURINA_ADC_H

#include "JYZQ_Conf.h"

#if USE_ADC
#ifdef __cplusplus
extern "C" {
#endif

extern ADC_HandleTypeDef hadc1;

void adc1_init();

static inline void adc1_start_it()
{
    HAL_ADC_Start_IT(&hadc1);
}

static inline void adc1_stop_it()
{
    HAL_ADC_Stop_IT(&hadc1);
}


/**
 * @brief 获得ADC值
 * @param ch:通道值
 * @ref ADC_channels  0~16：ADC_Channel_0~ADC_Channel_16
 * @return 转换结果
 */
static inline uint16_t Get_Adc(uint8_t ch)
{
    // 配置ADC规则通道
    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = ch;
    sConfig.Rank = 1; // 规则组通道1
    sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES; // 480个周期
    HAL_StatusTypeDef status = HAL_ADC_ConfigChannel(&hadc1, &sConfig);
    // 启动ADC转换
    status = HAL_ADC_Start(&hadc1);
    // 等待转换结束
    status = HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
    if (status == HAL_OK) {
        // 读取转换结果
        return HAL_ADC_GetValue(&hadc1);
    }
}
/**
 * @brief 获取通道ch的转换值，取times次,然后平均
 * @param ch:通道编号 times:获取次数
 * @return 通道ch的times次转换结果平均值
 */
static inline uint16_t Get_Adc_Average(uint8_t ch,uint8_t times) {
    uint32_t temp_val = 0;
    uint8_t t;

    for (t = 0; t < times; t++) {
        temp_val += Get_Adc(ch); // 获取通道转换值
        HAL_Delay(5); // 延时5毫秒
    }

    return (uint16_t) (temp_val / times);
}

#ifdef __cplusplus
}
#endif
#endif
#endif //FURINA_ADC_H
