//
// Created by 34753 on 2024/10/10.
//

#include "timer.h"


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

    // 不能关
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_ENABLE;
    HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig);

    // 由于默认使用的是优先级组4，所以没有子优先级
    HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);

    __HAL_TIM_CLEAR_IT(&htim6, TIM_IT_UPDATE); // 清除定时器初始化过程中的更新中断标志，避免定时器一启动就中断
}

extern void timer6_isr();
__weak void timer6_isr() {}
void TIM6_DAC_IRQHandler()
{
    // 我把TIM7当做系统时钟，所以并不需要判断中断源
    __HAL_TIM_CLEAR_FLAG(&htim6, TIM_FLAG_UPDATE);
    timer6_isr();
}

/**
 * @brief 设置定时器6的时钟
 * @param psc 预分频
 * @param arr 自动重载值，不能为0
 */
void timer6_set_freq(uint32_t psc,uint32_t arr )
{
    __HAL_TIM_SET_PRESCALER(&htim6, psc);
    // arr不能为0，否则CNT寄存器不变
    __HAL_TIM_SET_AUTORELOAD(&htim6, arr);
}

void timer6_set_arr(uint32_t arr)
{
    __HAL_TIM_SET_AUTORELOAD(&htim6, arr);
}
uint32_t timer6_get_arr()
{
    return __HAL_TIM_GET_AUTORELOAD(&htim6);
}

/**
 * @brief 设置定时器2的时钟
 * @param psc 预分频
 * @param arr 自动重载值，不能为0
 */
void timer2_set_freq(uint32_t psc,uint32_t arr )
{
    __HAL_TIM_SET_PRESCALER(&htim2, psc);
    // arr不能为0，否则CNT寄存器不变
    __HAL_TIM_SET_AUTORELOAD(&htim2, arr);
}
