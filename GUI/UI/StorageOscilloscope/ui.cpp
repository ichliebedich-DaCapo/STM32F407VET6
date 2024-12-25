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

//Component::set_pos_size(78, 36, 324, 208);
//Component::border_radius(5);
constexpr uint16_t start_x = 82;
constexpr uint16_t start_y = 240;

constexpr uint16_t sine_count = 128;// 采样点数量
constexpr uint16_t square_count = 200;
constexpr uint16_t max_value = 255;
constexpr uint16_t bg_color = 0xFFFF;
constexpr uint16_t line_color = 0x0000;

// 变量
LV_Timer tick_timer;
uint8_t Buf[point_cnt];

uint8_t CH0_buff[200]={0};//波形数据存储
uint16_t ADC_CH0_TAP[400];

//便利测试变量
bool use_square=false ;//测试方波
bool use_sine=true;//测试正弦波

// 播放
class StorageOscilloscope
{
public:
    // 播放
    static inline auto handler() -> void;
    // 开始
    static inline auto start() -> void;
    // 停止
    static inline auto stop() -> void;
    // 切换触发模式
    static inline auto switch_trigger_mode() -> void;
    // 图像左移
    static inline auto left_shift() -> void;
    // 图像右移
    static inline auto right_shift() -> void;
    // 设置扫描速度
    static inline auto switch_scan_speed() -> void;
    // 锁存数据
    static inline auto switch_latch() -> void;
    // 打印时间
    static inline auto print_tick() -> void;
    // 更新时间
    static inline auto update_count() -> void;
    // 获取时间
    static inline auto get_tick() -> uint32_t;
private:
    static inline auto set_trigger_mode(bool &trigger_mode) -> void;
private:
    static inline uint32_t count = 0;// 用于计时
    static inline uint8_t wave_cnt = 100;//显示点数 -> keyk8、keyk9
    static inline uint8_t length = 200;
    static inline uint16_t array_length = 400;
    static inline uint8_t wave_start_index = 100;
    static inline uint8_t index_offset = 5;
    static inline bool _trigger_mode = false;// 默认模式为单次触发
};

//
///***********************界面布置***********************/
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

    //按钮_触发模式：单次触发、多次触发
    customBtn.init(gui->main.btn_trigger_mode, gui->main.btn_trigger_mode_label, 40, 265, 40, 30, "单次触发");
    Button::hidden(gui->main.btn_trigger_mode);

    //按钮_图像水平移动：左移、右移
    customBtn.init(gui->main.btn_left_shift, gui->main.btn_left_shift_label, 115, 265, 30, 30, "01");
    customBtn.init(gui->main.btn_right_shift, gui->main.btn_right_shift_label, 155, 265, 30, 30, "68");

    //按钮_切换扫描速度
    customBtn.init(gui->main.btn_scan_speed, gui->main.btn_scan_speed_label, 40, 265, 40, 30, "切换扫描速度");
    Button::hidden(gui->main.btn_scan_speed);

    //按钮_锁存模式
    customBtn.init(gui->main.btn_latch, gui->main.btn_latch_label, 40, 265, 40, 30, "锁存");
    Button::hidden(gui->main.btn_latch);

    // 标签：标题
    Text label;
    label.init_font(&lv_customer_font_SourceHanSerifSC_Regular_15);
    label.init(gui->main.label_title, 190, 13, 140, 30, "简易存储示波器");
//    Text::hidden(gui->main.label_title);

    label.init_font(&lv_customer_font_SourceHanSerifSC_Regular_13);
    label.init(gui->main.label_tick, 40, 265, 40, 30, "tick：\n0");

    //     图片按钮：停止、播放
    ImageButton::init(gui->main.imgbtn_play, 216, 256, 48, 48, &_btn_list_play_alpha_48x48,
                      &_btn_list_pause_alpha_48x48);

}

///***********************函数实现***********************/

auto Events::init() -> void
{
    /*  创建定时器*/

    tick_timer.create(    timer_fun(
                                  StorageOscilloscope::handler();
                                  StorageOscilloscope::print_tick();
                          ), Freq_16K);
    // 绑定播放事件
    bond(gui->main.imgbtn_play, imgbtn_fun2(
            fun(StorageOscilloscope::start();),
            fun(StorageOscilloscope::stop();)));
    // 绑定切换触发模式事件
    bond(gui->main.btn_trigger_mode, btn_fun(
            fun(StorageOscilloscope::switch_trigger_mode();)));
    // 绑定图像水平移动事件
    bond(gui->main.btn_left_shift, btn_fun(
            fun(StorageOscilloscope::left_shift();)));
    bond(gui->main.btn_right_shift, btn_fun(
            fun(StorageOscilloscope::right_shift();)));
    // 绑定切换扫描速度事件
    bond(gui->main.btn_scan_speed, btn_fun(
            fun(StorageOscilloscope::switch_scan_speed();)));
    // 绑定锁存模式事件
    bond(gui->main.btn_latch, btn_fun(
            fun(StorageOscilloscope::switch_latch();)));

}
auto StorageOscilloscope::handler() -> void
{
// 调用绘制网格线的函数
    draw_dividers(78, 36, 324, 208, 10, 8, 0x8410, 2); // 使用灰色绘制分割线，留出边距
//不使用脏点数组
//    if(use_square) draw_interpolated_line_simple(start_x, start_y, square_wave, length, wave_start_index, array_length, 0);
//    if(use_sine) draw_interpolated_line_simple(start_x, start_y, sine_wave, length, wave_start_index, array_length, 0);
    if(use_square) draw_interpolated_wave(start_x, start_y, square_wave,CH0_buff, length, wave_start_index, array_length, 0,0XFFFF);
    if(use_sine)   draw_interpolated_wave(start_x, start_y, sine_wave,CH0_buff, length, wave_start_index, array_length, 0,0xFFFF);
}
auto StorageOscilloscope::print_tick() -> void
{
    Text::set_text(GUI_Base::get_ui()->main.label_tick, "tick：\n%lu", get_tick());
}
// 更新时间
auto StorageOscilloscope::update_count() -> void
{
    count = lv_tick_get();
}
// 获取时间
auto StorageOscilloscope::get_tick() -> uint32_t
{
    uint32_t temp_count = lv_tick_get();
    // 防止溢出
    if (temp_count < count)
        temp_count += (0xFFFF'FFFF - count);
    else
        temp_count -= count;

    return temp_count;
}
// 开始
auto StorageOscilloscope::start() -> void
{
    tick_timer.resume();
}
// 停止
auto StorageOscilloscope::stop() -> void
{
    tick_timer.pause();
}
// 切换触发模式
auto StorageOscilloscope::switch_trigger_mode() -> void
{
    _trigger_mode = !_trigger_mode;
    set_trigger_mode(_trigger_mode);
    update_count();
}
// 图像左移
void StorageOscilloscope::left_shift() {
    // 确保新的wave_index不会超出array_length - length
    if (wave_start_index + index_offset < array_length - length) {
        wave_start_index += index_offset;
    } else {
        // 如果wave_index + length接近或超过array_length，则设置为最大允许值
        wave_start_index = array_length - length;
    }

//不使用脏点数组(节省内存)
     LCD_Color_Clean(80, 38, 400, 242,0xFFFF);//修改这行要同时修改右移函数和画波形函数、start_x、start_y
    // 调用绘制网格线的函数
    draw_dividers(78, 36, 324, 208, 10, 8, 0x8410, 2); // 使用灰色绘制分割线，留出边距
//     if(use_square) draw_interpolated_line_simple(start_x, start_y, square_wave, length, wave_start_index, array_length, 0);
//     if(use_sine)   draw_interpolated_line_simple(start_x, start_y, sine_wave, length, wave_start_index, array_length, 0);
    if(use_square) draw_interpolated_wave(start_x, start_y, square_wave,CH0_buff, length, wave_start_index, array_length, 0,0XFFFF);
    if(use_sine)   draw_interpolated_wave(start_x, start_y, sine_wave,CH0_buff, length, wave_start_index, array_length, 0,0xFFFF);

}
// 图像右移
void StorageOscilloscope::right_shift() {
    // 确保新的wave_index不会小于0
    if (wave_start_index >= index_offset) {
        wave_start_index -= index_offset;
    } else {
        wave_start_index = 0; // 如果wave_index小于index_offset，则设置为0
    }

    // 确保新的wave_index加上length不会超出array_length
    if (wave_start_index + length > array_length) {
        length = array_length - wave_start_index; // 调整显示长度以适应新的wave_index
    }

//不使用脏点数组(节省内存)
    LCD_Color_Clean(80, 38, 400, 242,0xFFFF);//修改这行要同时修改左移函数和画波形函数、start_x、start_y
    // 调用绘制网格线的函数
    draw_dividers(78, 36, 324, 208, 10, 8, 0x8410, 2); // 使用灰色绘制分割线，留出边距
    //     if(use_square) draw_interpolated_line_simple(start_x, start_y, square_wave, length, wave_start_index, array_length, 0);
//     if(use_sine)   draw_interpolated_line_simple(start_x, start_y, sine_wave, length, wave_start_index, array_length, 0);
    if(use_square) draw_interpolated_wave(start_x, start_y, square_wave,CH0_buff, length, wave_start_index, array_length,  0,0XFFFF);
    if(use_sine)   draw_interpolated_wave(start_x, start_y, sine_wave,CH0_buff, length, wave_start_index, array_length, 0,0xFFFF);


}
// 设置扫描速度
auto StorageOscilloscope::switch_scan_speed() -> void
{

}
// 锁存数据
auto StorageOscilloscope::switch_latch() -> void
{

}
/***********************私有函数***********************/
//设置触发模式
auto StorageOscilloscope::set_trigger_mode(bool &trigger_mode) -> void
{
    _trigger_mode = trigger_mode;
    Text::set_text(_trigger_mode ? "单次触发" : "多次触发", GUI_Base::get_ui()->main.btn_trigger_mode_label);
}


/***********************对外接口***********************/
void UI_Interface::display()
{
    StorageOscilloscope::handler();
}

void UI_Interface::left_shift()
{
    StorageOscilloscope::left_shift();
}

void UI_Interface::right_shift()
{
    StorageOscilloscope::right_shift();
}
