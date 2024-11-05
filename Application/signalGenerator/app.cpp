//
// Created by fairy on 2024/10/12.
//

#include "app.hpp"

#include "timer.h"
#include "dac.h"
#include "wave_signal.hpp"
#include "key.hpp"
#include "GUI.hpp"
#include "WaveCurve.hpp"

void app_init()
{
    timer6_init(FREQ_84M_to_128x8K);// 预期1KHz
    dac_init();
}

void key_handler()
{

    switch (Key::getCode())
    {
        case keyk0:// 播放
            if (Key::stateHandler(KEY_STATE_TWO))
            {
                ImageButton::press(GUI_Base::get_ui()->main.imgbtn_play);

            } else
            {
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
            Button::click(GUI_Base::get_ui()->main.btn_bias_sub);
            break;

        case keyk3:// 偏置+
            Button::click(GUI_Base::get_ui()->main.btn_bias_add);
            break;

        case keyk4:// 比例-
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


        case keyk7:// 波形是否产生
            if (Key::stateHandler(KEY_STATE_TWO))
            {
                uiInterface::wave_is_generate();
                WaveSignal::on();
            } else
            {
                uiInterface::wave_is_not_generate();
                WaveSignal::off();
            }
            break;

        case keyk8:// 减少波形点数
            uiInterface::sub_wave_cnt();
            break;
        case keyk9:// 增加波形点数
            uiInterface::add_wave_cnt();
            break;
        case keykA:// 减少周期
            uiInterface::sub_period();
            break;
        case keykB:// 增加周期
            uiInterface::add_period();
            break;
        case keykC:// 切换波形类型
            uiInterface::switch_wave_type();

            break;
        case keykD:// 清屏
            uiInterface::clear_screen();
            break;
        case keykE:// 是否显示FPS一帧时间
            if (Key::stateHandler(KEY_STATE_TWO))
            {
                uiInterface::set_fps_mode(true);
            } else
            {
                uiInterface::set_fps_mode(false);
            }
            break;
        case keykF:// 是否显示FPS
            if (Key::stateHandler(KEY_STATE_TWO))
            {
                uiInterface::show_fps(true);
            } else
            {
                uiInterface::show_fps(false);
            }
            break;

        default:
            break;
    }
}

extern "C"
{
void timer6_isr()
{
    WaveSignal::generate();
}
}
