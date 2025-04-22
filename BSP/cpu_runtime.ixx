//
// Created by fairy on 2024/10/7.
//
module;
#include <project_config.h>
export module cpu_runtime;


export namespace bsp::cpu_runtime
{
    inline TIM_HandleTypeDef htim13;

}


namespace
{
    /* 用于统计运行时间 */
    CCMRAM_VAR volatile uint32_t CPU_RunTime = 0UL;
}


export extern "C"
{
void ConfigureTimerForRunTimeStats()
{
    using namespace bsp::cpu_runtime;
    __HAL_RCC_TIM13_CLK_ENABLE();
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    htim13.Instance = TIM13; //总频84MHz，现为15000Hz
    htim13.Init.Prescaler = 55;
    htim13.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim13.Init.Period = 99;
    htim13.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    HAL_TIM_Base_Init(&htim13);

    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_ENABLE;
    HAL_TIMEx_MasterConfigSynchronization(&htim13, &sMasterConfig);


    HAL_NVIC_SetPriority(TIM8_UP_TIM13_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(TIM8_UP_TIM13_IRQn);

    HAL_TIM_Base_Start_IT(&htim13); //不要忘记开启中断
}

void TIM8_UP_TIM13_IRQHandler()
{
    using namespace bsp::cpu_runtime;
    //    if (__HAL_TIM_GET_IT_SOURCE(&htim13, TIM_IT_UPDATE) != RESET)
    //    {
    __HAL_TIM_CLEAR_IT(&htim13, TIM_IT_UPDATE);
    ++CPU_RunTime;
    //    }
}
}
