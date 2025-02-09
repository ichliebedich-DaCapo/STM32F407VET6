//
// Created by fairy on 2024/10/22 16:34.
//
#ifndef SIMULATOR_UI_HPP
#define SIMULATOR_UI_HPP

#include "GUI_Base.hpp"

/*********************组件*******************/
namespace gui::widgets::main
{
    extern Component rect;//示波器方框
    extern Button btn_mode;// 模式
    extern Label label_mode;
    extern Button btn_freq;// 频率
    extern Label label_freq;
    extern Button btn_ratio_add;// 比例
    extern Button btn_ratio_sub;
    extern Label label_ratio;
    extern Button btn_bias_add;// 偏置
    extern Button btn_bias_sub;
    extern Label label_bias;
    extern Label label_tick;// 时刻
    extern Label label_info;// 信息
    extern Label label_title;// 标题
    extern ImageButton imgbtn_play;// 播放键
    extern Label label_cpu;
    extern Label label_wave_cnt;// 波形点数
    extern Label label_wave_type;// 波形类型
    extern Label label_wave_generate;// 波形生成
    extern Label label_wave_period;// 波形周期
}

namespace gui::interface
{
    // 是否显示FPS
    static auto show_fps(bool is_show) -> void;

    // 是否显示FPS一帧时间
    static auto set_fps_mode(bool fps_mode) -> void;

    // 增加波形点数
    static auto add_wave_cnt() -> void;

    // 减少波形点数
    static auto sub_wave_cnt() -> void;

    // 切换波形类型
    static auto switch_wave_type() -> void;

    // 波形已产生
    static auto wave_is_generate() -> void;

    // 波形没有产生
    static auto wave_is_not_generate() -> void;

    // 清屏
    static auto clear_screen() -> void;

    // 增加时钟周期
    static auto add_period() -> void;

    // 减少时钟周期
    static auto sub_period() -> void;

    // 获取时钟周期
    static auto get_period() -> uint32_t;

    // 获取比例
    static auto get_ratio() -> uint8_t;

    // 获取偏置
    static auto get_bias() -> int8_t;

    // 获取波形类型
    static auto get_mode() -> bool ;

    // 获取频率
    static auto get_freq() -> bool;
}



// 加载资源文件
LV_FONT_DECLARE(lv_customer_font_SourceHanSerifSC_Regular_15)
LV_FONT_DECLARE(lv_customer_font_SourceHanSerifSC_Regular_13)

LV_IMG_DECLARE(btn_list_pause);
LV_IMG_DECLARE(btn_list_play);


#endif //SIMULATOR_UI_HPP