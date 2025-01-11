//
// Created by fairy on 2025/1/11 16:19.
//
#include "RCC.h"
#include "JYZQ_Conf.h"
// 头文件
#include "stm32f4xx_hal.h"

void SystemClock_SetFrequency(uint32_t plln);

// 函数

/**
 * @brief 默认系统时钟初始化
 * @details 最高稳定频率为168MHz
 */
void SystemClock_DefaultConfig()
{
    SystemClock_SetFrequency(168);

}

void SystemClock_SwitchToHSI(void)
{
    // 将系统时钟切换到 HSI
    __HAL_RCC_SYSCLK_CONFIG(RCC_SYSCLKSOURCE_HSI);

    // 等待时钟切换完成
    while (__HAL_RCC_GET_SYSCLK_SOURCE() != RCC_SYSCLKSOURCE_STATUS_HSI) {}
}

void SystemClock_SwitchToPLL(void)
{
    // 将系统时钟切换到 PLL
    __HAL_RCC_SYSCLK_CONFIG(RCC_SYSCLKSOURCE_PLLCLK);

    // 等待时钟切换完成
    while (__HAL_RCC_GET_SYSCLK_SOURCE() != RCC_SYSCLKSOURCE_STATUS_PLLCLK) {}
}

/**
  * @brief  超频
  * @param  无
  * @retval 无
  * @details 通过超频可以提升至 200MHz~216MHz，这里默认使用216MHz
  */

void SystemClock_Overclock()
{
    SystemClock_SetFrequency(216);
}

/**
 * @brief 设置系统时钟频率
 * @param plln 锁相环倍频
 * @details 基于外部晶振为12MHz的配置
 */
void SystemClock_SetFrequency(uint32_t plln)
{
    // 禁用全局中断
    __disable_irq();

    // 切换到 HSI
    SystemClock_SwitchToHSI();

    // 禁用 PLL
    __HAL_RCC_PLL_DISABLE();

    // 配置 PLL
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 6;       // HSE 分频系数
    RCC_OscInitStruct.PLL.PLLN = plln;    // PLL 倍频系数
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2; // PLLP 分频系数
    RCC_OscInitStruct.PLL.PLLQ = 4;       // PLLQ 分频系数

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {

        Error_Handler();
    }

    // 配置系统时钟
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                  | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    // 设置 Flash 等待周期
    uint32_t flash_latency = (plln <= 168) ? FLASH_LATENCY_5 : FLASH_LATENCY_7;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, flash_latency) != HAL_OK)
    {
        Error_Handler();
    }

    // 切换到 PLL
    SystemClock_SwitchToPLL();

    // 重新启用全局中断
    __enable_irq();
}