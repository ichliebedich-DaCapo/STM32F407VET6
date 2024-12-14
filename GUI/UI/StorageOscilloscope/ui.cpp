//
// Created by fairy on 2024/10/22 16:34.
//
#include "ui.hpp"
#include "GUI.hpp"

// 头文件
#include "wave.h"
#include <cstdio>
#include "WaveCurve.hpp"

// 宏定义
#define Strong_Print 0 // 使用更强烈的显示（时间和索引都会加倍）

// 常量
//constexpr uint32_t Freq_8K = 30;
//constexpr uint32_t Freq_16K = 15;
constexpr uint32_t Freq_8K = 75;
constexpr uint32_t Freq_16K = 12;
constexpr uint8_t index_offset_8K = 2;// 8K下索引递增值
constexpr uint8_t index_offset_16K = 4;// 16K下索引递增值
constexpr uint16_t point_cnt = 256;// 点的数量
constexpr uint16_t chart_width = 320;
constexpr uint16_t chart_height = 200;
constexpr uint16_t start_x = 80;
constexpr uint16_t start_y = 40;
constexpr uint16_t sine_count = 128;// 采样点数量
constexpr uint16_t max_value = 255;
constexpr uint16_t bg_color = 0xFFFF;
constexpr uint16_t line_color = 0x0000;

// 变量
LV_Timer tick_timer;
uint8_t Buf[point_cnt];


// 播放
class SignalGenerator
{
public:

};

//
///***********************函数实现***********************/
auto Screen::init() -> void
{
    Component::set_parent(gui->main.screen);
    // 设置边框
    Component::init(gui->main.rect);
    Component::set_pos_size(78, 36, 324, 208);
    Component::border_radius(5);

    // 初始化器
    Button customBtn;
    customBtn.init_font(&lv_customer_font_SourceHanSerifSC_Regular_15);
    Text label;
    label.init_font(&lv_customer_font_SourceHanSerifSC_Regular_13);
    Component::set_parent(gui->main.screen);// 设置父对象
}

///***********************函数实现***********************/

auto Events::init() -> void
{

}




