//
// Created by DV on 2025/2/7.
//

#ifndef FURINA_DELAY_H
#define FURINA_DELAY_H
#include <bsp_config.h>

#ifdef __cplusplus
extern "C" {
#endif

void delay_Init();

// 延时微秒函数
void delay_us(uint16_t us);

uint32_t get_delay_tick();

#ifdef __cplusplus
}
#endif

#endif //FURINA_DELAY_H