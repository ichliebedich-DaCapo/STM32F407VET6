//
// Created by fairy on 2025/1/9 13:31.
//
#include <etl/array.h>
#include <app.hpp>

#ifdef GUI_ENABLE

import gui;

#endif
#ifdef FREERTOS_ENABLE
#include "cmsis_os2.h"
#endif
// 头文件


import key_adapter;

import async_delay;
import delay;
import adc;
import rng;
import usart;
import key;
import hw_registers;
import ads1115;

using AsyncDelay_HAL = utils::async_delay<HAL_GetTick>;
AsyncDelay_HAL async_delay(100);

// 经过测试 DA输出频率要达到1K以上偏置稳定，频率越高越稳定
namespace FPGA
{
    constexpr uint32_t FREQ_1K = 42950;
    constexpr uint32_t FREQ_10K = 429497;
    constexpr uint32_t FREQ_100K = 4294967;
    constexpr uint32_t FREQ_1M = 42949673;
}
using FREQ_WORD = Register<0x60000000>;

static volatile int16_t adc_value = 0;
static volatile float adc_mv = 0;

/**
 * RLC测量
 * @details 电压单位均为mV
 */
class RLC_Measure
{
public:
    // 测量出
    static auto init()
    {
        FREQ_WORD::write(FPGA::FREQ_1K);
        async_delay.reset();

        calibrate();// 初始校准
    }

    static auto measure()
    {
        // 配置参数
        constexpr float alpha = 0.2f;// 滤波参数,越小滤波效果越强
        constexpr int samples_size = 5;
        static float filtered_adc_VQ = 0.0f, filtered_adc_VI = 0.0f;

        // 首次读取并初始化
        int32_t sum_VQ = 0, sum_VI = 0;
        for (int i = 0; i < samples_size; ++i)
        {
            sum_VQ += bsp::adc::ADS1115::read(bsp::adc::ads1115::MuxConfig::Single_1);
            sum_VI += bsp::adc::ADS1115::read(bsp::adc::ads1115::MuxConfig::Single_2);
        }
        filtered_adc_VQ = static_cast<float>(sum_VQ) / 5.0f;
        filtered_adc_VI = static_cast<float>(sum_VI) / 5.0f;

        // 读5次并实时滤波
        for (int i = 0; i < 5; i++)
        {
            int16_t raw_adc_VQ = bsp::adc::ADS1115::read(bsp::adc::ads1115::MuxConfig::Single_1);
            filtered_adc_VQ = alpha * raw_adc_VQ + (1.0f - alpha) * filtered_adc_VQ;

            int16_t raw_adc_VI = bsp::adc::ADS1115::read(bsp::adc::ads1115::MuxConfig::Single_2);
            filtered_adc_VI = alpha * raw_adc_VI + (1.0f - alpha) * filtered_adc_VI;
        }

        // 使用滤波后的值
        adc_data_VQ[index] = static_cast<int16_t>(filtered_adc_VQ + 0.5f);
        adc_data_VI[index] = static_cast<int16_t>(filtered_adc_VI + 0.5f);
    }

    // 计算阻抗
    static auto calculate_Z()
    {
        // 电压单位均为mV
        constexpr float A = 910.0f;
        constexpr float A2 = A * A;
        constexpr float R0 = 200;
        constexpr float ADC_RANGE = 2048.0f;

        float VI = (adc_data_VI[index] - adc_offset_VI) * ADC_RANGE / 32768;
        float VQ = (adc_data_VQ[index] - adc_offset_VQ) * ADC_RANGE / 32768;
        float VI2_and_VQ2 = VI * VI + VQ * VQ;

        R[index] = A2 * R0 * VI / (2 * VI2_and_VQ2) - R0;
        X[index] = A2 * R0 * VQ / (2 * VI2_and_VQ2);
    }

    // 计算元件参数（先判断元件类型）
    static auto calculate_element_parameter()
    {
        constexpr uint32_t freq_word[FREQ_WORD_NUM] = {FPGA::FREQ_1K, FPGA::FREQ_10K, FPGA::FREQ_100K, FPGA::FREQ_1M};
        index = (index + 1) & 0x3;// 对4取模
        FREQ_WORD::write(freq_word[index]);

        if (index == 0)
        {


        }

    }

    // 校准
    static auto calibrate() -> void
    {
        int32_t sum_VQ = 0, sum_VI = 0;
        for (int i = 0; i < 5; ++i)
        {
            sum_VQ += bsp::adc::ADS1115::read(bsp::adc::ads1115::MuxConfig::Single_1);
            sum_VI += bsp::adc::ADS1115::read(bsp::adc::ads1115::MuxConfig::Single_2);
        }
        adc_offset_VQ = static_cast<int16_t>(sum_VQ / 5.0f);
        adc_offset_VI = static_cast<int16_t>(sum_VI / 5.0f);
    }

private:
    static constexpr int FREQ_WORD_NUM = 4;// 这里如果要改，请到calculate_element_parameter函数里改取模逻辑
    static inline float R_parameter = 0;
    static inline float L_parameter = 0;
    static inline float C_parameter = 0;
    static inline uint8_t index = 0;
    static inline int16_t adc_offset_VI = 0;
    static inline int16_t adc_offset_VQ = 0;
    static inline std::array<int16_t, SIZE> adc_data_VI;
    static inline std::array<int16_t, SIZE> adc_data_VQ;
    static inline std::array<float, SIZE> R;
    static inline std::array<float, SIZE> X;
};


namespace app
{
    void Control::init()
    {
        using namespace bsp;
        adc::init_temperature_sensor();
        rng::init();
        usart::init();
        adc::ADS1115::init();

        GPIO_InitTypeDef GPIO_InitStruct = {};
        GPIO_InitStruct.Pin = GPIO_PIN_5;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        // 第一阶段：复位信号激活（低电平有效时省略此步）
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET); // 释放复位
        HAL_Delay(10); // 保证稳定

        // 正式复位脉冲
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET); // 拉低复位
        HAL_Delay(50); // 维持复位状态时间（典型值20-100ms）

        // 结束复位
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET); // 释放复位
    }

    void Control::background_process()
    {    // 获取ADC值
        if (async_delay.is_timeout())
        {
            RLC_Measure::measure();
            RLC_Measure::calculate_Z();
            RLC_Measure::calculate_element_parameter();
        }
    }
}


namespace utils
{
    void Key::handler()
    {
        switch (PlatformKey::getCode())
        {
            case keyK0:
                // 1K
                break;

            case keyK1:
                // 10K
                break;

            case keyK2:
                // 100K
                break;


            case keyKF:
                break;
            default:
                break;
        }
    }
}


/**实现中断服务例程*/
// 用于采集ADC数据
void adc1_isr()
{

}
