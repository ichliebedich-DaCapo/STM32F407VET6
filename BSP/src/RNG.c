//
// Created by fairy on 2025/1/11 15:03.
//
#include "RNG.h"
#include "JYZQ_Conf.h"
// 头文件

#include "stm32f4xx_hal.h"

RNG_HandleTypeDef hrng;

// 函数
void RNG_Init(void)
{
    __HAL_RCC_RNG_CLK_ENABLE();  // 使能 RNG 时钟
    hrng.Instance = RNG;
    if (HAL_RNG_Init(&hrng) != HAL_OK)
    {
        // 初始化失败处理
        Error_Handler();
    }
}


uint32_t Get_Random_Number(void)
{
    uint32_t random_number;
    if (HAL_RNG_GenerateRandomNumber(&hrng, &random_number) == HAL_OK)
    {
        return random_number;
    }
    else
    {
        // 生成失败处理
        return 0;  // 返回默认值
    }
}

// 处理 RNG 错误
//  RNG 可能会因为时钟问题或种子错误而失败。可以通过检查 RNG 的状态寄存器来诊断问题：
void Check_RNG_Status(void)
{
    if (__HAL_RNG_GET_FLAG(&hrng, RNG_FLAG_SECS))
    {
        // 种子错误
        __HAL_RNG_CLEAR_FLAG(&hrng, RNG_FLAG_SECS);
    }
    if (__HAL_RNG_GET_FLAG(&hrng, RNG_FLAG_CECS))
    {
        // 时钟错误
        __HAL_RNG_CLEAR_FLAG(&hrng, RNG_FLAG_CECS);
    }
}