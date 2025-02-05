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
        Button btn_plus; // "+" 按钮
        Button btn_minus; // "-" 按钮
        Button btn_reset; // 复位按钮
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

    static auto set_button1_value(int value) -> void;
    static auto set_button2_value(int value) -> void;

    // 获取计数器值
    static auto get_counter_value() -> int;

    static void pressA();

    static void pressS();
};


#endif //SIMULATOR_UI_HPP