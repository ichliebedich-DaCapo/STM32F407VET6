//
// Created by fairy on 2025/1/9 13:31.
//
/**
 * 【简介】：经过粗测，切换频率后稳定时间至少需要20ms(变化≤5)，34ms（变化≤1）
 */
#include <array>
#include <tuple>
#include <cmath>
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
    constexpr uint32_t FREQ_1K = 171799;//42950;
    constexpr uint32_t FREQ_10K = 1717987;//429497;
    constexpr uint32_t FREQ_100K = 17179869;//4294967;
    constexpr uint32_t FREQ_500K = 85899346;
    constexpr uint32_t FREQ_1M = 171798692;//42949673;
}
using FREQ_WORD = Register<0x60000000>;



/**
 * RLC测量
 * @details 电压单位均为mV
 */
float temp_VI,temp_VQ;
class RLC_Measure
{
public:
    // 测量出
    static auto init()
    {
//        gui::RLC::set_reset_callback(calibrate);// 设置重置回调
        FREQ_WORD::write(FPGA::FREQ_1K);
        HAL_Delay(50);
        calibrate();// 初始校准
        async_delay.reset();
    }

    static auto measure()
    {
        // 配置参数
        constexpr float alpha = 0.2f;// 滤波参数,越小滤波效果越强
        constexpr int samples_size = 10;
        static float filtered_adc_VQ = 0.0f, filtered_adc_VI = 0.0f;

        // 首次读取并初始化
        int32_t sum_VQ = 0, sum_VI = 0;
        for (int i = 0; i < samples_size; ++i)
        {
            sum_VQ += bsp::adc::ADS1115::read(bsp::adc::ads1115::MuxConfig::Single_1);
            sum_VI += bsp::adc::ADS1115::read(bsp::adc::ads1115::MuxConfig::Single_2);
        }
        filtered_adc_VQ = static_cast<float>(sum_VQ) / 10.0f;
        filtered_adc_VI = static_cast<float>(sum_VI) / 10.0f;

//        // 读5次并实时滤波
//        for (int i = 0; i < 5; i++)
//        {
//            float raw_adc_VQ = bsp::adc::ADS1115::read(bsp::adc::ads1115::MuxConfig::Single_1);
//            filtered_adc_VQ = alpha * raw_adc_VQ + (1.0f - alpha) * filtered_adc_VQ;
//
//            float raw_adc_VI = bsp::adc::ADS1115::read(bsp::adc::ads1115::MuxConfig::Single_2);
//            filtered_adc_VI = alpha * raw_adc_VI + (1.0f - alpha) * filtered_adc_VI;
//        }
//
//        // 使用滤波后的值
//        adc_data_VQ[index] = static_cast<int16_t>(filtered_adc_VQ + 0.5f-(float)adc_offset_VQ);
//        adc_data_VI[index] = static_cast<int16_t>(filtered_adc_VI + 0.5f-(float)adc_offset_VI);
       //
        adc_data_VQ[index] = static_cast<int16_t>(filtered_adc_VQ -(float)adc_offset_VQ);
        adc_data_VI[index] = static_cast<int16_t>(filtered_adc_VI -(float)adc_offset_VI);
    }

    // 计算阻抗

    static auto calculate_Z()
    {
        // 电压单位均为V,用mV会出现问题
//        constexpr float A = 910.0f;//1.84-1.86 10~13 ***** 1.88-1.92 -4~-6
//        constexpr float A2 = A * A;
        constexpr float A1 = 0.925;
        constexpr float A2 = 0.950;
        constexpr float A1_2 = A1 * A1;
        constexpr float A2_2 = A2 * A2;
        constexpr float R0 = 200;
        constexpr float R_C = A1_2*A2_2*R0;// R的分子系数
        constexpr float X_C = A1_2*A1*A2*R0;// X的分子系数
        const float ADC_RANGE = 2.048f;

        float VI =( (float)adc_data_VI[index]  * ADC_RANGE / 32768/1.6f);
        float VQ =( (float)adc_data_VQ[index]  * ADC_RANGE / 32768/1.6f);

        const float div_C = 2*(A2_2 * VI * VI + A1_2* VQ * VQ);// 分母系数


        R[index] = (A1_2/2/VI-1)*R0;//R_C*VI/div_C-R0;

        X[index] = -X_C*VQ/div_C;
        //        R[index] = A2 * R0 * VI / (2 * VI2_and_VQ2) - R0;
//        X[index] = A2 * R0 * VQ / (2 * VI2_and_VQ2);

    }


    static auto get_VI(){return adc_data_VI;}
    static auto get_VQ(){return adc_data_VQ;}
    static auto get_parameter()
    {
            return std::tie(adc_offset_VI, adc_offset_VQ,R_parameter, L_parameter, C_parameter);
    }

    // 计算元件参数（先判断元件类型）
    enum class ElementType
    {
        RESISTOR, CAPACITOR, INDUCTOR, UNKNOWN
    };

    static auto calculate_element_parameter()
    {
        constexpr uint32_t freq_word[FREQ_WORD_NUM] = {FPGA::FREQ_1K, FPGA::FREQ_10K, FPGA::FREQ_100K, FPGA::FREQ_500K};
        constexpr std::array<float, 4> frequencies = {1000.0f, 10000.0f, 100000.0f, 500000.0f};

        index = (index + 1) & 0x3;// 对4取模
        FREQ_WORD::write(freq_word[index]);

        if (index == 0)
        {
            type = ElementType::UNKNOWN;// 重置类型
            R_parameter  = 0;
            L_parameter  = 0;
            C_parameter  = 0;

            // 线性拟合
            float R_avg = R[0]*0.936928f-15.81321f;// 只取1KHZ下的阻值

            // 检查X是否可视为零（电阻）
            float max_X_abs = 0.0f;
            for (float x: X)
            {
                float abs_x = std::abs(x);
                if (abs_x > max_X_abs) max_X_abs = abs_x;
            }

            // 阈值设为R平均值的1%或最小0.1Ω
            const float threshold = std::max(R_avg * 0.05f, 1.0f);
            if (max_X_abs < threshold)
            {
                type = ElementType::RESISTOR;
                R_parameter = R_avg;
                return;
            }

            // 检查X符号一致性
            bool all_positive = true, all_negative = true;
            for (auto x: X)
            {
                if (x <= 0.0f) all_positive = false;
                if (x >= 0.0f) all_negative = false;
            }

            // 电感判断：X全正且与频率线性相关
            if (all_positive)
            {
                float L_avg = 0.0f;
                for (size_t i = 0; i < 4; ++i)
                {
                    L_avg += X[i] / (2 * M_PI * frequencies[i]);
                }
                L_avg /= 4.0f;
                type = ElementType::INDUCTOR;
                L_parameter = L_avg;
                return;
            }

            // 电容判断：X全负且与频率反比
            if (all_negative)
            {
                float C_avg = 0.0f;
                for (size_t i = 0; i < 4; ++i)
                {
                    C_avg += 1.0f / (2 * M_PI * frequencies[i] * std::abs(X[i]));
                }
                C_avg /= 4.0f;
                type = ElementType::CAPACITOR;
                C_parameter = C_avg;
                return;
            }

            // 异常情况
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
        adc_offset_VQ = static_cast<int16_t>((float)sum_VQ / 5.0f);
        adc_offset_VI = static_cast<int16_t>((float)sum_VI / 5.0f);
    }
public:
    static constexpr int FREQ_WORD_NUM = 4;// 这里如果要改，请到calculate_element_parameter函数里改取模逻辑
private:
    static inline ElementType type = ElementType::UNKNOWN;
    static inline float R_parameter = 0;
    static inline float L_parameter = 0;
    static inline float C_parameter = 0;
    static inline uint8_t index = 0;
    static inline int16_t adc_offset_VI = 0;
    static inline int16_t adc_offset_VQ = 0;
    static inline std::array<int16_t, FREQ_WORD_NUM> adc_data_VI;
    static inline std::array<int16_t, FREQ_WORD_NUM> adc_data_VQ;
    static inline std::array<float, FREQ_WORD_NUM> R;
    static inline std::array<float, FREQ_WORD_NUM> X;
};



int32_t R_data[RLC_Measure::FREQ_WORD_NUM],X_data[RLC_Measure::FREQ_WORD_NUM];
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
        HAL_Delay(20); // 保证稳定
        RLC_Measure::init();
    }

    void Control::background_process()
    {    // 获取ADC值
        if (async_delay.is_timeout())
        {
            RLC_Measure::measure();
            RLC_Measure::calculate_Z();
            RLC_Measure::calculate_element_parameter();

            for(int i=0;i<RLC_Measure::FREQ_WORD_NUM;++i)
            {
                R_data[i]=(int32_t)RLC_Measure::get_VI()[i];
                X_data[i]=(int32_t)RLC_Measure::get_VQ()[i];
            }
//            // FREQ_WORD_NUM个不同频点下的R和X
//            gui::RLC::generate_data(R_data, X_data,4);
//            // gui显示R、L、C等文本内容
//            auto [VI_VZ, VQ_VZ,R,L,C] = RLC_Measure::get_parameter();
//            gui::RLC::generate_text(VI_VZ, VQ_VZ, R,L,C);

        }
    }
}


uint32_t temp_tick,temp_tick2;
int16_t data1,data2;
namespace utils
{
    void Key::handler()
    {
        switch (PlatformKey::getCode())
        {
            case keyK0:
                // 1K
//                FREQ_WORD::write(FPGA::FREQ_1K);
//                HAL_Delay(100);
//                FREQ_WORD::write(FPGA::FREQ_1M);
//                temp_tick = HAL_GetTick();
//
//                 data2= bsp::adc::ADS1115::read(bsp::adc::ads1115::MuxConfig::Single_1);
//                do{
//                    data1 = data2;
//                    data2= bsp::adc::ADS1115::read(bsp::adc::ads1115::MuxConfig::Single_1);
//                }while(std::abs(data1-data2)>1);
//                temp_tick2 = HAL_GetTick();
//                temp_tick = temp_tick2-temp_tick;
//                __BKPT(0);
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
