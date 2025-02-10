//
// Created by fairy on 2025/1/10 18:43.
//
#include "ui.hpp"
#include "GUI.hpp"

// 变量
namespace gui::widgets::main
{
    Label label_counter; // 计数器显示
    Button btn_x1;     // 触摸点1 x坐标 按钮
    Button btn_y1;    // 触摸点1 y坐标 按钮
    Button btn_x2;    // 触摸点2 x坐标 按钮
    Button btn_y2;    // 触摸点2 y坐标 按钮
    Slider slider_1;   // 滑动条
    Roller roller_1;
}
static int counter_value = 0; // 计数器值存储

// 计数器逻辑
class CounterLogic
{
public:
    // 更新触摸点坐标
    static inline auto update_point() -> void;

public:
    // 获取触摸点坐标
    static inline  auto get_touch_point() -> void;

    // 增加计数器
    static inline auto increment() -> void;

    // 减少计数器
    static inline auto decrement() -> void;

    // 复位计数器
    static inline auto reset() -> void;

    // 获取计数器值
    static inline auto get_value() -> int;

    // 设置计数器值
    static inline auto set_value_label(int value) -> void;

    // 触摸点1 x坐标
    static inline auto set_value_x1(int value) -> void;

    // 触摸点1 y坐标
    static inline auto set_value_y1(int value) -> void;

    // 触摸点2 x坐标
    static inline auto set_value_x2(int value) -> void;

    // 触摸点2 y坐标
    static inline auto set_value_y2(int value) -> void;

private:
    static inline int counter_value = 0; // 计数器值
    static inline lv_point_t point_1{};
    static inline lv_point_t point_2{};// 不存在第二个触摸点
};

// 默认使用main屏幕里的组件（暂时用不到其他界面）
using namespace gui::widgets::main;
namespace gui::init
{
    void screen()
    {
        // 设置全局字体格式
        Label::Font(lv_customer_font_SourceHanSerifSC_Regular_15);
        Button::Font(lv_customer_font_SourceHanSerifSC_Regular_15);

        // 计数器显示
        widgets::main::label_counter.init("0").pos_size(260, 150, 80, 40);

        // 触摸点1 x坐标
        btn_x1.init(50, 100, 80, 40, "0").bg_color(lv_color_hex(0x34e6ff));

        // 触摸点1 y坐标
        btn_y1.init(150, 100, 80, 40, "0").bg_color(lv_color_hex(0x34e6ff));

        // 触摸点2 x坐标 按钮
        btn_x2.init(50, 150, 80, 40, "+").bg_color(lv_color_hex(0x34e6ff));

        // 触摸点2 y坐标 按钮
        btn_y2.init(150, 150, 80, 40, "-").bg_color(lv_color_hex(0x34e6ff));

        // 测试 滑动条
        slider_1.init(50, 200, 200, 10).bg_color(lv_color_hex(0x34e6ff));

    }


    void events()
    {
        // 绑定 "+" 按钮事件
        widgets::main::btn_x2.OnClicked<CounterLogic::increment>();

        // 绑定 "-" 按钮事件
        widgets::main::btn_y2.OnClicked<CounterLogic::decrement>();

        // 屏幕触摸事件
        widgets::main::scr.OnPressing<CounterLogic::update_point>();
    }
}


/// UI接口
namespace gui::interface
{
    auto set_counter_value(int value) -> void
    {
        CounterLogic::set_value_label(value);
    }

    auto set_x1_value(int value) -> void
    {
        CounterLogic::set_value_x1(value);
    }

    auto set_y1_value(int value) -> void
    {
        CounterLogic::set_value_y1(value);
    }

    auto set_x2_value(int value) -> void
    {
        CounterLogic::set_value_x2(value);
    }

    auto set_y2_value(int value) -> void
    {
        CounterLogic::set_value_y2(value);
    }

    auto get_counter_value() -> int
    {
        return CounterLogic::get_value();
    }
}



auto CounterLogic::increment() -> void
{
    counter_value++;
    char buf[12];
    sprintf(buf, "%d", counter_value);
    label_counter.text(buf);
}

auto CounterLogic::decrement() -> void
{
    counter_value--;
    char buf[12];
    sprintf(buf, "%d", counter_value);
    label_counter.text(buf);
}

auto CounterLogic::reset() -> void
{
    counter_value = 0;
    char buf[12];
    sprintf(buf, "%d", counter_value);
    label_counter.text(buf);
}

auto CounterLogic::get_value() -> int
{
    return counter_value;
}

auto CounterLogic::set_value_label(int value) -> void
{
    counter_value = value;
    char buf[12];
    sprintf(buf, "%d", counter_value);
    label_counter.text(buf);
}

auto CounterLogic::set_value_x1(int value) -> void
{
    char buf[12];
    sprintf(buf, "%d", value);
    btn_x1.text(buf);
}

auto CounterLogic::set_value_y1(int value) -> void
{
    char buf[12];
    sprintf(buf, "%d", value);
    btn_y1.text(buf);
}

auto CounterLogic::set_value_x2(int value) -> void
{
    char buf[12];
    sprintf(buf, "%d", value);
    btn_x2.text(buf);
}

auto CounterLogic::set_value_y2(int value) -> void
{
    char buf[12];
    sprintf(buf, "%d", value);
    btn_y2.text(buf);
}

auto CounterLogic::get_touch_point() -> void
{
    lv_indev_get_point(lv_indev_get_act(), &point_1);
}

auto CounterLogic::update_point() -> void
{
    get_touch_point();
    set_value_x1(point_1.x);
    set_value_y1(point_1.y);
}
