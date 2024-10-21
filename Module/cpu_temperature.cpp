//
// Created by DV on 2024/10/13.
//

#include "cpu_temperature.hpp"
#include "adc.h"

//#if USE_CPU_TEMPERATURE

/**
 * @brief 测量CPU温度
 * @details 返回值:温度值(扩大了100倍,单位:℃.)
 */
auto thermometer::Get_Temperature() -> short {
    uint32_t adcx = Get_Adc_Average(&hadc1, ADC_CHANNEL_16, 10);
    double temperature = (double)adcx * (3.3 / 4096);
    temperature = (temperature - 0.76) / 0.0025 + 25;
    auto result = (short)(temperature * 100);
    return result;
}
//#endif