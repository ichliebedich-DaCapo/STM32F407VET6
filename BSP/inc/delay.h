//
// Created by DV on 2025/2/7.
//

#ifndef FURINA_DELAY_H
#define FURINA_DELAY_H
#include "App_Conf.h"
#include "stm32f4xx_hal.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void delay_Init();

// ��ʱ΢�뺯��
void delay_us(uint16_t us);


#ifdef __cplusplus
}
#endif

#endif //FURINA_DELAY_H