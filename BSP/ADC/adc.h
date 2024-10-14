//
// Created by 34753 on 2024/10/10.
//

#ifndef FURINA_ADC_H
#define FURINA_ADC_H

#include "JYZQ_Conf.h"
#include "stm32f4xx_hal.h"
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


#ifdef __cplusplus
}
#endif
#endif
#endif //FURINA_ADC_H
