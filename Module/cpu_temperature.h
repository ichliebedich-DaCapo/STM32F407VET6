//
// Created by 吴俊颐 on 2024/10/13.
//

#ifndef FURINA_CPU_TEMPERATURE_H
#define FURINA_CPU_TEMPERATURE_H
#include "JYZQ_Conf.h"
#if USE_CPU_TEMPERATURE

/**
 * @brief 测温计（CPU）
 * @note 实现测量CPU温度的测温计功能
 *
 */
class thermometer
{
public:

    static auto Get_Temprate()->uint8_t ;

};


#endif  // USE_CPU_TEMPERATURE
#endif //FURINA_CPU_TEMPERATURE_H