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

void PowerModeInit()
{
    // Enable PWR clock
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
}
/**
 * @brief 进入低功耗模式
 * @details 任何中断都可以唤醒。
 * @note  CPU 停止运行，但外设和时钟仍然工作。
 */
void EnterSleepMode()
{
    __WFI();  // Wait for interrupt
}


/**
 * @brief 进入停止模式
 * @details 外部中断或 RTC 闹钟可以唤醒。
 * @note  所有时钟停止，但 SRAM 和寄存器内容保留。
 */
void EnterStopMode()
{
    // Set PDDS (Power Down Deep Sleep) bit in PWR_CR
    PWR->CR |= PWR_CR_PDDS;

    // Clear WUF (Wake-Up Flag) bit in PWR_CR
    PWR->CR |= PWR_CR_CWUF;

    // Enter stop mode
    __WFI();
}

/**
 * @brief 进入低功耗模式
 * @details 外部复位、RTC 闹钟或 WKUP 引脚可以唤醒。
 * @note  除了备份寄存器和待机电路，所有其他电路都关闭。
 */
void EnterStandbyMode()
{
    // Set PDDS (Power Down Deep Sleep) bit in PWR_CR
    PWR->CR |= PWR_CR_PDDS;

    // Set WUF (Wake-Up Flag) bit in PWR_CSR
    PWR->CR |= PWR_CR_CWUF;

    // Enter standby mode
    __WFI();
}

