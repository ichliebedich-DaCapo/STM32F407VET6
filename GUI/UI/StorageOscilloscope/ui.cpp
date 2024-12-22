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
    static inline uint8_t wave_index = 0;
    static inline uint8_t index_offset = 1;
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
    Text label;
    label.init_font(&lv_customer_font_SourceHanSerifSC_Regular_13);
    Component::set_parent(gui->main.screen);// 设置父对象

    //按钮_触发模式：单次触发、多次触发
    customBtn.init(gui->main.btn_trigger_mode, gui->main.btn_trigger_mode_label, 40, 265, 40, 30, "单次触发");

    //按钮_图像水平移动：左移、右移
    customBtn.init(gui->main.btn_left_shift, gui->main.btn_left_shift_label, 115, 265, 30, 30, "左移");
    customBtn.init(gui->main.btn_right_shift, gui->main.btn_right_shift_label, 155, 265, 30, 30, "右移");

    //按钮_切换扫描速度
    customBtn.init(gui->main.btn_scan_speed, gui->main.btn_scan_speed_label, 40, 265, 40, 30, "切换扫描速度");

    //按钮_锁存模式
    customBtn.init(gui->main.btn_latch, gui->main.btn_latch_label, 40, 265, 40, 30, "锁存");

    // 标签：标题
    label.init_font(&lv_customer_font_SourceHanSerifSC_Regular_15);
    label.init(gui->main.label_title, 190, 13, 140, 30, "简易存储示波器");

    //     图片按钮：停止、播放
    ImageButton::init(gui->main.imgbtn_play, 216, 256, 48, 48, &_btn_list_play_alpha_48x48,
                      &_btn_list_pause_alpha_48x48);


}

///***********************函数实现***********************/

auto Events::init() -> void
{
    /*  创建定时器*/
    tick_timer.create([](lv_timer_t *)
                      {
                          StorageOscilloscope::handler();
                          StorageOscilloscope::print_tick();
                      }, Freq_8K);
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
    int wave_date = sine_wave[wave_index]; // 瞎抄的
    wave_date = (wave_date > max_value) ? max_value : (wave_date < 0) ? 0 : wave_date;
    WaveCurve<>::draw_curve<WaveCurveType::Interpolated_Line, uint8_t>(Buf, wave_cnt, wave_date, start_x,
                                                                       start_y,
                                                                       chart_width, chart_height, 255,
                                                                       0xFFFF, 0);

    wave_index += index_offset;
    if (wave_index >= sine_count)[[unlikely]]
    {
        wave_index -= sine_count;// 尽量让转折处柔和一些
    }
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
auto StorageOscilloscope::left_shift() -> void
{
    // 左移时，索引减小
    wave_index -= index_offset;
    if (wave_index < 0) {
        wave_index += sine_count; // 循环回到数组末尾
    }
}
// 图像右移
auto StorageOscilloscope::right_shift() -> void
{
    // 右移时，索引增加
    wave_index += index_offset;
    if (wave_index >= sine_count) {
        wave_index -= sine_count; // 循环回到数组开头
    }
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

