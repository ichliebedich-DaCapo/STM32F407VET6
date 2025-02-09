//
// Created by fairy on 2024/10/17 18:53.
//
#include "GUI.hpp"

// 函数
auto GUI::resource_init() -> void
{
    using namespace gui;// 使用命名空间，简化书写

    widgets::main::scr.init().size(DISP_HOR_RES, DISP_VER_RES);// 默认父对象为nullpter
    Component::parent(widgets::main::scr);// 默认主屏幕为父对象
    init::screen();// 初始化屏幕
    lv_obj_update_layout(widgets::main::scr);// 更新布局
    init::events();// 初始化事件
    lv_scr_load(widgets::main::scr);// 加载主屏幕
}



