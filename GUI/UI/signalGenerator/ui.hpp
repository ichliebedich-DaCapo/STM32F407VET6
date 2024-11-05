//
// Created by fairy on 2024/10/22 16:34.
//
#ifndef SIMULATOR_UI_HPP
#define SIMULATOR_UI_HPP

#include "JYZQ_Conf.h"
#include "lvgl.h"

/* 预编译命令 */
#if 1


/*********************组件*******************/
struct lv_ui_t
{
    using Obj = lv_obj_t *;
    // 主屏幕
    struct
    {
        Obj screen;// 屏幕自身
        Obj rect;//示波器方框
        Obj btn_mode;// 模式
        Obj btn_mode_label;
        Obj label_mode;
        Obj btn_freq;// 频率
        Obj btn_freq_label;
        Obj label_freq;
        Obj btn_ratio_add;// 比例
        Obj btn_ratio_add_label;
        Obj btn_ratio_sub;
        Obj btn_ratio_sub_label;
        Obj label_ratio;
        Obj btn_bias_add;// 偏置
        Obj btn_bias_add_label;
        Obj btn_bias_sub;
        Obj btn_bias_sub_label;
        Obj label_bias;
        Obj label_tick;// 时刻
        Obj label_info;// 信息
        Obj label_title;// 标题
        Obj imgbtn_play;// 播放键
        Obj label_cpu;
        Obj label_wave_cnt;// 波形点数
        Obj label_wave_type;// 波形类型
        Obj label_wave_generate;// 波形生成
    } main;// 主屏幕

};

// 取别名
using lv_ui_t = struct lv_ui_t;


// 对外接口
class uiInterface
{
public:
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
};




// 加载资源文件
LV_FONT_DECLARE(lv_customer_font_SourceHanSerifSC_Regular_15)
LV_FONT_DECLARE(lv_customer_font_SourceHanSerifSC_Regular_13)

LV_IMG_DECLARE(_btn_list_play_alpha_48x48);
LV_IMG_DECLARE(_btn_list_pause_alpha_48x48);
#endif
#endif //SIMULATOR_UI_HPP