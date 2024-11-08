//
// Created by 34753 on 2024/10/12.
//

#ifndef FURINA_WAVE_SIGNAL_HPP
#define FURINA_WAVE_SIGNAL_HPP

#include "JYZQ_Conf.h"
#ifdef USE_WAVE_SIGNAL
#include "waveData.h"


// 波形类型
//enum class WaveType : uint8_t
//{
//    Sine,// 正弦波
//    Square,// 方波
//    Triangle,// 三角波
//    Sawtooth,
//    Noise,// 可以使用stm32自带的硬件随机数生成器产生
//};

// 以采样点为256为1个周期，所以下面频率对应的定时器频率需要乘以256
enum class WaveFreq : uint8_t
{
    Freq_8K,
    Freq_16K,
};



class WaveSignal
{
public:





private:
//    static inline const uint8_t *pWave=sineWave;// 现在是常量指针，默认指向正弦波。如果需要噪声，那么需要把它们变为普通指针
};

#endif

#endif //FURINA_WAVE_SIGNAL_HPP
