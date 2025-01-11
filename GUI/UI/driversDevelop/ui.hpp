//
// Created by fairy on 2025/1/10 18:43.
//
#ifndef SIMULATOR_UI_HPP
#define SIMULATOR_UI_HPP

#include "GUI_Base.hpp"
struct lv_ui_t
{
    // 主屏幕
    struct
    {
        Component screen{};// 屏幕自身
        Button btn_test{};
    } main;// 主屏幕

};
extern struct lv_ui_t* gui;

// UI接口
class UI
{
public:
   static void pressA();
   static void pressS();
};


#endif //SIMULATOR_UI_HPP