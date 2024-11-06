//
// Created by fairy on 2024/10/10.
//
/**
* @brief 中断服务例程
 * @note 用于处理各种中断回调函数，之所以不用stm32f4xx_it.c,是因为本工程是由C/C++混编，
 *      中断服务例程可能会调用到C++文件相关的代码。
*/
#include "JYZQ_Conf.h"
#include "stm32f4xx_hal.h"
#include "timer.h"
#include "adc.h"

// 由用户自行实现的中断处理函数
extern void timer6_isr();

extern void adc1_isr();

// 弱定义，避免找不到

__weak void adc1_isr() {}

/*TIM中断回调函数*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM6)
    {

    }
}

extern "C"
{


}

/*ADC中断回调函数*/
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if (hadc->Instance == ADC1)
    {
        adc1_isr();
    }
}
