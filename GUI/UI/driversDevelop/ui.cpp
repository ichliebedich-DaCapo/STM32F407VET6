//
// Created by fairy on 2025/1/10 18:43.
//
#include "ui.hpp"
// 头文件

// 变量
struct lv_ui_t lv_ui;
struct lv_ui_t *gui = &lv_ui;
static int counter_value = 0; // 计数器值存储
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
void GUI_Base::events_init()
{
    // 绑定 "+" 按钮事件
    gui->main.btn_plus.add_event(btn_fun(CounterLogic::increment();));

    // 绑定 "-" 按钮事件
    gui->main.btn_minus.add_event(btn_fun(CounterLogic::decrement();));

    // 绑定复位按钮事件
    gui->main.btn_reset.add_event(btn_fun(CounterLogic::reset();));
}

// 函数
auto GUI_Base::screen_init()->void
{
    //LV_FONT_DEFAULT
//    gui->main.btn_test.init(100, 100, 60, 50, "吴俊颐",LV_FONT_DEFAULT);
//    gui->main.btn_test.init(100, 100, 60, 50, "项佳节",&lv_customer_font_SourceHanSerifSC_Regular_15);
//    gui->main.btn_test.text_color(lv_color_black());


//    CPU::init();


    Component::set_parent(gui->main.screen.get_obj());
    gui->main.screen.bg_color(lv_color_white());

    // 计数器显示
    gui->main.label_counter.init(200, 150, 80, 40, "0", &lv_customer_font_SourceHanSerifSC_Regular_15);
    gui->main.label_counter.set_align(LV_ALIGN_CENTER, 0, 0);

    // "+" 按钮
    gui->main.btn_plus.init(100, 250, 80, 40, "+", &lv_customer_font_SourceHanSerifSC_Regular_15);
    gui->main.btn_plus.set_align(LV_ALIGN_CENTER, -100, 0);
    lv_obj_set_style_bg_color(gui->main.btn_plus.get_obj(), lv_color_hex(0x34e6ff), LV_PART_MAIN); // 蓝色背景
    lv_obj_set_style_text_color(gui->main.btn_plus.get_obj(), lv_color_hex(0x000000), LV_PART_MAIN); // 黑色文字

    // "-" 按钮
    gui->main.btn_minus.init(300, 250, 80, 40, "-", &lv_customer_font_SourceHanSerifSC_Regular_15);
    gui->main.btn_minus.set_align(LV_ALIGN_CENTER, 100, 0);
    lv_obj_set_style_bg_color(gui->main.btn_minus.get_obj(), lv_color_hex(0x34e6ff), LV_PART_MAIN); // 蓝色背景
    lv_obj_set_style_text_color(gui->main.btn_minus.get_obj(), lv_color_hex(0x000000), LV_PART_MAIN); // 黑色文字
    // 复位按钮
    gui->main.btn_reset.init(200, 300, 80, 40, "复位", &lv_customer_font_SourceHanSerifSC_Regular_15);
    gui->main.btn_reset.set_align(LV_ALIGN_CENTER, 0, 50);
    lv_obj_set_style_bg_color(gui->main.btn_reset.get_obj(), lv_color_hex(0x34e6ff), LV_PART_MAIN); // 蓝色背景
    lv_obj_set_style_text_color(gui->main.btn_reset.get_obj(), lv_color_hex(0x000000), LV_PART_MAIN); // 黑色文字
}



auto CounterLogic::increment() -> void
{
    counter_value++;
    char buf[12];
    sprintf(buf, "%d", counter_value);
    gui->main.label_counter.set_text(buf);
}

auto CounterLogic::decrement() -> void
{
    counter_value--;
    char buf[12];
    sprintf(buf, "%d", counter_value);
    gui->main.label_counter.set_text(buf);
}

auto CounterLogic::reset() -> void
{
    counter_value = 0;
    char buf[12];
    sprintf(buf, "%d", counter_value);
    gui->main.label_counter.set_text(buf);
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
    gui->main.label_counter.set_text(buf);
}
/***************************UI接口*********************************/
void UI::pressA()
{

}

void UI::pressS()
{


}
/*****************************对外接口**************************/
auto UI::set_counter_value(int value) -> void
{
    CounterLogic::set_value(value);
}
auto UI::get_counter_value() -> int
{
    return CounterLogic::get_value();
}