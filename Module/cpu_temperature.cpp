//
// Created by DV on 2024/10/13.
//

#include "cpu_temperature.h"

#if USE_CPU_TEMPERATURE

/**
 * @brief 测量CPU温度
 * @details 返回值:温度值(扩大了100倍,单位:℃.)
 */
auto thermometer::Get_Temprate() -> uint8_t
{
    uint32_t adcx;
    uint8_t result;
    double temperate;
    adcx=Get_Adc_Average(ADC_Channel_16,10);	//读取通道16内部温度传感器通道,10次取平均
    temperate=(float )adcx*(3.3/4096);		//电压值
    temperate=(temperate-0.76)/0.0025 + 25; //转换为温度值
    result=(uint8_t)(temperate*=100);					//扩大100倍.
    return result;
}


#endif