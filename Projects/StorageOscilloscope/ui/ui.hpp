//
// Created by fairy on 2024/10/22 16:34.
//
#ifndef SIMULATOR_UI_HPP
#define SIMULATOR_UI_HPP

#include "GUI_Base.hpp"

namespace gui::widgets::main
{
    extern Component rect;//示波器方框
    extern Label label_vpp;// 电压峰峰值
    extern Button trigger_mode;//触发方式
    extern Button left_shift;//左移
    extern Button right_shift;//右移
    extern Button scan_speed;//切换扫描速度
    extern Button latch;//锁存功能
    extern Button magnification;//放大倍数
    extern Label title;// 标题
    extern Label magnification_value;// 放大倍数
    extern Label scan_speed_value;// 扫描速度
}


// 加载资源文件
LV_FONT_DECLARE(lv_customer_font_SourceHanSerifSC_Regular_15)
LV_FONT_DECLARE(lv_customer_font_SourceHanSerifSC_Regular_13)

LV_IMG_DECLARE(_btn_list_play_alpha_48x48);
LV_IMG_DECLARE(_btn_list_pause_alpha_48x48);

// 边框信息
namespace border_info
{
    constexpr uint16_t x = 10;
    constexpr uint16_t y = 10;
    constexpr uint16_t width = 400;
    constexpr uint16_t height = 250;
    constexpr uint16_t margin = 2;
//    constexpr uint16_t x = 78;
//    constexpr uint16_t y = 36;
//    constexpr uint16_t width = 324;
//    constexpr uint16_t height = 208;
//    constexpr uint16_t margin = 2;
}


namespace gui::interface
{
    void display(uint8_t (&read_wave)[400]);

    void clear_screen();

    void left_shift(uint8_t (&read_wave)[400]);

    void right_shift(uint8_t (&read_wave)[400]);

    void switch_trigger_mode(uint8_t trigger_mode_flag);

    void switch_latch_mode(uint8_t latch_mode_flag);

    void print_magnification(uint8_t magnification);

    void print_vpp_max_min(float vpp, float v_max, float v_min);

    void print_scan_speed(uint8_t &magnification);
}


#endif //SIMULATOR_UI_HPP