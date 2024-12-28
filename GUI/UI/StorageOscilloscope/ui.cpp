//
// Created by fairy on 2024/10/22 16:34.
//
#include "ui.hpp"
#include "GUI.hpp"

// 头文件
#include "wave.h"
#include <cstdio>
#include "Draw_wave.hpp"

// 宏定义
#define Strong_Print 0 // 使用更强烈的显示（时间和索引都会加倍）

// 常量
//constexpr uint32_t Freq_8K = 30;
//constexpr uint32_t Freq_16K = 15;
constexpr uint32_t Freq_8K = 75;
constexpr uint32_t Freq_16K = 12;

constexpr uint8_t index_offset_8K = 2;// 8K下索引递增值
constexpr uint8_t index_offset_16K = 4;// 16K下索引递增值
constexpr uint16_t chart_width = 320;
constexpr uint16_t chart_height = 200;

//Component::set_pos_size(78, 36, 324, 208);
//Component::border_radius(5);


constexpr uint16_t sine_count = 128;// 采样点数量
constexpr uint16_t square_count = 200;
constexpr uint16_t max_value = 255;
constexpr uint16_t bg_color = 0xFFFF;
constexpr uint16_t line_color = 0x0000;

constexpr uint16_t start_x = border_info::x + 2 * border_info::margin;                       //82
constexpr uint16_t start_y = border_info::y + border_info::height - 2 * border_info::margin;   //240
//constexpr uint16_t x = 78;
//constexpr uint16_t y = 36;
//constexpr uint16_t width = 324;
//constexpr uint16_t height = 208;


// 变量
LV_Timer tick_timer;

uint8_t CH0_buff[200] = {0};//波形数据存储


//便利测试变量
bool use_square = false;//测试方波
bool use_sine = true;//测试正弦波

// 播放
class StorageOscilloscope
{
public:
    // 播放
    static inline auto handler(uint8_t (&read_wave)[400]) -> void;

    // 开始
    static inline auto start() -> void;

    // 停止
    static inline auto stop() -> void;

    // 切换触发模式
    static inline auto set_trigger_mode(uint8_t &trigger_mode) -> void;

    // 切换触发模式
    static inline auto set_latch_mode(uint8_t &latch_mode) -> void;

    // 图像左移
    static inline auto left_shift(uint8_t (&read_wave)[400]) -> void;

    // 图像右移
    static inline auto right_shift(uint8_t (&read_wave)[400]) -> void;

    // 打印时间
    static inline auto print_tick() -> void;
    // 打印放大倍数
    static inline auto print_magnification(uint32_t& magnification) -> void;
    // 打印扫描速度
    static inline auto print_scan_speed(uint32_t& scan_speed) -> void;
    // 更新时间
    static inline auto update_count() -> void;
    // 获取时间
    static inline auto get_tick() -> uint32_t;
    //获取放大倍数
    static inline auto get_magnification(uint32_t& magnification) -> uint32_t;
    // 获取扫描速度
    static inline auto get_scan_speed(uint32_t& scan_speed) -> uint32_t;


private:
    static inline uint32_t count = 0;// 用于计时
    static inline uint8_t wave_cnt = 100;//显示点数 -> keyk8、keyk9
    static inline uint8_t length = 200;
    static inline uint16_t array_length = 400;
    static inline uint8_t wave_start_index = 100;
    static inline uint8_t index_offset = 5;

};

//
///***********************界面布置***********************/
auto Screen::init() -> void
{
    Component::set_parent(gui->main.screen);

    // 设置边框
    Component::init(gui->main.rect);
    Component::set_pos_size(border_info::x, border_info::y, border_info::width, border_info::height);
    Component::border_radius(5);


    // 设置边框背景颜色和边框颜色
    lv_obj_set_style_bg_color(gui->main.rect, lv_color_hex(0), LV_PART_MAIN);
    lv_obj_set_style_border_color(gui->main.rect, lv_color_hex(0X6675), LV_PART_MAIN);
    // 初始化器
    Button customBtn;
    customBtn.init_font(&lv_customer_font_SourceHanSerifSC_Regular_15);

    //按钮_触发模式：单次触发、多次触发
    customBtn.init(gui->main.btn_trigger_mode, gui->main.btn_trigger_mode_label, 15, 270, 60, 40, "多次触发");

    //按钮_锁存模式
    customBtn.init(gui->main.btn_latch, gui->main.btn_latch_label, 85, 270, 40, 40, "锁存关");

    //按钮_图像水平移动：左移、右移
    customBtn.init(gui->main.btn_left_shift, gui->main.btn_left_shift_label, 135, 270, 40, 40, "左移");
    customBtn.init(gui->main.btn_right_shift, gui->main.btn_right_shift_label, 243, 270, 40, 40, "右移");

    //按钮_切换扫描速度
    customBtn.init(gui->main.btn_scan_speed, gui->main.btn_scan_speed_label, 415, 10, 60, 40, "扫描速度");

    //按钮_切换放大倍数
    customBtn.init(gui->main.btn_magnification, gui->main.btn_magnification_label, 415, 110, 60, 40, "放大倍数");

    //     图片按钮：停止、播放
    ImageButton::init(gui->main.imgbtn_play, 185, 266, 48, 48, &_btn_list_play_alpha_48x48,
                      &_btn_list_pause_alpha_48x48);
    // 标签：标题
    Text label;
//    label.init_font(&lv_customer_font_SourceHanSerifSC_Regular_15);
//    label.init(gui->main.label_title, 190, 13, 140, 30, "简易存储示波器");
//    label.init(gui->main.label_title, 190, 13, 140, 30, "简易存储示波器");
    label.init_font(&lv_customer_font_SourceHanSerifSC_Regular_15);
    lv_color_t label_color = lv_color_hex(0XFCC0); // 绿色
    label.init(gui->main.label_scan_speed, 415, 60, 60, 40, "Hz",label_color);
    label.init(gui->main.label_magnification, 415, 160, 60, 40, "1",label_color);

//    label.init(gui->main.label_tick, 40, 265, 40, 30, "tick：\n0",label_color);

}

///***********************函数实现***********************/

auto Events::init() -> void
{
    /*  创建定时器*/

    tick_timer.create(timer_fun(
//                                  StorageOscilloscope::handler();
                              StorageOscilloscope::print_tick();
                      ), Freq_16K);

}

auto StorageOscilloscope::handler(uint8_t (&read_wave)[400]) -> void
{

    // 调用绘制网格线的函数
    //    draw_dividers(78, 36, 324, 208, 10, 8, 0X8410, 2); // 使用灰色绘制分割线，留出边距
    draw_dashed_dividers(border_info::x, border_info::y, border_info::width, border_info::height, 10, 8, 0X8410,
                         border_info::margin, 4, 4);
//不使用脏点数组
//    if(use_square) draw_interpolated_line_simple(start_x, start_y, square_wave, length, wave_start_index, array_length, 0);
//    if(use_sine) draw_interpolated_line_simple(start_x, start_y, sine_wave, length, wave_start_index, array_length, 0);

//    if (use_square)
//        draw_interpolated_wave(start_x, start_y, square_wave, CH0_buff, length, wave_start_index, array_length, 0XFCC0,
//                               0);
//    if (use_sine)
//        draw_interpolated_wave(start_x, start_y, sine_wave, CH0_buff, length, wave_start_index, array_length, 0XFCC0,
//                               0);
    draw_interpolated_wave(start_x, start_y, read_wave, CH0_buff, length, wave_start_index, array_length, 0XFCC0,
                           0);

//    __BKPT(0);
}

auto StorageOscilloscope::print_tick() -> void
{
    Text::set_text(GUI_Base::get_ui()->main.label_tick, "tick：\n%lu", get_tick());
}

auto StorageOscilloscope::print_magnification(uint32_t& magnification) -> void
{
    Text::set_text(GUI_Base::get_ui()->main.label_magnification, "%lu", get_magnification(magnification));
}
auto StorageOscilloscope::print_scan_speed(uint32_t& scan_speed) -> void
{
    Text::set_text(GUI_Base::get_ui()->main.label_scan_speed, "%luHz", get_scan_speed(scan_speed));
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
//获取放大倍数
auto StorageOscilloscope::get_magnification(uint32_t& magnification) -> uint32_t
{
  return magnification;
}
//获取扫描速度
auto StorageOscilloscope::get_scan_speed(uint32_t& scan_speed) -> uint32_t
{
    return scan_speed;
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

// 图像左移
void StorageOscilloscope::left_shift(uint8_t (&read_wave)[400])
{
    // 确保新的wave_index不会超出array_length - length
    if (wave_start_index + index_offset < array_length - length)
    {
        wave_start_index += index_offset;
    } else
    {
        // 如果wave_index + length接近或超过array_length，则设置为最大允许值
        wave_start_index = array_length - length;
    }

//不使用脏点数组(节省内存)
//     LCD_Color_Clean(80, 38, 400, 242,0xFFFF);//修改这行要同时修改右移函数和画波形函数、start_x、start_y
    // 调用绘制网格线的函数
//    draw_dividers(78, 36, 324, 208, 10, 8, 0X8410, 2); // 使用灰色绘制分割线，留出边距
    draw_dashed_dividers(border_info::x, border_info::y, border_info::width, border_info::height, 10, 8, 0X8410,
                         border_info::margin, 4,
                         4);
//     if(use_square) draw_interpolated_line_simple(start_x, start_y, square_wave, length, wave_start_index, array_length, 0);
//     if(use_sine)   draw_interpolated_line_simple(start_x, start_y, sine_wave, length, wave_start_index, array_length, 0);
//    if (use_square)
//        draw_interpolated_wave(start_x, start_y, square_wave, CH0_buff, length, wave_start_index, array_length, 0XFCC0,
//                               0);
//    if (use_sine)
//        draw_interpolated_wave(start_x, start_y, sine_wave, CH0_buff, length, wave_start_index, array_length, 0XFCC0,
//                               0);
    draw_interpolated_wave(start_x, start_y, read_wave, CH0_buff, length, wave_start_index, array_length, 0XFCC0,
                           0);


}

// 图像右移
void StorageOscilloscope::right_shift(uint8_t (&read_wave)[400])
{
    // 确保新的wave_index不会小于0
    if (wave_start_index >= index_offset)
    {
        wave_start_index -= index_offset;
    } else
    {
        wave_start_index = 0; // 如果wave_index小于index_offset，则设置为0
    }

    // 确保新的wave_index加上length不会超出array_length
    if (wave_start_index + length > array_length)
    {
        length = array_length - wave_start_index; // 调整显示长度以适应新的wave_index
    }

//不使用脏点数组(节省内存)
//    LCD_Color_Clean(80, 38, 400, 242,0xFFFF);//修改这行要同时修改左移函数和画波形函数、start_x、start_y
    // 调用绘制网格线的函数
//    draw_dividers(78, 36, 324, 208, 10, 8, 0X8410, 2); // 使用灰色绘制分割线，留出边距
    draw_dashed_dividers(border_info::x, border_info::y, border_info::width, border_info::height, 10, 8, 0X8410,
                         border_info::margin, 4,
                         4);
    //     if(use_square) draw_interpolated_line_simple(start_x, start_y, square_wave, length, wave_start_index, array_length, 0);
//     if(use_sine)   draw_interpolated_line_simple(start_x, start_y, sine_wave, length, wave_start_index, array_length, 0);
//    if (use_square)
//        draw_interpolated_wave(start_x, start_y, square_wave, CH0_buff, length, wave_start_index, array_length, 0XFCC0,
//                               0);
//    if (use_sine)
//        draw_interpolated_wave(start_x, start_y, sine_wave, CH0_buff, length, wave_start_index, array_length, 0XFCC0,
//                               0);
    draw_interpolated_wave(start_x, start_y, read_wave, CH0_buff, length, wave_start_index, array_length, 0XFCC0,
                           0);

}

//设置触发模式
auto StorageOscilloscope::set_trigger_mode(uint8_t &trigger_mode_flag) -> void
{
    Text::set_text(trigger_mode_flag ? "单次触发" : "多次触发", GUI_Base::get_ui()->main.btn_trigger_mode_label);
}

//设置触发模式
auto StorageOscilloscope::set_latch_mode(uint8_t &latch_mode_flag) -> void
{
    Text::set_text(latch_mode_flag ? "锁存开" : "锁存关", GUI_Base::get_ui()->main.btn_latch_label);
}


/***********************对外接口***********************/
void UI_Interface::display(uint8_t (&read_wave)[400])
{
    StorageOscilloscope::handler(read_wave);
}

void UI_Interface::left_shift(uint8_t (&read_wave)[400])
{
    StorageOscilloscope::left_shift(read_wave);
}

void UI_Interface::right_shift(uint8_t (&read_wave)[400])
{
    StorageOscilloscope::right_shift(read_wave);
}
void UI_Interface::switch_trigger_mode(uint8_t trigger_mode_flag)
{
    StorageOscilloscope::set_trigger_mode(trigger_mode_flag);
}
void UI_Interface::switch_latch_mode(uint8_t latch_mode_flag)
{
    StorageOscilloscope::set_latch_mode(latch_mode_flag);
}
void UI_Interface::print_magnification(uint32_t& magnification)
{
    StorageOscilloscope::print_magnification(magnification);
}
void UI_Interface::print_scan_speed(uint32_t &scan_speed)
{
    StorageOscilloscope::print_scan_speed(scan_speed);
}
