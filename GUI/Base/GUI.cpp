//
// Created by fairy on 2024/10/17 18:53.
//
#include "GUI.hpp"
#include "scr_screen.hpp"

#if 1
// 头文件
#include "gui_guider.h"
#include "custom.h"
#include "events_init.h"

// 函数
auto GUI::init() -> void
{
    init_scr_del_flag(&guider_ui);
    Screen::init(&guider_ui);
    lv_scr_load(guider_ui.screen);

    custom_init(&guider_ui);
}


#endif

