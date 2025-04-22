module; // 全局模块片段用于包含C头文件
// ==================包含头文件====================
#include <project_config.h>
// ==================包含头文件====================
export module adc; // 声明模块

// 引入模块
import timer;


export namespace bsp::adc
{
    // 硬件句柄（inline确保单例）
    inline ADC_HandleTypeDef hadc1 = {};

    // ADC 操作接口
    void init(uint32_t channel,uint32_t sampling_time) noexcept;

    void init_temperature_sensor() noexcept;

    void start() noexcept;

    void stop() noexcept;

    // 数据采集接口
    uint16_t read(uint32_t channel) noexcept;

    uint16_t read_average(uint32_t channel, uint8_t samples) noexcept;

    float read_temperature() noexcept;
}




namespace bsp::adc::detail
{
    // 硬件常量（替代原宏）
    #define gpio_port  GPIOA
    static constexpr uint16_t gpio_pin = GPIO_PIN_0;
    static constexpr uint32_t sampling_time = ADC_SAMPLETIME_480CYCLES;

    // 内部工具函数
    static void configure_gpio() noexcept
    {
        __HAL_RCC_GPIOA_CLK_ENABLE();
        GPIO_InitTypeDef init = {
            .Pin = gpio_pin,
            .Mode = GPIO_MODE_ANALOG,
            .Pull = GPIO_NOPULL
        };
        HAL_GPIO_Init(gpio_port, &init);
    }

    // 中断回调处理
    inline void (*user_callback)() = nullptr;
}

// 中断处理实现
extern "C" void ADC_IRQHandler()
{
    HAL_ADC_IRQHandler(&bsp::adc::hadc1);
    if (bsp::adc::detail::user_callback)
    {
        bsp::adc::detail::user_callback();
    }
}

// ================== 实现导出的函数 ==================
void bsp::adc::init(const uint32_t channel, const uint32_t sampling_time) noexcept
{
    using namespace detail;

    __HAL_RCC_ADC1_CLK_ENABLE();
    configure_gpio();

    hadc1.Instance = ADC1;
    hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
    hadc1.Init.Resolution = ADC_RESOLUTION_12B;
    hadc1.Init.ScanConvMode = DISABLE;
    hadc1.Init.ContinuousConvMode = DISABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion = 1;
    hadc1.Init.DMAContinuousRequests = DISABLE;
    hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;

    hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISINGFALLING;
    hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T2_TRGO;
    HAL_ADC_Init(&hadc1);

    ADC_ChannelConfTypeDef ch_cfg = {
        .Channel =channel,
        .Rank = 1,
        .SamplingTime = sampling_time
    };
    HAL_ADC_ConfigChannel(&hadc1, &ch_cfg);

    HAL_NVIC_SetPriority(ADC_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(ADC_IRQn);
}

void bsp::adc::init_temperature_sensor() noexcept
{
    using namespace detail;

    __HAL_RCC_ADC1_CLK_ENABLE();

    hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    if (HAL_ADC_Init(&hadc1) != HAL_OK)
    {
        Error_Handler();
    }

    ADC_ChannelConfTypeDef ch_cfg = {
        .Channel = ADC_CHANNEL_TEMPSENSOR,
        .Rank = 1,
        .SamplingTime = ADC_SAMPLETIME_480CYCLES
    };
    if (HAL_ADC_ConfigChannel(&hadc1, &ch_cfg) != HAL_OK)
    {
        Error_Handler();
    }
}

void bsp::adc::start() noexcept
{
    timer2::start();
    HAL_ADC_Start_IT(&hadc1);
}

void bsp::adc::stop() noexcept
{
    timer2::stop();
    HAL_ADC_Stop_IT(&hadc1);
}

uint16_t bsp::adc::read(uint32_t channel) noexcept
{
    ADC_ChannelConfTypeDef ch_cfg = {
        .Channel = channel,
        .Rank = 1,
        .SamplingTime = bsp::adc::detail::sampling_time
    };
    HAL_ADC_ConfigChannel(&hadc1, &ch_cfg);

    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
    return HAL_ADC_GetValue(&hadc1);
}

uint16_t bsp::adc::read_average(uint32_t channel, uint8_t samples) noexcept
{
    uint32_t sum = 0;
    for (uint8_t i = 0; i < samples; ++i)
    {
        sum += read(channel);
        HAL_Delay(5);
    }
    return sum / samples;
}

float bsp::adc::read_temperature() noexcept
{
    HAL_ADC_Start(&hadc1);
    if (HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY) != HAL_OK)
    {
        return 0.0f;
    }

    const float voltage = (HAL_ADC_GetValue(&hadc1) * 3300.0f / 4095.0f);
    return (voltage - 760.0f) / 2.5f + 25.0f;
}
