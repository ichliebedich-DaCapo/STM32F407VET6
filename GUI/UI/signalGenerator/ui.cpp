//
// Created by fairy on 2024/10/22 16:34.
//
#include "ui.hpp"
#include "GUI.hpp"
#include "LV_Timer.hpp"

#if 1
// 头文件
#include "wave.h"
#include "chart.hpp"

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
    Chart::init(gui->main.chart);
    Chart::set_pos_size(122, 54, 320, 200);// 设置图表大小位置
    Chart::set_align(LV_ALIGN_CENTER, 0, 0);// 居中对齐

    // 配置图表
    Chart::set_type(LV_CHART_TYPE_LINE);// 设置类型:折线 柱形
    Chart::set_point_count(128);// 设置数据点数量
    Chart::set_update_mode(LV_CHART_UPDATE_MODE_SHIFT);// 滚动模式
    Chart::set_range(0, 255, LV_CHART_AXIS_PRIMARY_Y);// 设置Y轴范围
    Chart::remove_style_dot();// 取消折线点样式

    // 自定义刻度线和背景
    Chart::set_div_count(0, 0);// 设置刻度线

    //Write style for screen_chart_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    Chart::bg_color(Color_Firefly_Green);
    Chart::bg_opa(105);
    Chart::bg_grad_dir(LV_GRAD_DIR_HOR);// 设置垂直渐变
    Chart::bg_grad_color(Color_Firefly_Gold);// 设置渐变结束颜色

    Chart::border_set_clip_corner();
    Chart::border_color(lv_color_hex(0x3f3f3f));
    Chart::border_opa(255);
    Chart::border_width(1);
    Chart::radius(0);
    Chart::shadow_width(0);
    Chart::line_width(0);


    //Write style for screen_chart_1, Part: LV_PART_TICKS, State: LV_STATE_DEFAULT.
    Chart::text_color(lv_color_hex(0x151212));
    Chart::text_opa(255);
    Chart::add_series(series,lv_color_hex(0x00ff00)); // 添加数据序列

    /*  创建定时器*/
    timer.create([](lv_timer_t *)
                 {
                     lv_chart_set_next_value(GUI_Base::get_ui()->main.chart, series, sine_wave[wave_index]);
                     wave_index += 1;
                     if (wave_index == 128)
                         wave_index = 0;
                 }, 25);

    timer.resume();

}

auto Events::init() -> void
{


}

/***********************函数实现***********************/


#endif