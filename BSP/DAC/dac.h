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

#define dac_start() HAL_DAC_Start(&hdac, DAC_CHANNEL_1)
#define dac_stop() HAL_DAC_Stop(&hdac, DAC_CHANNEL_1)


void dac_init();




#ifdef __cplusplus
}
#endif
#endif
#endif //FURINA_DAC_H
