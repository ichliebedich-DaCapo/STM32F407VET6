//
// Created by 34753 on 2024/10/12.
//

#ifndef FURINA_SIGNALGENERATOR_HPP
#define FURINA_SIGNALGENERATOR_HPP
#include "JYZQ_Conf.h"
#if ENABLE_SIGNAL_GENERATOR
// 宏定义
#define APP_NO_RTOS // 没有使用FreeRTOS

/*************************模块级**************************/
#define USE_WAVE_SIGNAL // 使用波形信号

/**************************板级**************************/
#define USE_TIMER // 使用定时器
#define USE_ADC  // 使用ADC
#define USE_DAC // 使用DAC

#endif
#endif //FURINA_SIGNALGENERATOR_HPP
