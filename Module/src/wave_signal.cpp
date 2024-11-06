//
// Created by fairy on 2024/10/12.
//

#include "wave_signal.hpp"

#ifdef USE_WAVE_SIGNAL

#include "timer.h"
#include "dac.h"


auto WaveSignal::on() -> void
{
    timer6_start_it();
    dac_start();
}

auto WaveSignal::off() -> void
{
    timer6_stop_it();
    dac_stop();
}

/**
 * @brief 设置频率
 * @param wave_freq 枚举类型
 * @details 还需要改变status
 */
auto WaveSignal::set_frequency(WaveFreq wave_freq) -> void
{
    switch (wave_freq)
    {
        case WaveFreq::Freq_8K:
            timer6_set_freq(FREQ_84M_to_256x800);// 256*800
            break;

        case WaveFreq::Freq_16K:
            timer6_set_freq(FREQ_84M_to_256x1k);// 256*1k
            break;

        default:
            break;
    }
}

/**
 * @brief 设置波形类型
 * @param type 波形类型，分别为 正弦、方波、三角波、锯齿波等
 */
//auto WaveSignal::set_type(WaveType type) -> void
//{
//    switch (type)
//    {
//        case WaveType::Sine:
//            pWave = sineWave;
//            break;
//
//        case WaveType::Square:
//            pWave = squareWave;
//            break;
//
//        case WaveType::Triangle:
//            pWave = triangleWave;
//            break;
//
//        case WaveType::Sawtooth:
//            pWave = sawtoothWave;
//            break;
//
//        default:
//            break;
//    }
//}


#endif