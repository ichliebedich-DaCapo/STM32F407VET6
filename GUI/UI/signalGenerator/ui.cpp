//
// Created by fairy on 2024/10/22 16:34.
//
#include "ui.hpp"
#include "GUI.hpp"
#include "LV_Timer.hpp"

#if 1
// 头文件
#include "wave.h"

// 变量
uint8_t wave_index = 0;
lv_chart_series_t *series;
LV_Timer timer;

/***********************函数声明***********************/


/***********************函数实现***********************/
auto Screen::init() -> void
{
    Component::set_parent(gui->main.screen);

    // 创建图表对象
    gui->main.chart = lv_chart_create(gui->main.screen);
    lv_obj_set_size(gui->main.chart, 320, 240); // 设置图表大小
    lv_obj_align(gui->main.chart, LV_ALIGN_CENTER, 0, 0); // 居中对齐

    // 配置图表
    lv_chart_set_type(gui->main.chart, LV_CHART_TYPE_LINE);
    lv_chart_set_update_mode(gui->main.chart, LV_CHART_UPDATE_MODE_SHIFT);
    lv_chart_set_point_count(gui->main.chart, 255); // 设置数据点数量
    lv_chart_set_range(gui->main.chart, LV_CHART_AXIS_PRIMARY_Y, 0, 255);

    // 自定义刻度线和背景
    lv_chart_set_div_line_count(gui->main.chart, 0, 0); // 更多刻度线
    lv_chart_set_axis_tick(gui->main.chart, LV_CHART_AXIS_PRIMARY_Y, 2, 1, 1, 10, true, 2); // 调整刻度线样式
    lv_obj_set_style_bg_color(gui->main.chart, lv_color_hex(0x1E1E1E), 0); // 深色背景
    lv_obj_set_style_bg_opa(gui->main.chart, LV_OPA_COVER, 105); // 完全不透明
    lv_obj_set_style_border_color(gui->main.chart, lv_color_hex(0x3C3C3C), 0); // 边框颜色
    lv_obj_set_style_border_width(gui->main.chart, 1, 0); // 边框宽度
    // 添加数据序列
    series = lv_chart_add_series(gui->main.chart, lv_color_hex(0x00ff00), LV_CHART_AXIS_PRIMARY_Y);

    /*  创建定时器*/
    timer.create([](lv_timer_t *)
                 {
                     lv_chart_set_next_value(GUI_Base::get_ui()->main.chart, series, sine_wave[wave_index++]);
                 }, 10);
    timer.resume();

}

auto Events::init() -> void
{


}

/***********************函数实现***********************/


#endif