//
// Created by fairy on 2024/10/17 18:53.
//
#include "GUI.hpp"
#include "component.hpp"

#if 1
// 头文件
#include "events.hpp"


// 函数
auto GUI::init() -> void
{
    Screen::init();// 初始化屏幕
    Events::init();
    lv_scr_load(guider_ui.screen);
}


#endif

