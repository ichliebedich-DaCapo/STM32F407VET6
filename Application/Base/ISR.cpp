//
// Created by fairy on 2024/10/10.
//
/**
* @brief 中断服务例程
 * @note 用于处理各种中断回调函数，之所以不用stm32f4xx_it.c,是因为本工程是由C/C++混编，
 *      中断服务例程可能会调用到C++文件相关的代码。
*/
#include "Module_Conf.h"
#include "stm32f4xx_hal.h"
#include "timer.h"
#include "adc.h"

// 由用户自行实现的中断处理函数
extern void timer6_isr();

extern void adc1_isr();

// 弱定义，避免找不到
__weak void timer6_isr() {} // 万一忘记定义了也没报错那可就糟糕了
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
void TIM6_DAC_IRQHandler()
{

    // 我把TIM7当做系统时钟，所以并不需要判断中断源
    __HAL_TIM_CLEAR_FLAG(&htim6, TIM_FLAG_UPDATE);
    timer6_isr();
}

}

/*ADC中断回调函数*/
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if (hadc->Instance == ADC1)
    {
        adc1_isr();
    }
}
