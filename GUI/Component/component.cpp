/*
* Copyright 2024 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "lvgl.h"
#include "component.hpp"

extern void ui_init();


auto Screen::init() -> void
{
    init_scr_del_flag(&guider_ui);
    ui_init();// 由UI下面文件实现
    lv_obj_update_layout(guider_ui.screen);
}
