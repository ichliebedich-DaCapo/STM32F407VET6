//
// Created by fairy on 2024/10/18 20:36.
//
#ifndef SIMULATOR_UI_HPP
#define SIMULATOR_UI_HPP

#include "JYZQ_Conf.h"
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

/* 预编译命令 */
#endif
#endif //SIMULATOR_UI_HPP