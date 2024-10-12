//
// Created by 34753 on 2024/10/10.
//

#include "adc.h"
#if USE_ADC
ADC_HandleTypeDef hadc1;

#define ADC1_PIN GPIO_PIN_0
#define ADC1_GPIO_Port GPIOA


/**
 * @brief ADC1初始化函数
 * @details 使用Timer2来触发，默认引脚为PA0
 */
void adc1_init()
{
    __HAL_RCC_ADC1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    ADC_ChannelConfTypeDef sConfig = {0};
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    /**ADC1 GPIO Configuration
    PA0-WKUP     ------> ADC1_IN0
    */
    GPIO_InitStruct.Pin = ADC1_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(ADC1_GPIO_Port, &GPIO_InitStruct);

    hadc1.Instance = ADC1;
    hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
    hadc1.Init.Resolution = ADC_RESOLUTION_12B;
    hadc1.Init.ScanConvMode = DISABLE;
    hadc1.Init.ContinuousConvMode = DISABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISINGFALLING;
    hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T2_TRGO;// 使用Timer2触发
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion = 1;
    hadc1.Init.DMAContinuousRequests = DISABLE;
    hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
    HAL_ADC_Init(&hadc1);

    sConfig.Channel = ADC_CHANNEL_0;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_56CYCLES;
    HAL_ADC_ConfigChannel(&hadc1, &sConfig);



    /* ADC1 interrupt Init */
    HAL_NVIC_SetPriority(ADC_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(ADC_IRQn);
}



#endif