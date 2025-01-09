//
// Created by fairy on 2025/1/9 23:06.
//
#include "pwr.h"
#include "JYZQ_Conf.h"
// 头文件
#include "stm32f4xx_hal.h"

// 函数
void pvd_init()
{
    // 初始化 PVD
    __HAL_RCC_PWR_CLK_ENABLE();

    PWR_PVDTypeDef sConfigPVD;
    sConfigPVD.PVDLevel = PWR_PVDLEVEL_7;// 对应大概2.9V
    sConfigPVD.Mode = PWR_PVD_MODE_IT_RISING_FALLING;
    HAL_PWR_ConfigPVD(&sConfigPVD);
    HAL_PWR_EnablePVD();
    HAL_NVIC_SetPriority(PVD_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(PVD_IRQn);
}


void PVD_IRQHandler(void)
{
    HAL_PWR_PVD_IRQHandler(); // 调用 HAL 库的中断处理函数
}

