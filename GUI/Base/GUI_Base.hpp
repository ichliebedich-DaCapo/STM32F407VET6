//
// Created by fairy on 2025/1/2 19:26.
//
#ifndef SIMULATOR_GUI_BASE_HPP
#define SIMULATOR_GUI_BASE_HPP

#include <cstdio>
#ifdef ARM_MATH_CM4
#include "JYZQ_Conf.h"
#endif
// 头文件
#include "lvgl.h"
#include "timer.hpp"
#include "chart.hpp"
#include "slider.hpp"
#include "image.hpp"
#include "label.hpp"
#include "button.hpp"
#include "imageButton.hpp"
#include "FPS.hpp"
#include "anim.hpp"

class GUI_Base
{
public:
    static void screen_init();
    static void events_init();
};






#endif //SIMULATOR_GUI_BASE_HPP