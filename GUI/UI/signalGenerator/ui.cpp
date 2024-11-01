//
// Created by fairy on 2024/10/22 16:34.
//
#include "ui.hpp"
#include "GUI.hpp"

#if 1
// 头文件
#include "wave.h"
#include <cstdio>
// 宏定义
#define Strong_Print 0 // 使用更强烈的显示（时间和索引都会加倍）


// 常量
//constexpr uint32_t Freq_8K = 30;
//constexpr uint32_t Freq_16K = 15;
constexpr uint32_t Freq_8K = 12;
constexpr uint32_t Freq_16K = 6;

// 变量
lv_chart_series_t *series;
LV_Timer timer;
LV_Timer tick_timer;

class SignalGenerator : public GUI_Base
{
public:
    // 播放
    static inline auto handler() -> void
    {
        /**可以加个值判断*/
        if (_mode)
        {
            // 16K -> 8K
            if (_freq)
            {
                if (get_tick() >= period * 2)
                {
                    if (_ratio != 0)
                    {
                        set_freq(false);// 切换为8K
                        update_count();
                        index_offset = 1;
                    }
                }
            } else// 8K -> 16K
            {
                // 极端情况：
                // ① _ratio为100，那么会直接跳过
                // ② _ratio为0，那么会一直执行这个
                if (get_tick() >= period * _ratio / 50)
                {
                    if (_ratio != 100)
                    {
                        set_freq(true);
                        index_offset = 2;
                    }
                }
            }
        }
        Chart::set_next_value(series, static_cast<Coord>(sine_wave[wave_index] + bias), gui->main.chart);
        wave_index += index_offset;
        if (wave_index >= 128)
            wave_index = 0;
    }

    static inline auto start() -> void
    {
        timer.resume();
        tick_timer.resume();
    }

    static inline auto stop() -> void
    {
        timer.pause();
        tick_timer.pause();
    }

    // 切换频率
    static inline auto switch_freq() -> void
    {
        _freq = !_freq;
        set_freq(_freq);
        update_count();
    }

    // 切换模式
    static inline auto switch_mode() -> void
    {
        _mode = !_mode;
        set_mode(_mode);
        update_count();
    }

    // 加偏置
    static inline auto add_bias() -> void
    {
        if (bias < 120)
            bias += 10;
        char buf[12];
        sprintf(buf, "偏置：%d", bias);
        Text::set_text(buf, gui->main.label_bias);
    }

    // 减偏置
    static inline auto sub_bias() -> void
    {
        if (bias > -120)
            bias -= 10;
        char buf[12];
        sprintf(buf, "偏置：%d", bias);
        Text::set_text(buf, gui->main.label_bias);
    }

    static inline auto add_ratio() -> void
    {
        if (_ratio < 100)
            _ratio += 10;
        set_ratio(_ratio);
    }

    static inline auto sub_ratio() -> void
    {
        if (_ratio > 0)
            _ratio -= 10;
        set_ratio(_ratio);
    }

    static inline auto print_tick() -> void
    {
        char buf[12];
        sprintf(buf, "tick：\n%d", get_tick());
        Text::set_text(buf, gui->main.label_tick);
    }

private:
    // 设置频率,true表示16K，false表示8K
    static inline auto set_freq(bool freq) -> void
    {
        _freq = freq;
        Text::set_text(_freq ? "16K" : "8K", gui->main.btn_freq_label);
#if Strong_Print
        timer.set_period(_freq ? Freq_16K : Freq_8K);
#endif
    }

    static inline auto set_mode(bool &mode) -> void
    {
        _mode = mode;
        Text::set_text(_mode ? "双频" : "单频", gui->main.btn_mode_label);
    }

    static inline auto set_ratio(uint8_t ratio) -> void
    {
        _ratio = ratio;
        char buf[12];
        sprintf(buf, "占比：%d%%", _ratio);
        Text::set_text(buf, gui->main.label_ratio);
    }

    // 更新时间
    static inline auto update_count() -> void
    {
        count = lv_tick_get();
    }

    // 获取时间
    static inline auto get_tick() -> uint32_t
    {
        uint32_t temp_count = lv_tick_get();
        // 防止溢出
        if (temp_count < count)
            temp_count += (0xFFFF'FFFF - count);
        else
            temp_count -= count;

        return temp_count;
    }

private:
    static inline uint32_t count = 0;// 用于计时
    static inline int8_t bias = 0;
    static inline uint8_t _ratio = 50;// 8K的占比
    static inline uint8_t wave_index = 0;
    static inline uint8_t index_offset = 1;
    static inline bool _mode = false;// 默认模式为单频
    static inline bool _freq = false;// 默认频率为8K
    static constexpr uint32_t period = 1500;// 周期为1500ms
};

/***********************函数实现***********************/
auto Screen::init() -> void
{
    Component::set_parent(gui->main.screen);

    // 创建图表对象
    Chart::init(gui->main.chart, 0, -15, 320, 200);
    Chart::add_series(series, lv_color_hex(0x00ff00)); // 添加数据序列

    // 初始化器
    Button customBtn;
    customBtn.init_font(&lv_customer_font_SourceHanSerifSC_Regular_15);
    Text label;
    label.init_font(&lv_customer_font_SourceHanSerifSC_Regular_13);
    Component::set_parent(gui->main.screen);// 设置父对象

//     按钮_模式：单频、混频
    customBtn.init(gui->main.btn_mode, gui->main.btn_mode_label, 40, 265, 40, 30, "单频");
    label.init(gui->main.label_mode, 45, 300, 40, 20, "模式");

//     按钮_偏置：+、-
    customBtn.init(gui->main.btn_bias_add, gui->main.btn_bias_add_label, 115, 265, 30, 30, "+");
    customBtn.init(gui->main.btn_bias_sub, gui->main.btn_bias_sub_label, 155, 265, 30, 30, "-");
    label.init(gui->main.label_bias, 118, 300, 80, 20, "偏置：0");

//     按钮_频率：8K、16K
    customBtn.init(gui->main.btn_freq, gui->main.btn_freq_label, 400, 265, 40, 30, "8K");// 根据对称性，x：480-40-40
    label.init(gui->main.label_freq, 405, 300, 40, 20, "频率");

//     按钮_占比：+、-
    customBtn.init(gui->main.btn_ratio_add, gui->main.btn_ratio_add_label, 295, 265, 30, 30, "+");
    customBtn.init(gui->main.btn_ratio_sub, gui->main.btn_ratio_sub_label, 335, 265, 30, 30, "-");
    label.init(gui->main.label_ratio, 298, 300, 80, 20, "占比：50%");

//     图片按钮：停止、播放
    ImageButton::init(gui->main.imgbtn_play, 216, 256, 48, 48, &_btn_list_play_alpha_48x48,
                      &_btn_list_pause_alpha_48x48);

    // 标签：信息
    label.init(gui->main.label_info, 420, 60, 60, 80, "队伍：\n\n王正翔\n吴俊颐\n谢智晴\n何乃滔");

    // 标签：时刻
    label.init(gui->main.label_tick, 20, 60, 60, 80, "tick：\n0");
    Text::set_text_color(Color_Firefly_Green);

    // 标签：标题
    label.init_font(&lv_customer_font_SourceHanSerifSC_Regular_15);
    label.init(gui->main.label_title, 190, 13, 140, 30, "双音频信号发生器");
}


/***********************函数实现***********************/

auto Events::init() -> void
{
    /*  创建定时器*/
    timer.create([](lv_timer_t *) { SignalGenerator::handler(); }, Freq_8K);
    tick_timer.create([](lv_timer_t *) { SignalGenerator::print_tick(); }, 10);

    // 绑定播放事件
    bond(gui->main.imgbtn_play, imgbtn_fun2(
            FUN(SignalGenerator::start();),
            FUN(SignalGenerator::stop();)));

    // 绑定频率事件
    bond(gui->main.btn_freq, btn_fun(
            FUN(SignalGenerator::switch_freq();)));

    // 绑定偏置事件
    bond(gui->main.btn_bias_add, btn_fun(
            FUN(SignalGenerator::add_bias();)));
    bond(gui->main.btn_bias_sub, btn_fun(
            FUN(SignalGenerator::sub_bias();)));

    // 绑定模式事件
    bond(gui->main.btn_mode, btn_fun(
            FUN(SignalGenerator::switch_mode();)));

    // 绑定占比事件
    bond(gui->main.btn_ratio_add, btn_fun(
            FUN(SignalGenerator::add_ratio();)));
    bond(gui->main.btn_ratio_sub, btn_fun(
            FUN(SignalGenerator::sub_ratio();)));
}


#endif