//
// Created by 34753 on 2024/10/10.
//

#ifndef FURINA_DAC_H
#define FURINA_DAC_H

#include "Module_Conf.h"
#include "stm32f4xx_hal.h"

#ifdef USE_DAC
#ifdef __cplusplus
extern "C" {
#endif
extern DAC_HandleTypeDef hdac;

// DAC初始化
void dac_init();

// DAC启动
static inline void dac_start()
{
    HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
}

// DAC停止
static inline void dac_stop()
{
    HAL_DAC_Stop(&hdac, DAC_CHANNEL_1);
}

/**
 * @brief 设置DAC输出值
 * @param value 12位数据值 范围：0~4095
 * @note 默认12位右对齐
 */
static inline void dac_set_value(uint16_t value)
{
    HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, value);
}

#ifdef __cplusplus
}
#endif
#endif
#endif //FURINA_DAC_H
