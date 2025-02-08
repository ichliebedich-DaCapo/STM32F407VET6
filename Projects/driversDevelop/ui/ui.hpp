//
// Created by fairy on 2025/1/10 18:43.
//
#ifndef SIMULATOR_UI_HPP
#define SIMULATOR_UI_HPP

#include "GUI_Base.hpp"

// 声明各组件
namespace gui::widgets::main
{
    extern Label label_counter; // 计数器显示
    extern Button btn_plus; // "+" 按钮
    extern Button btn_minus; // "-" 按钮
    extern Button btn_reset; // 复位按钮
}



// 加载资源文件
LV_FONT_DECLARE(lv_customer_font_SourceHanSerifSC_Regular_15)
LV_FONT_DECLARE(lv_customer_font_SourceHanSerifSC_Regular_13)

LV_IMG_DECLARE(_btn_list_play_alpha_48x48);
LV_IMG_DECLARE(_btn_list_pause_alpha_48x48);


// UI接口
namespace gui::interface
{
    // 设置计数器值
    auto set_counter_value(int value) -> void;

    // 获取计数器值
    auto get_counter_value() -> int;

    void pressA();

    void pressS();
}




#endif //SIMULATOR_UI_HPP