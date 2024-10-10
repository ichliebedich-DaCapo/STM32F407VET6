//
// Created by 34753 on 2024/10/10.
//

#include "timer.h"
#if USE_TIMER

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim6;

/**
 * @brief TIM2初始化（84MHz）
 * @param arr 周期值
 * @param psc 预分频
 * @note    作为触发器使用，下面配置是配置是为了ADC采集
 */
void timer2_init(uint32_t arr, uint32_t psc)
{
    __HAL_RCC_TIM2_CLK_ENABLE();

    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    htim2.Instance = TIM2;
    htim2.Init.Prescaler = psc;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = arr;
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE; // 自动预加载，关闭。
    // 更新事件不会立即将ARR的新值应用到影子寄存器中，而是等到下一个更新事件才会生效。这种设置通常用于不需要立即更新计数器重载值的情况
    HAL_TIM_Base_Init(&htim2);

    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig);

    sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig);
}

/**
 * @brief TIM6初始化（84MHz）
 * @param arr
 * @param psc
 * @note    一般作为DAC的中断使用
 */
void timer6_init(uint32_t arr, uint32_t psc)
{
    __HAL_RCC_TIM6_CLK_ENABLE();

    TIM_MasterConfigTypeDef sMasterConfig = {0};

    htim6.Instance = TIM6;
    htim6.Init.Prescaler = psc;
    htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim6.Init.Period = arr;
    htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;// 自动预加载，开启。
    HAL_TIM_Base_Init(&htim6);

    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_ENABLE;
    HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig);

    // 由于默认使用的是优先级组4，所以没有子优先级
    HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
}

#endif