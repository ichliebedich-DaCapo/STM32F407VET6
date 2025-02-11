//
// Created by fairy on 2025/1/2 19:26.
//
#ifndef SIMULATOR_GUI_BASE_HPP
#define SIMULATOR_GUI_BASE_HPP


#ifdef ARM_MATH_CM4

#endif
// 头文件
#include "component.hpp"
#include "timer.hpp"
#include "chart.hpp"
#include "slider.hpp"
#include "image.hpp"
#include "label.hpp"
#include "button.hpp"
#include "imageButton.hpp"
#include "FPS.hpp"
#include "anim.hpp"
#include "CPU.hpp"
#include "roller.hpp"
#include "dropdown.hpp"


namespace gui
{
    // 初始化相关
    namespace init
    {
        void screen();// 初始化界面，需要用户实现
        void events();// 初始化事件，需要用户实现
    }


    // 控件定义
    namespace widgets
    {
        // 主屏幕
        namespace main
        {
            extern Component scr;// 主屏幕，由于会与init里的screen函数重名，所以就简化了名称
        }

        // 占个位置
        namespace others
        {

        }
    }


    // 自己定义各种对外接口，用于app里调用驱动时可以实时更新画面
    namespace interface
    {

    }
}



#endif //SIMULATOR_GUI_BASE_HPP