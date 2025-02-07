//
// Created by fairy on 2025/1/10 18:43.
//
#ifndef SIMULATOR_UI_HPP
#define SIMULATOR_UI_HPP

#include "GUI_Base.hpp"
struct lv_ui_t
{
    // 主屏幕
    struct
    {
        Component screen{}; // 屏幕自身
        Label label_counter; // 计数器显示
        Button btn_x1;     // 触摸点1 x坐标 按钮
        Button btn_y1;    // 触摸点1 y坐标 按钮
        Button btn_x2;    // 触摸点2 x坐标 按钮
        Button btn_y2;    // 触摸点2 y坐标 按钮
    } main;// 主屏幕

};
extern struct lv_ui_t* gui;

// 加载资源文件
LV_FONT_DECLARE(lv_customer_font_SourceHanSerifSC_Regular_15)
LV_FONT_DECLARE(lv_customer_font_SourceHanSerifSC_Regular_13)

LV_IMG_DECLARE(_btn_list_play_alpha_48x48);
LV_IMG_DECLARE(_btn_list_pause_alpha_48x48);

//// 弹窗尺寸信息（）
//namespace login_dialog_info
//{
//    constexpr uint16_t x = 40;   // (480-400)/2
//    constexpr uint16_t y = 10;   // (320-300)/2
//    constexpr uint16_t width = 400;
//    constexpr uint16_t height = 300;
//    constexpr uint16_t margin = 5;
//}

// UI接口
class UI
{
public:
    // 设置计数器值
    static auto set_counter_value(int value) -> void;

    static auto set_x1_value(int value) -> void;
    static auto set_x2_value(int value) -> void;
    static auto set_y1_value(int value) -> void;
    static auto set_y2_value(int value) -> void;
    // 获取计数器值
    static auto get_counter_value() -> int;

    static void pressA();

    static void pressS();
};


#endif //SIMULATOR_UI_HPP