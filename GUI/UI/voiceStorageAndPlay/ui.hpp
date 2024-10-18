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
//typedef struct
//{
//    lv_obj_t *label_slider_time;
//    lv_obj_t *label_speed;
//    lv_obj_t *slider;
//} my_slider_t;// 进度条
//
//typedef struct
//{
//    lv_obj_t *imgbtn_slow;
//    lv_obj_t *imgbtn_acc;
//    lv_obj_t *imgbtn_play;
//} my_fun_t;// 功能键
//
//typedef struct
//{
//    lv_obj_t *img_11;
//    lv_obj_t *label_title_music;
//}my_resource_t;
/*********************屏幕*******************/

typedef struct
{
    bool screen_del;
    lv_obj_t *screen;// 屏幕自身
    lv_obj_t *screen_img_11;
    lv_obj_t *screen_label_speed;
    lv_obj_t *screen_imgbtn_slow;
    lv_obj_t *screen_imgbtn_acc;
    lv_obj_t *screen_label_title_music;
    lv_obj_t *screen_label_slider_time;
    lv_obj_t *screen_slider;
    lv_obj_t *screen_imgbtn_play;
} my_screen_t;// 主屏幕

/*********************GUI*******************/
typedef struct
{
    my_screen_t *main;
} lv_ui_t;// gui

extern lv_ui_t ui;
// 接口

/* 预编译命令 */
#endif
#endif //SIMULATOR_UI_HPP