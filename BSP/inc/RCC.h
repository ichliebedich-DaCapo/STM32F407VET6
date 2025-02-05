//
// Created by fairy on 2025/1/11 16:19.
//
#ifndef FURINA_RCC_H
#define FURINA_RCC_H
#include <bsp_config.h>
#ifdef __cplusplus
extern "C" {
#endif
void SystemClock_DefaultConfig();// 默认配置
void SystemClock_Overclock();// 超频
#ifdef __cplusplus
}
#endif
#endif //FURINA_RCC_H