//
// Created by 34753 on 2024/10/12.
//

#ifndef FURINA_WAVE_SIGNAL_HPP
#define FURINA_WAVE_SIGNAL_HPP

#include "JYZQ_Conf.h"
#ifdef USE_WAVE_SIGNAL
#include "waveData.h"

enum class WaveMode : uint8_t
{
    SINGLE_FREQ,// 单频波
    MIX_FREQ,// 混合波,默认是8K与16K混合
};

// 波形类型
enum class WaveType : uint8_t
{
    Sine,// 正弦波
    Square,// 方波
    Triangle,// 三角波
    Sawtooth,
    Noise,// 可以使用stm32自带的硬件随机数生成器产生
};

// 以采样点为256为1个周期，所以下面频率对应的定时器频率需要乘以256
enum class WaveFreq : uint8_t
{
    Freq_8K,
    Freq_16K,
};

// 波形占空比
enum class WaveDuty : uint8_t
{
    Duty_10=1,// 10%
    Duty_20=2,// 20%
    Duty_30=3,// 30%
    Duty_40=4,// 40%
    Duty_50=5,// 50%
    Duty_60=6,// 60%
    Duty_70=7,// 70%
    Duty_80=8,// 80%
    Duty_90=9,// 90%
};

class WaveSignal
{
public:
    static auto on() -> void;

    static auto off() -> void;

    static auto set_mode(WaveMode wave_mode) -> void;

    static auto set_type(WaveType type) -> void;

    // 加占空比10%
   static auto add_duty()->uint8_t
   {
       if(duty!=WaveDuty::Duty_90)
       {
           duty = static_cast<WaveDuty>(static_cast<uint8_t>(duty)+1);
           set_duty(duty);
       }
       return static_cast<uint8_t>(duty);
    }

    // 减占空比10%
    static auto sub_duty()->uint8_t
    {
        if(duty!=WaveDuty::Duty_10)
        {
            duty = static_cast<WaveDuty>(static_cast<uint8_t>(duty)-1);
            set_duty(duty);
        }
        return static_cast<uint8_t>(duty);
    }

    static auto set_frequency(WaveFreq wave_freq) -> void;

    static auto generate() -> void;


private:
    static auto reset_count() -> void { count = get_clock(); }// 重置计数

    static auto get_clock() -> uint32_t { return HAL_GetTick(); }// 获取当前时间

    static auto set_duty(WaveDuty wave_duty) -> void{ switch_count = max_count * static_cast<uint8_t>(wave_duty) / 10;}// 设置占空比

    static auto switch_freq() -> void
    {
        // 交换频率
        if (freq == WaveFreq::Freq_8K)
        {
            set_frequency(WaveFreq::Freq_16K);
        } else
        {
            set_frequency(WaveFreq::Freq_8K);
        }
    }

private:
    static inline uint32_t count = 0;
    static inline const uint8_t *pWave=sineWave;// 现在是常量指针，默认指向正弦波。如果需要噪声，那么需要把它们变为普通指针
    static inline uint16_t status = 0;
    constexpr const static uint16_t cycle = 3;// 周期为3秒
    constexpr const static uint16_t sys_freq = 1000;
    constexpr const static uint16_t max_count = sys_freq * cycle;
    constexpr const static uint16_t half_count = max_count / 2;
    static inline uint16_t switch_count = half_count;// 默认切频计数值为1/2周期
    static inline uint8_t index = 0;// 波形索引，考虑到内存有限，所以一般波形数据只有256个采样点
    static inline WaveMode mode = WaveMode::SINGLE_FREQ;// 默认单频
    static inline WaveFreq freq = WaveFreq::Freq_8K;// 默认8K
    static inline WaveDuty duty = WaveDuty::Duty_50;// 默认占空比50%
};

#endif

#endif //FURINA_WAVE_SIGNAL_HPP
