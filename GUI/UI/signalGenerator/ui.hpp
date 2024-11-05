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
    } main;// 主屏幕

};

// 取别名
using lv_ui_t = struct lv_ui_t;


// 加载资源文件
LV_FONT_DECLARE(lv_customer_font_SourceHanSerifSC_Regular_15)
LV_FONT_DECLARE(lv_customer_font_SourceHanSerifSC_Regular_13)

LV_IMG_DECLARE(_btn_list_play_alpha_48x48);
LV_IMG_DECLARE(_btn_list_pause_alpha_48x48);
#endif
#endif //SIMULATOR_UI_HPP