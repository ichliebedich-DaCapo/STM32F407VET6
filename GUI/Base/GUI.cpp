//
// Created by fairy on 2024/10/17 18:53.
//
#include "GUI.hpp"
#include "component.hpp"

#if 1
// 头文件
#include "events.hpp"
#include "gui_guider.hpp"


// 函数
auto GUI::init() -> void
{
    gui->screen_dell= true;
    Screen::init();// 初始化屏幕
    lv_obj_update_layout(gui->main.screen);
    Events::init();
    lv_scr_load(gui->main.screen);
}


#endif

