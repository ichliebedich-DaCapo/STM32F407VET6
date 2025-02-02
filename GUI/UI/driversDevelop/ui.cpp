//
// Created by fairy on 2025/1/10 18:43.
//
#include "ui.hpp"
// 头文件

// 变量
struct lv_ui_t lv_ui;
struct lv_ui_t *gui = &lv_ui;


// 函数
void GUI_Base::screen_init()
{
    gui->main.btn_test.init(100, 100, 60, 50, "123", LV_FONT_DEFAULT);
    gui->main.btn_test.text_color(lv_color_black());


//    CPU::init();

}

void GUI_Base::events_init()
{

}


/****************************************UI接口*********************************/
void UI::pressA()
{

}

void UI::pressS()
{


}
