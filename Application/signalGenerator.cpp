//
// Created by 34753 on 2024/10/12.
//

#include "signalGenerator.h"
#if ENABLE_SIGNAL_GENERATOR
#include "timer.h"
#include "dac.h"
#include "wave_signal.h"
#include "key.h"


void app_init()
{
    timer6_init(FREQ_84M_to_1K);
    dac_init();
}

void key_handler()
{
    switch (key.getCode())
    {
        case keyk0:// 播放
            if (key.stateHandler(KEY_STATE_TWO))
            {
                WaveSignal::on();
            } else
            {
                WaveSignal::off();
            }
            break;

            case keyk1:// 播放模式
            if (key.stateHandler(KEY_STATE_TWO))
            {
               WaveSignal::set_mode(WaveMode::MIX_FREQ);// 混合
            } else
            {
                WaveSignal::set_mode(WaveMode::SINGLE_FREQ);// 单频
            }
            break;

            case keyk2:// 频率
         if(key.stateHandler(KEY_STATE_TWO))
         {
             WaveSignal::set_frequency(WaveFreq::Freq_16K);
         }
         else
         {
             WaveSignal::set_frequency(WaveFreq::Freq_8K);
         }
            break;

            case keyk3:// 音量

            break;

            case keyk4:// 静音
                break;

        default:
            break;
    }
}

void timer6_isr()
{
    WaveSignal::generate();
}

#endif