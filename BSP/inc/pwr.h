//
// Created by fairy on 2025/1/9 23:06.
//
#ifndef FURINA_PWR_H
#define FURINA_PWR_H
#include <bsp_config.h>
#ifdef __cplusplus
extern "C" {
#endif

// *******并未验证过******** //
void pvd_init();// 初始化PVD，用于电源检测

void PowerModeInit();// 初始化PWR，用于电源管理，比如睡眠模式等

void EnterSleepMode();// 进入睡眠模式

void EnterStopMode();// 进入停止模式

void EnterStandbyMode();// 进入待机模式


#ifdef __cplusplus
}
#endif
#endif //FURINA_PWR_H