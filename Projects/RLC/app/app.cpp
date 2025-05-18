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
// 电阻用1kHz
// 电容用1K到100K，电容越小，频率越高越不准,需要减小 1uF需要小于100k 100uF需要小于10K
// 电感用100K测量

// freq_out/25M = freq_word/2^32  →  freq_word = freq_out*2^32/25M
namespace FPGA
{
    constexpr uint32_t FREQ_1K = 171799;//42950;
    constexpr uint32_t FREQ_10K = 1717987;//429497;
    constexpr uint32_t FREQ_50K = 8589934;
    constexpr uint32_t FREQ_100K = 17179869;//4294967;
    constexpr uint32_t FREQ_500K = 85899346;
    constexpr uint32_t FREQ_1M = 171798692;//42949673;
}
using FREQ_WORD = Register<0x60000000>;


/**
 * RLC测量
 * @details 电压单位均为mV
 */
float temp_VI, temp_VQ;

class RLC_Measure
{
public:
    // 测量出
    static auto init()
    {
                gui::RLC::set_reset_callback(calibrate);// 设置重置回调
        FREQ_WORD::write(FPGA::FREQ_1K);
        HAL_Delay(50);
        calibrate();// 初始校准
        async_delay.reset();
    }

    static auto measure()
    {
        // 配置参数
        constexpr float alpha = 0.2f;// 滤波参数,越小滤波效果越强
        constexpr int samples_size = 20;
        constexpr float samples = 20.0f;
        static float filtered_adc_VQ = 0.0f, filtered_adc_VI = 0.0f;

        // 首次读取并初始化
        int32_t sum_VQ = 0, sum_VI = 0;
        for (int i = 0; i < samples_size; ++i)
        {
            sum_VQ += bsp::adc::ADS1115::read(bsp::adc::ads1115::MuxConfig::Single_1);
            sum_VI += bsp::adc::ADS1115::read(bsp::adc::ads1115::MuxConfig::Single_2);
        }
        filtered_adc_VQ = static_cast<float>(sum_VQ) / samples;
        filtered_adc_VI = static_cast<float>(sum_VI) / samples;

        adc_data_VQ[index] = static_cast<int16_t>(filtered_adc_VQ - (float) adc_offset_VQ);
        adc_data_VI[index] = static_cast<int16_t>(filtered_adc_VI - (float) adc_offset_VI);
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
        constexpr float R_C = A1_2 * A2_2 * R0;// R的分子系数
        constexpr float X_C = A1_2 * A1 * A2 * R0;// X的分子系数
        const float ADC_RANGE = 1.024f;

        float VI = ((float) adc_data_VI[index] * ADC_RANGE / 32768 / 1.6f);
        float VQ = ((float) adc_data_VQ[index] * ADC_RANGE / 32768 / 1.6f);

        const float div_C = 2 * (A2_2 * VI * VI + A1_2 * VQ * VQ);// 分母系数


        R[index] = (A1_2 / 2 / VI - 1) * R0;//R_C*VI/div_C-R0;

        X[index] = X_C * VQ / div_C;
        //        R[index] = A2 * R0 * VI / (2 * VI2_and_VQ2) - R0;
        //        X[index] = A2 * R0 * VQ / (2 * VI2_and_VQ2);

    }


    // 计算元件参数（先判断元件类型）
    enum class ElementType
    {
        RESISTOR,
        CAPACITOR,
        INDUCTOR, // 电感类型
        UNKNOWN
    };

    // 细测
    static auto calculate_element_parameter_precise()
    {
//        std::array<float, 8> frequencies{500.0f,700.0f,1000.0f, 1500.0f,2000.0f,3000.0f,5000.0f,8000.0f};
        std::array<float, 4> frequencies{500.0f,550.0f,600.0f, 650.0f};
        index = (index + 1) & 0x3;// 对3取模

        // 根据类型选择对应的频率范围
        switch (type)
        {
            case ElementType::INDUCTOR:
//                frequencies = {50000.0f,80000.0f,100000.0f,150000.0f,200000.0f,300000.0f,400000.0f,500000.0f};
                    frequencies ={300000.0f,325000.0f,350000.0f,400000.0f};
                break;
            case ElementType::CAPACITOR:
//                frequencies = {500.0f,520.0f,550.0f, 600.0f,700.0f,800.0f,900.0f,1000.0f};
                frequencies = {500.0f, 550.0f, 600.0f, 650.0f};
                break;
            case ElementType::UNKNOWN:
//                frequencies = {500.0f,700.0f,1000.0f, 1500.0f,2000.0f,3000.0f,5000.0f,8000.0f};
                break;
            default:
                break;
        }
        FREQ_WORD ::write(get_freq_word(frequencies[index]));

        if(index == 0)
        {
            // 检查X符号一致性
            bool all_positive = true, all_negative = true;
            for (int i = 0; i < 3; ++i)
            {
                if (X[i] <= 0.0f) all_positive = false;
                if (X[i] >= 0.0f) all_negative = false;
            }

            // 检查R、X是否是递增的
            bool x_increase_flag = true, r_increase_flag = true;
            for (int i = 0; i < 3; ++i)
            {
                if (X[i] > X[i + 1]) x_increase_flag = false;
                if (R[i] > R[i + 1]) r_increase_flag = false;
            }
            // 检查R、X是否是递减的
            bool x_decrease_flag = true, r_decrease_flag = true;
            for (int i = 0; i < 3; ++i)
            {
                if (X[i] < X[i + 1]) x_decrease_flag = false;
                if (R[i] < R[i + 1]) r_decrease_flag = false;
            }

            // ---------- 判断电感 ----------
            if(all_positive  && x_increase_flag)
            {
                precise_type = ElementType::INDUCTOR;
                R_parameter = 0;
                C_parameter = 0;

                // 计算电感
                float L_avg = 0.0f;
                for (size_t i = 0; i < 4; ++i)
                {
                    L_avg += 4.933e-6f+0.659937f*X[i] /(float)(2 * M_PI * frequencies[i]);
                }
                L_avg /= 4.0f;
                L_parameter = -L_avg*L_avg*6.3e-5f+1.398213f*L_avg-8.876001e-6f;
                // 检验元件类型是否变化
                goto FINAL_JUDGE;
            }

            // ---------- 判断电容 ----------
            // X基本递减，但是前两个未必递减，R基本不变
            if(all_negative)
            {
                precise_type = ElementType::CAPACITOR;
                R_parameter = 0;
                L_parameter = 0;

                // 计算电容
                float C_avg = 0.0f;
                for (size_t i = 0; i < 4; ++i)
                {
                    C_avg += 1.0f / (float)(2 * M_PI * frequencies[i] * std::abs(X[i]));
                }
                C_avg /= 4.0f;
                C_parameter = C_avg*1.078309f+2.001357e-8f;

                // 检验元件类型是否变化
                goto FINAL_JUDGE;
            }

            FINAL_JUDGE:
            if(type != precise_type || precise_type == ElementType::UNKNOWN)
            {
                reset_measure_precise_flag();// 启用粗测
                FREQ_WORD::write(FPGA::FREQ_1K);
            }
        }
    }

    static auto calculate_element_parameter()
    {
        constexpr uint32_t freq_word[FREQ_WORD_NUM] = {FPGA::FREQ_1K, FPGA::FREQ_10K, FPGA::FREQ_50K, FPGA::FREQ_100K};
        constexpr std::array<float, 4> frequencies = {1000.0f, 10000.0f, 50000.0f, 100000.0f};

        index = (index + 1) & 0x3;// 对4取模
        FREQ_WORD::write(freq_word[index]);

        if (index == 0)
        {
            type = ElementType::UNKNOWN;// 重置类型

            /// ============================= 小电阻判断 ===============================
            // 检查X是否可视为零（电阻）
            float max_X_abs = 0.0f;
            for (float x: X)
            {
                float abs_x = std::abs(x);
                if (abs_x > max_X_abs) max_X_abs = abs_x;
            }
            const float threshold = std::max(R[0] * 0.05f, 1.0f);// 阈值设为R平均值的1%或最小0.1Ω

            // 电阻类型：X很小，R会随着频率增加，但不会很高。并且电阻小于10KΩ
            // 其中adc_data_VQ[0]>-30这个条件是区分小电阻和大电容的，小电阻的电感性质会凸出一些
            if ((max_X_abs < threshold || (R[0] < 100 && adc_data_VQ[0]>-30)) && R[3] < R[0] * 1.5f && R[0] < 10000 )
            {
                type = ElementType::RESISTOR;
                L_parameter = 0;
                C_parameter = 0;

                // 线性拟合
//            float R_parameter = R[0]*0.936928f-15.81321f;// 只取1KHZ下的阻值
                if (R[0] > 226)
                {
//                R_parameter = -7.40365166e-7f*R[0]*R[0]+9.35024273e-1f*R[0]-1.93776772e1f;
                    R_parameter = 9.27304319e-1f * R[0] - 1.12925627e1f;
                }
                else
                {
//                R_parameter = 2.72047542e-5f*R[0]*R[0]*R[0]-7.18297741e-3f*R[0]*R[0]+1.46828988e0f*R[0]-2.64858679e1f;
                    R_parameter = 9.21542293e-1f * R[0] - 1.46575687e1f;
                }
                R_parameter =  1.6e-5f*R_parameter*R_parameter+0.986144f*R_parameter-13.50644f;
                return;
            }

            // 只要不是小电阻就一定要细测
            set_measure_precise_flag();


            /// ============================= 电感判断 ===============================
            // 检查X符号一致性
            bool all_positive = true, all_negative = true;
            for (int i = 0; i < 3; ++i)
            {
                if (X[i] <= 0.0f) all_positive = false;
                if (X[i] >= 0.0f) all_negative = false;
            }

            // 检查R、X是否是递增的
            bool x_increase_flag = true, r_increase_flag = true;
            for (int i = 0; i < 3; ++i)
            {
                if (X[i] > X[i + 1]) x_increase_flag = false;
                if (R[i] > R[i + 1]) r_increase_flag = false;
            }

            // 电感判断：R、X全正且与频率线性相关
            if (all_positive && x_increase_flag && r_increase_flag)
            {
                type = ElementType::INDUCTOR;
                FREQ_WORD ::write(get_freq_word(300000.0f));// 即细测频率的最小值
                R_parameter = 0;
                C_parameter = 0;

                float L_avg = 0.0f;
                for (size_t i = 0; i < 4; ++i)
                {
                    L_avg += X[i] / (2 * M_PI * frequencies[i]);
                }
                L_avg /= 4.0f;
                L_parameter = L_avg;
                return;
            }

            // 判断电容类型
            if(adc_data_VQ[0]<0)
            {
                type = ElementType::CAPACITOR;
                R_parameter = 0;
                L_parameter = 0;
                C_parameter = 1.0f / (float)(2 * M_PI * frequencies[0] * std::abs(X[0])*0.93698f-5462.0f)+1.8972e-8f;
                return;
            }

            // 未知元件类型，由细测来考虑
            FREQ_WORD ::write(get_freq_word(500.0f));// 即细测频率的最小值
        }
    }

    // 校准
    static auto calibrate() -> void
    {
        int32_t sum_VQ = 0, sum_VI = 0;
        for (int i = 0; i < 20; ++i)
        {
            sum_VQ += bsp::adc::ADS1115::read(bsp::adc::ads1115::MuxConfig::Single_1);
            sum_VI += bsp::adc::ADS1115::read(bsp::adc::ads1115::MuxConfig::Single_2);
        }
        adc_offset_VQ = static_cast<int16_t>((float) sum_VQ / 20.0f);
        adc_offset_VI = static_cast<int16_t>((float) sum_VI / 20.0f);
    }

public:
    static constexpr int FREQ_WORD_NUM = 4;// 这里如果要改，请到calculate_element_parameter函数里改取模逻辑
    static auto get_VI()
    { return adc_data_VI; }

    static auto get_VQ()
    { return adc_data_VQ; }

    static auto get_parameter()
    { return std::tie(adc_offset_VI, adc_offset_VQ, R_parameter, L_parameter, C_parameter); }

    static auto get_measure_flag(){ return measure_flag; }

private:
    static auto set_measure_precise_flag() -> void
    { measure_flag = true; }

    static auto reset_measure_precise_flag() -> void
    { measure_flag = false; }

    static auto get_freq_word(float freq)->uint32_t
    { return static_cast<uint32_t >(freq * 171.79869184f); }// 4294967296.0f/25000000.0f;
private:
    static inline ElementType type = ElementType::UNKNOWN;
    static inline ElementType precise_type = ElementType::UNKNOWN;
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
    static inline bool measure_flag = false;// ture:表示需要细测
};


int32_t R_data[RLC_Measure::FREQ_WORD_NUM], X_data[RLC_Measure::FREQ_WORD_NUM];
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

            if (RLC_Measure::get_measure_flag())
            {
                RLC_Measure::calculate_element_parameter_precise();
            } else
            {
                RLC_Measure::calculate_element_parameter();
            }


            for(int i=0;i<RLC_Measure::FREQ_WORD_NUM;++i)
            {
                R_data[i]=(int32_t)RLC_Measure::get_VI()[i];
                X_data[i]=(int32_t)RLC_Measure::get_VQ()[i];
            }
            // FREQ_WORD_NUM个不同频点下的R和X
            gui::RLC::generate_data(R_data, X_data,4);
            // gui显示R、L、C等文本内容
            auto [VI_VZ, VQ_VZ,R,L,C] = RLC_Measure::get_parameter();
            gui::RLC::generate_text(VI_VZ, VQ_VZ, R,L,C);

        }
    }
}


uint32_t temp_tick, temp_tick2;
int16_t data1, data2;
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
