//
// Created by fairy on 2024/10/18 20:36.
//
#ifndef SIMULATOR_UI_HPP
#define SIMULATOR_UI_HPP
#include "GUI_Base.hpp"

namespace gui::widgets::main
{
    extern Image img_slider_flag;
    extern ImageButton imgbtn_slow;
    extern ImageButton imgbtn_acc;
    extern Label label_title_music;
    extern Slider slider;
    extern Label label_play_speed;
    extern Label label_slider_time;
    extern ImageButton imgbtn_play;
    extern Component spectrum;// 频谱
    extern Label label_record_state;// 录音状态
    extern Label label_save_state;// 保存状态
    extern Label label_record_sample_rate;// 录音采样率
}




// 加载资源文件
LV_FONT_DECLARE(lv_customer_font_SourceHanSerifSC_Regular_14)
LV_FONT_DECLARE(lv_customer_font_SourceHanSerifSC_Regular_12)
//
LV_IMG_DECLARE(_icn_slider_alpha_15x15);
LV_IMG_DECLARE(_btn_prev_alpha_37x37);
LV_IMG_DECLARE(_btn_next_alpha_37x37);
LV_IMG_DECLARE(_btn_list_play_alpha_48x48);
LV_IMG_DECLARE(_btn_list_pause_alpha_48x48);


// 录音采样率
enum class RecordSampleRate:uint8_t
{
    SAMPLE_RATE_8K=1,
    SAMPLE_RATE_16K=2,
};

enum class PlaySpeed: uint8_t
{
    SPEED_NORMAL=1,// 正常速度
    SPEED_0_75=75,// 0.75倍
    SPEED_1_5=150,// 1.5倍
};

namespace gui::interface
{
    auto reset_time() -> void;// 重置进度条时间
    auto get_time()->uint32_t ;
    auto set_time(uint32_t t)->void;
    auto resume_record() -> void;// 开启进度条时间
    auto pause_record() -> void;// 关闭进度条时间
    auto set_record_state(RecordSampleRate state) -> void;
    auto set_play_speed(PlaySpeed speed) -> void;
    auto saveInfo(bool state) -> void;
    auto erasing()->void ;
    auto erase_done()->void ;
}





#endif //SIMULATOR_UI_HPP