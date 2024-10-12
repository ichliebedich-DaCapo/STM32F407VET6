//
// Created by 34753 on 2024/10/12.
//

#ifndef FURINA_WAVE_SIGNAL_H
#define FURINA_WAVE_SIGNAL_H

#include "JYZQ_Conf.h"

#if USE_WAVE_SIGNAL

enum class WaveMode : uint8_t
{
    Freq_8K,// 8KHz单频波
    Freq_16K,// 16KHz单频波
    MixFreq_8K_16K,// 8KHz与16KHz混合波
};

// 波形类型
enum class WaveType : uint8_t
{
    Sine,
    Square,
    Triangle,
    Sawtooth,
    Noise,
};

class WaveSignal
{
public:
    auto on() -> void;

    auto off() -> void;

    auto set_mode(WaveMode mode) -> void;

    auto set_type(WaveType type) -> void;

    static auto check_mode() -> WaveMode
    {
        return static_cast<WaveMode>(status & static_cast<uint8_t>(FlagStatus::Mode));
    }

private:
    static inline uint32_t count = 0;
    static inline uint8_t *pWave;// 默认指向正弦波
    static inline uint16_t status = 0;
    static inline uint8_t index = 0;// 波形索引，考虑到内存有限，所以一般波形数据只有256个采样点

    enum class FlagStatus : uint8_t
    {
        Mode = 0x3,// bits:0-1，模式
        Reset = 0x4,// bit:2 重置
        Switch = 0x8,// bit:3 交换
    };
};

#endif

#endif //FURINA_WAVE_SIGNAL_H
