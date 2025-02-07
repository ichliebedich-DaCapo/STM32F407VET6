//
// Created by DV on 2025/2/7.
//

#include "delay.h"

TIM_HandleTypeDef htim14;

void timer14_start()
{
    HAL_TIM_Base_Start(&htim14);
}

void timer14_stop()
{
    HAL_TIM_Base_Stop(&htim14);
}

// ����Ԥ��Ƶ
void timer14_set_psc(uint16_t psc)
{
    __HAL_TIM_SET_PRESCALER(&htim14, psc);
}

void delay_Init()
{
    __HAL_RCC_TIM14_CLK_ENABLE();

    TIM_MasterConfigTypeDef sMasterConfig = {0};

    htim14.Instance = TIM14;
    htim14.Init.Prescaler = 84-1;
    htim14.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim14.Init.Period = 0xFFFF;
    htim14.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&htim14) != HAL_OK)
    {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim14, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }
    // ������ʱ������Ƶ�����أ�������쳣
    timer14_start();
}

/**
 * @brief ΢�뼶��ʱ
 * @param us ΢��
 * @�� ֻ�ʺϵ��̣߳����Ҳ�������0����0xFFFF
 */
void delay_us(uint16_t us)
{
    // ������ʱ��

    // Ƶ��Ϊ84MHz��84M/1000 = 84000
    volatile uint16_t final_count =TIM14->CNT+us;

    // ���������
    while (TIM14->CNT>final_count);

    // �����
    while (TIM14->CNT < final_count);

    // ֹͣ��ʱ��
}
