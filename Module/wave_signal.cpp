//
// Created by 34753 on 2024/10/12.
//

#include "wave_signal.hpp"

#ifdef USE_WAVE_SIGNAL

#include "timer.h"
#include "dac.h"

/**
 * @brief 设置波形模式
 * @param wave_mode 波形模式,枚举类型
 * @details 改变的是status，交由ISR来处理
 */
auto WaveSignal::set_mode(WaveMode wave_mode) -> void
{
    mode = wave_mode;
}

auto WaveSignal::on() -> void
{
    timer6_start_it();
    dac_start();
    reset_count();
}

auto WaveSignal::off() -> void
{
    timer6_stop_it();
    dac_stop();
}

/**
 * @brief 生成波形
 * @details 在ISR中调用即可
 */
auto WaveSignal::generate() -> void
{
    HAL_DAC_SetValue(&hdac, DAC1_CHANNEL_1, DAC_ALIGN_12B_R, pWave[index++]);
    // 检测模式
    if (mode == WaveMode::MIX_FREQ)
    {
        // 获取计差数值
        uint32_t temp_count = get_clock();
        if (temp_count >= count)
        {
            temp_count -= count;
        } else
        {
            temp_count += (0xFFFFFFFF - count);// 溢出处理
        }

        // 切频处理
        if (temp_count == switch_count)
        {
            switch_freq();
           /* 这个时候可以做一些别的处理，比如切换波形，但是这个机制还没有完善（低耦合）*/
        }

        // 计数值满就切频
        if (count == max_count)
        {
            // 重置计数值
            reset_count();
            switch_freq();
        }

    }

}

/**
 * @brief 设置频率
 * @param wave_freq 枚举类型
 * @details 还需要改变status
 */
auto WaveSignal::set_frequency(WaveFreq wave_freq) -> void
{
    freq = wave_freq;
    switch (wave_freq)
    {
        case WaveFreq::Freq_8K:
            timer6_set_freq(FREQ_84M_to_256x8K);// 256*8K
            break;

        case WaveFreq::Freq_16K:
            timer6_set_freq(FREQ_84M_to_256x16K);// 256*16K
            break;

        default:
            break;
    }
}

/**
 * @brief 设置波形类型
 * @param type 波形类型，分别为 正弦、方波、三角波、锯齿波等
 */
auto WaveSignal::set_type(WaveType type) -> void
{
    switch (type)
    {
        case WaveType::Sine:
            pWave = sineWave;
            break;

        case WaveType::Square:
            pWave = squareWave;
            break;

        case WaveType::Triangle:
            pWave = triangleWave;
            break;

        case WaveType::Sawtooth:
            pWave = sawtoothWave;
            break;

        default:
            break;
    }
}


#endif