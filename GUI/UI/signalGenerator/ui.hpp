//
// Created by fairy on 2024/10/22 16:34.
//
#ifndef SIMULATOR_UI_HPP
#define SIMULATOR_UI_HPP

#include "Module_Conf.h"
#include "lvgl.h"
/* 预编译命令 */
#if 1
/*********************组件*******************/
typedef struct
{
    struct
    {
        lv_obj_t *screen;// 屏幕自身
        lv_obj_t *chart;// 折线图
        lv_obj_t  *btn_mode;// 模式
        lv_obj_t  *btn_mode_label;
        lv_obj_t  *btn_freq;// 频率
        lv_obj_t  *btn_freq_label;
        lv_obj_t  *btn_ratio_add;// 比例
        lv_obj_t  *btn_ratio_sub;
        lv_obj_t  *btn_bias_add;// 偏置
        lv_obj_t  *btn_bias_sub;
        lv_obj_t  *imgbtn_start;// 开始
        lv_obj_t  *imgbtn_stop;// 停止
    } main;// 主屏幕

}lv_ui_t;

// 加载资源文件
LV_FONT_DECLARE(lv_customer_font_SourceHanSerifSC_Regular_16)
LV_FONT_DECLARE(lv_customer_font_SourceHanSerifSC_Regular_13)
#endif
#endif //SIMULATOR_UI_HPP