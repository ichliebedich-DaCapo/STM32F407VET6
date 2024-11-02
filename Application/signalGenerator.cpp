//
// Created by fairy on 2024/10/12.
//

#include "signalGenerator.hpp"

#if ENABLE_SIGNAL_GENERATOR

#include "timer.h"
#include "dac.h"
#include "wave_signal.hpp"
#include "key.hpp"
#include "GUI.hpp"

void app_init()
{
    timer6_init(FREQ_84M_to_1K);
    dac_init();
}

void key_handler()
{
    switch (Key::getCode())
    {
        case keyk0:// 播放
            if (Key::stateHandler(KEY_STATE_TWO))
            {
//                WaveSignal::on();
                ImageButton::press(GUI_Base::get_ui()->main.imgbtn_play);
            } else
            {
//                WaveSignal::off();
                ImageButton::release(GUI_Base::get_ui()->main.imgbtn_play);
            }
            break;

        case keyk1:// 播放模式
            if (Key::stateHandler(KEY_STATE_TWO))
            {
                WaveSignal::set_mode(WaveMode::MIX_FREQ);// 混合

            } else
            {
                WaveSignal::set_mode(WaveMode::SINGLE_FREQ);// 单频
            }
            Button::click(GUI_Base::get_ui()->main.btn_mode);
            break;

        case keyk2:// 偏置-
#warning "未实现偏置"
            Button::click(GUI_Base::get_ui()->main.btn_bias_sub);
            break;

        case keyk3:// 偏置+
            Button::click(GUI_Base::get_ui()->main.btn_bias_add);
            break;

        case keyk4:// 比例-
#warning "未实现比例"
            Button::click(GUI_Base::get_ui()->main.btn_ratio_sub);
            break;

        case keyk5:// 比例+
            Button::click(GUI_Base::get_ui()->main.btn_ratio_add);
            break;


        case keyk6:// 频率
            if (Key::stateHandler(KEY_STATE_TWO))
            {
                WaveSignal::set_frequency(WaveFreq::Freq_16K);
            } else
            {
                WaveSignal::set_frequency(WaveFreq::Freq_8K);
            }
            Button::click(GUI_Base::get_ui()->main.btn_freq);
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