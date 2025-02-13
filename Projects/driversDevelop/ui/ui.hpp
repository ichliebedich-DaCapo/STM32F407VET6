//
// Created by fairy on 2025/1/10 18:43.
//
#ifndef SIMULATOR_UI_HPP
#define SIMULATOR_UI_HPP

#include "GUI_Base.hpp"
#include "WaveDraw.hpp"
#include "BasicDraw.hpp"
/// 声明各组件
namespace gui::widgets::main
{
    extern Label label_counter; // 计数器显示
    extern Button btn_x1;     // 触摸点1 x坐标 按钮
    extern Button btn_y1;    // 触摸点1 y坐标 按钮
    extern Button btn_x2;    // 触摸点2 x坐标 按钮
    extern Button btn_y2;    // 触摸点2 y坐标 按钮
    extern Slider slider_1;   // 滑动条
    extern Roller roller_1;    //滚轮控件
    extern Dropdown dropdown_1; // 下拉框控件
    extern Label label_weekday; // 星期几显示
    extern Button btn_hidden; // 隐藏按键
    extern Chart chart_1;       // 图表控价
    extern Button btn_Random_data; // 生成随机数据按键
}
// 边框信息
namespace border_info
{
    constexpr uint32_t x = 10;
    constexpr uint32_t y = 10;
    constexpr uint32_t width = 400;
    constexpr uint32_t height = 250;
    constexpr uint32_t margin = 2;
    constexpr uint32_t h_div = 10;
    constexpr uint32_t v_div = 8;
    constexpr uint32_t grid_dash_length = 4;
    constexpr uint32_t grid_gap_length = 4;
//    constexpr uint16_t x = 78;
//    constexpr uint16_t y = 36;
//    constexpr uint16_t width = 324;
//    constexpr uint16_t height = 208;
//    constexpr uint16_t margin = 2;
}



/// UI接口
namespace gui::interface
{
    // 设置计数器值
    auto set_counter_value(int value) -> void;

    auto set_x1_value(int value) -> void;

    auto set_x2_value(int value) -> void;

    auto set_y1_value(int value) -> void;

    auto set_y2_value(int value) -> void;

    // 获取计数器值
    auto get_counter_value() -> int;

    void pressA();

    void pressS();
}


// 加载资源文件
LV_FONT_DECLARE(lv_customer_font_SourceHanSerifSC_Regular_15)
LV_FONT_DECLARE(lv_customer_font_SourceHanSerifSC_Regular_13)

LV_IMG_DECLARE(_btn_list_play_alpha_48x48);
LV_IMG_DECLARE(_btn_list_pause_alpha_48x48);

#endif //SIMULATOR_UI_HPP