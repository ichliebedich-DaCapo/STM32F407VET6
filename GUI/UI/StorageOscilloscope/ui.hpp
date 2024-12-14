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
        Obj rect;
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