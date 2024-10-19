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
typedef struct
{
    bool screen_dell;
    struct
    {
        lv_obj_t *screen;// 屏幕自身
        lv_obj_t *img_11;
        lv_obj_t *imgbtn_slow;
        lv_obj_t *imgbtn_acc;
        lv_obj_t *label_title_music;
        lv_obj_t *slider;
        lv_obj_t *label_speed;
        lv_obj_t *label_slider_time;
        lv_obj_t *imgbtn_play;
    } main;// 主屏幕

}lv_ui_t;


// 加载资源文件
//LV_FONT_DECLARE(lv_customer_font_SourceHanSerifSC_Regular_14)
//LV_FONT_DECLARE(lv_customer_font_SourceHanSerifSC_Regular_12)
//
//LV_IMG_DECLARE(_icn_slider_alpha_15x15);
//LV_IMG_DECLARE(_btn_prev_alpha_37x37);
//LV_IMG_DECLARE(_btn_next_alpha_37x37);
//LV_IMG_DECLARE(_btn_list_play_alpha_48x48);
//LV_IMG_DECLARE(_btn_list_pause_alpha_48x48);



/* 预编译命令 */
#endif
#endif //SIMULATOR_UI_HPP