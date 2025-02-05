//
// Created by fairy on 2024/10/10.
//
#include "adc.h"
#ifdef BSP_USE_ADC
#include "timer.h"

ADC_HandleTypeDef hadc1;

#define ADC1_PIN GPIO_PIN_0
#define ADC1_GPIO_Port GPIOA


/**
 * @brief ADC1初始化函数
 * @details 使用Timer2来触发，默认引脚为PA0（ADC_CHANNEL_0）,如果是别的通道需要自行添加。
 *          需要自己初始化TIM2
 */
void adc1_init(uint32_t Channel)
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

    sConfig.Channel = Channel;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
    HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    /* ADC1 interrupt Init */
    HAL_NVIC_SetPriority(ADC_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(ADC_IRQn);
}


/**
 * @brief 初始化温度传感器
 * @details 使用同一个ADC1，暂时没有做多通道。
 */
void adc1_temperature_sensor_init()
{
    __HAL_RCC_ADC1_CLK_ENABLE();

    ADC_ChannelConfTypeDef sConfig = {0};

    hadc1.Instance = ADC1;
    hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
    hadc1.Init.Resolution = ADC_RESOLUTION_12B;
    hadc1.Init.ScanConvMode = DISABLE;
    hadc1.Init.ContinuousConvMode = DISABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion = 1;
    hadc1.Init.DMAContinuousRequests = DISABLE;
    hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
    if (HAL_ADC_Init(&hadc1) != HAL_OK)
    {
        Error_Handler();
    }


    sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
}

void adc1_start_it()
{
    timer2_start();
    HAL_ADC_Start_IT(&hadc1);
}

void adc1_stop_it()
{
    timer2_stop();
    HAL_ADC_Stop_IT(&hadc1);
}

/**********************后面这些代码为轮询状态的，暂时没有验证，肯定有不少问题*******************************/

/**
 * @brief 配置ADC通道
 * @param hadc:ADC句柄
 * @param channel:通道值
 */
void config_ADC1_Channel(uint32_t channel)
{
    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = channel;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
 * @brief 获取ADC值
 * @param hadc
 * @param channel
 * @return
 */
uint16_t get_ADC1_value(uint32_t channel)
{
    config_ADC1_Channel(channel);
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
    return HAL_ADC_GetValue(&hadc1);
}

/**
 * @brief 获取ADC平均值
 * @param hadc
 * @param channel
 * @param times
 * @return
 */
uint16_t get_ADC1_average(uint32_t channel, uint8_t times)
{
    uint32_t temp_val = 0;
    for (int t = 0; t < times; t++)
    {
        temp_val += get_ADC1_value(channel);
        HAL_Delay(5); // 延时5ms
    }
    return temp_val / times;
}

/**
 * @brief 获取温度传感器值
 * @details 不能读取过于频繁
 * @return
 */
float get_adc1_temperature()
{
    float temperature = 0;
    HAL_ADC_Start(&hadc1);
    if (HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY) == HAL_OK)
    {
        temperature = (float) HAL_ADC_GetValue(&hadc1) * 3300.0f / 4095.0f;
        temperature = (temperature - 760) / 2.5f + 25.0f;// 760是25度时的电压，这里并未做校准，2.5f是温度传感器的斜率
    }
    return temperature;
}


void ADC_IRQHandler(void)
{
    HAL_ADC_IRQHandler(&hadc1);
}

#endif