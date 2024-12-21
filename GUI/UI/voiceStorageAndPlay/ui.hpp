//
// Created by fairy on 2024/10/18 20:36.
//
#ifndef SIMULATOR_UI_HPP
#define SIMULATOR_UI_HPP

#include "lvgl.h"
/* 预编译命令 */
#if 1
// 头文件
/*********************组件*******************/
struct lv_ui_t
{
    using Obj = lv_obj_t *;
    struct
    {
        Obj screen;// 屏幕自身
        Obj img_slider_flag;
        Obj imgbtn_slow;
        Obj imgbtn_acc;
        Obj label_title_music;
        Obj slider;
        Obj label_play_speed;
        Obj label_slider_time;
        Obj imgbtn_play;
        Obj spectrum;// 频谱
        Obj label_record_state;// 录音状态
        Obj label_save_state;// 保存状态
        Obj label_record_sample_rate;// 录音采样率
    } main;// 主屏幕

};
// 取别名
using lv_ui_t = struct lv_ui_t;

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
enum class RecordSampleRate
{
    SAMPLE_RATE_8K,
    SAMPLE_RATE_16K,
};

enum class PlaySpeed
{
    SPEED_NORMAL,// 正常速度
    SPEED_0_75,// 0.75倍
    SPEED_1_5,// 1.5倍
};

class UI_Interface
{
public:
    static auto reset_time() -> void;// 重置进度条时间
    static auto resume_record() -> void;// 开启进度条时间
    static auto pause_record() -> void;// 关闭进度条时间
    static auto set_record_state(RecordSampleRate state) -> void;
    static auto set_play_speed(PlaySpeed speed) -> void;
};




/* 预编译命令 */
#endif
#endif //SIMULATOR_UI_HPP