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
};
void GUI_Base::events_init()
{

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

    // 触摸点1 x坐标 按钮
    gui->main.btn_x1.init(50, 100, 80, 40, "0", &lv_customer_font_SourceHanSerifSC_Regular_15);
//    gui->main.btn_x1.set_align(LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_color(gui->main.btn_x1.get_obj(), lv_color_hex(0x34e6ff), LV_PART_MAIN); // 蓝色背景
    lv_obj_set_style_text_color(gui->main.btn_x1.get_obj(), lv_color_hex(0x000000), LV_PART_MAIN); // 黑色文字

    // 触摸点1 y坐标 按钮
    gui->main.btn_y1.init(150, 100, 80, 40, "0", &lv_customer_font_SourceHanSerifSC_Regular_15);
//    gui->main.btn_y1.set_align(LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_color(gui->main.btn_y1.get_obj(), lv_color_hex(0x34e6ff), LV_PART_MAIN); // 蓝色背景
    lv_obj_set_style_text_color(gui->main.btn_y1.get_obj(), lv_color_hex(0x000000), LV_PART_MAIN); // 黑色文字

    // 触摸点2 x坐标 按钮
    gui->main.btn_x2.init(50, 150, 80, 40, "0", &lv_customer_font_SourceHanSerifSC_Regular_15);
//    gui->main.btn_x2.set_align(LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_color(gui->main.btn_x2.get_obj(), lv_color_hex(0x34e6ff), LV_PART_MAIN); // 蓝色背景
    lv_obj_set_style_text_color(gui->main.btn_x2.get_obj(), lv_color_hex(0x000000), LV_PART_MAIN); // 黑色文字

    // 触摸点2 y坐标 按钮
    gui->main.btn_y2.init(150, 150, 80, 40, "0", &lv_customer_font_SourceHanSerifSC_Regular_15);
//    gui->main.btn_y2.set_align(LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_color(gui->main.btn_y2.get_obj(), lv_color_hex(0x34e6ff), LV_PART_MAIN); // 蓝色背景
    lv_obj_set_style_text_color(gui->main.btn_y2.get_obj(), lv_color_hex(0x000000), LV_PART_MAIN); // 黑色文字
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

auto CounterLogic::set_value_label(int value) -> void
{
    counter_value = value;
    char buf[12];
    sprintf(buf, "%d", counter_value);
    gui->main.label_counter.set_text(buf);
}
auto CounterLogic::set_value_x1(int value) -> void
{
    char buf[12];
    sprintf(buf, "%d", value);
    gui->main.btn_x1.set_text(buf);
}
auto CounterLogic::set_value_y1(int value) -> void
{
    char buf[12];
    sprintf(buf, "%d", value);
    gui->main.btn_y1.set_text(buf);
}
auto CounterLogic::set_value_x2(int value) -> void
{
    char buf[12];
    sprintf(buf, "%d", value);
    gui->main.btn_x2.set_text(buf);
}
auto CounterLogic::set_value_y2(int value) -> void
{
    char buf[12];
    sprintf(buf, "%d", value);
    gui->main.btn_y2.set_text(buf);
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
    CounterLogic::set_value_label(value);
}
auto UI::set_x1_value(int value) -> void
{
    CounterLogic::set_value_x1(value);
}
auto UI::set_y1_value(int value) -> void
{
    CounterLogic::set_value_y1(value);
}
auto UI::set_x2_value(int value) -> void
{
    CounterLogic::set_value_x2(value);
}
auto UI::set_y2_value(int value) -> void
{
    CounterLogic::set_value_y2(value);
}
auto UI::get_counter_value() -> int
{
    return CounterLogic::get_value();
}