//
// Created by fairy on 2024/10/22 16:34.
//
#ifndef SIMULATOR_UI_HPP
#define SIMULATOR_UI_HPP

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
        Obj btn_trigger_mode;//触发方式
        Obj btn_trigger_mode_label;
        Obj btn_left_shift;//左移
        Obj btn_left_shift_label;
        Obj btn_right_shift;//右移
        Obj btn_right_shift_label;
        Obj imgbtn_play;// 播放键
        Obj btn_scan_speed;//切换扫描速度
        Obj btn_scan_speed_label;
        Obj btn_latch;//锁存功能
        Obj btn_latch_label;
        Obj label_title;// 标题
        Obj label_tick;// 时刻
    } main;// 主屏幕

};

// 取别名
using lv_ui_t = struct lv_ui_t;


// 加载资源文件
LV_FONT_DECLARE(lv_customer_font_SourceHanSerifSC_Regular_15)
LV_FONT_DECLARE(lv_customer_font_SourceHanSerifSC_Regular_13)

LV_IMG_DECLARE(_btn_list_play_alpha_48x48);
LV_IMG_DECLARE(_btn_list_pause_alpha_48x48);

class UI_Interface
{
public:
    static  void display();
    static  void left_shift();
    static  void right_shift();
};





#endif
#endif //SIMULATOR_UI_HPP