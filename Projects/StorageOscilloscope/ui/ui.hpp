//
// Created by fairy on 2024/10/22 16:34.
//
#ifndef SIMULATOR_UI_HPP
#define SIMULATOR_UI_HPP

#include "GUI_Base.hpp"



/*********************组件*******************/
struct lv_ui_t
{
    // 主屏幕
    struct
    {
        Component screen{};// 屏幕自身
        Component rect{};//示波器方框
        Label vpp{};// 电压峰峰值
        Button trigger_mode{};//触发方式
        Button left_shift{};//左移
        Button right_shift{};//右移
        Button scan_speed{};//切换扫描速度
        Button latch{};//锁存功能
        Button magnification{};//放大倍数
        Label title{};// 标题
        Label magnification_value{};// 放大倍数
        Label scan_speed_value{};// 扫描速度
    } main;// 主屏幕

};

extern struct lv_ui_t* gui;


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

class UI_Interface
{
public:
    static void display(uint8_t (&read_wave)[400]);
    static void clear_screen();
    static void left_shift(uint8_t (&read_wave)[400]);
    static void right_shift(uint8_t (&read_wave)[400]);
    static void switch_trigger_mode(uint8_t trigger_mode_flag);
    static void switch_latch_mode(uint8_t latch_mode_flag);
    static void print_magnification(uint8_t magnification);
    static void print_vpp_max_min(float vpp, float v_max, float v_min);
    static void print_scan_speed(uint8_t& magnification);
};




#endif //SIMULATOR_UI_HPP