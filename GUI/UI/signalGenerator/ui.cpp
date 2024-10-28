//
// Created by fairy on 2024/10/22 16:34.
//
#include "ui.hpp"
#include "GUI.hpp"

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
    Chart::init(gui->main.chart);
    Chart::set_align(LV_ALIGN_CENTER, 0, -15);
    Chart::set_size(320, 200);// 设置图表大小位置

    // 配置图表
    Chart::set_type(LV_CHART_TYPE_LINE);// 设置类型:折线 柱形
    Chart::set_point_count(128);// 设置数据点数量
    Chart::set_update_mode(LV_CHART_UPDATE_MODE_SHIFT);// 滚动模式
    Chart::set_range(0, 255, LV_CHART_AXIS_PRIMARY_Y);// 设置Y轴范围
    Chart::remove_style_dot();// 取消折线点样式

    // 自定义刻度线和背景
    Chart::set_div_count(0, 0);// 设置刻度线

    Chart::text_color(lv_color_hex(0x151212));
    Chart::text_opa(255);

    Chart::bg_color(Color_Firefly_Green);
    Chart::bg_opa(105);
    Chart::bg_grad_dir(LV_GRAD_DIR_HOR);// 设置垂直渐变
    Chart::bg_grad_color(Color_Firefly_Gold);// 设置渐变结束颜色

    Chart::border_radius(6);
    Chart::border_color(lv_color_hex(0x3f3f3f));
    Chart::border_opa(255);
    Chart::border_width(1);

    Chart::add_series(series, lv_color_hex(0x00ff00)); // 添加数据序列

// 按钮_模式：单频、混频
    CustomButton customBtn;
    customBtn.font(&lv_customer_font_SourceHanSerifSC_Regular_16);
    customBtn.init(gui->main.btn_mode, gui->main.btn_freq_label, 40, 280, 40, 30, "单频");



// 按钮_频率：8K、16K

// 按钮_占空比：+、-

// 按钮_偏置：+、-

// 图片按钮：停止、播放

}


/***********************函数实现***********************/

auto Events::init() -> void
{


    /*  创建定时器*/
    timer.create([](lv_timer_t *)
                 {
                     Chart::set_next_value(series, sine_wave[wave_index], gui->main.chart);
                     wave_index += 1;
                     if (wave_index == 128)
                         wave_index = 0;
                 }, 25);
    timer.resume();
}


#endif