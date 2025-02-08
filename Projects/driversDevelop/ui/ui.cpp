//
// Created by fairy on 2025/1/10 18:43.
//
#include "GUI.hpp"
#include "ui.hpp"
// 头文件

// 变量
namespace gui::widgets::main
{
    Label label_counter; // 计数器显示
    Button btn_plus; // "+" 按钮
    Button btn_minus; // "-" 按钮
    Button btn_reset; // 复位按钮
}


// 计数器逻辑
class CounterLogic
{
public:
    // 增加计数器
    static inline auto increment() -> void;

    // 减少计数器
    static inline auto decrement() -> void;

    // 复位计数器
    static inline auto reset() -> void;

    // 获取计数器值
    static inline auto get_value() -> int;

    // 设置计数器值
    static inline auto set_value(int value) -> void;

private:
    static inline int counter_value = 0; // 计数器值
};

// 默认使用main屏幕里的组件（暂时用不到其他界面）
using namespace gui::widgets::main;
namespace gui::init
{
    void screen()
    {
        Component::set_parent(scr.get_obj());
        scr.bg_color(lv_color_white());

        // 计数器显示
        label_counter.init(200, 150, 80, 40, "0", &lv_customer_font_SourceHanSerifSC_Regular_15);
        label_counter.set_align(LV_ALIGN_CENTER, 0, 0);

        // "+" 按钮
        btn_plus.init(100, 250, 80, 40, "+", &lv_customer_font_SourceHanSerifSC_Regular_15);
        btn_plus.set_align(LV_ALIGN_CENTER, -100, 0);
        lv_obj_set_style_bg_color(btn_plus.get_obj(), lv_color_hex(0x34e6ff), LV_PART_MAIN); // 蓝色背景
        lv_obj_set_style_text_color(btn_plus.get_obj(), lv_color_hex(0x000000), LV_PART_MAIN); // 黑色文字

        // "-" 按钮
        btn_minus.init(300, 250, 80, 40, "-", &lv_customer_font_SourceHanSerifSC_Regular_15);
        btn_minus.set_align(LV_ALIGN_CENTER, 100, 0);
        lv_obj_set_style_bg_color(btn_minus.get_obj(), lv_color_hex(0x34e6ff), LV_PART_MAIN); // 蓝色背景
        lv_obj_set_style_text_color(btn_minus.get_obj(), lv_color_hex(0x000000), LV_PART_MAIN); // 黑色文字
        // 复位按钮
        btn_reset.init(200, 300, 80, 40, "复位", &lv_customer_font_SourceHanSerifSC_Regular_15);
        btn_reset.set_align(LV_ALIGN_CENTER, 0, 50);
        lv_obj_set_style_bg_color(btn_reset.get_obj(), lv_color_hex(0x34e6ff), LV_PART_MAIN); // 蓝色背景
        lv_obj_set_style_text_color(btn_reset.get_obj(), lv_color_hex(0x000000), LV_PART_MAIN); // 黑色文字
    }


    void events()
    {
        // 绑定 "+" 按钮事件
        btn_plus.add_event(btn_fun(CounterLogic::increment();));

        // 绑定 "-" 按钮事件
        btn_minus.add_event(btn_fun(CounterLogic::decrement();));

        // 绑定复位按钮事件
        btn_reset.add_event(btn_fun(CounterLogic::reset();));
    }


}


// UI接口
namespace gui::interface
{
    void pressA()
    {

    }

    void pressS()
    {


    }

    auto set_counter_value(int value) -> void
    {
        CounterLogic::set_value(value);
    }

    auto get_counter_value() -> int
    {
        return CounterLogic::get_value();
    }
}

// -----------------------------成员函数的实现---------------------------------------

auto CounterLogic::increment() -> void
{
    counter_value++;
    char buf[12];
    sprintf(buf, "%d", counter_value);
    label_counter.set_text(buf);
}

auto CounterLogic::decrement() -> void
{
    counter_value--;
    char buf[12];
    sprintf(buf, "%d", counter_value);
    label_counter.set_text(buf);
}

auto CounterLogic::reset() -> void
{
    counter_value = 0;
    char buf[12];
    sprintf(buf, "%d", counter_value);
    label_counter.set_text(buf);
}

auto CounterLogic::get_value() -> int
{
    return counter_value;
}

auto CounterLogic::set_value(int value) -> void
{
    counter_value = value;
    char buf[12];
    sprintf(buf, "%d", counter_value);
    label_counter.set_text(buf);
}
