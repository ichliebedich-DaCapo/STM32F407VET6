//
// Created by fairy on 2024/10/19 10:43.
//
#ifndef SIMULATOR_GUI_BASE_HPP
#define SIMULATOR_GUI_BASE_HPP

#include "JYZQ_Conf.h"
/* 预编译命令 */
#if 1
// 头文件
#include "ui.hpp"


// 宏定义
class GUI_Base
{
public:
    // 谁都有逼不得已的时候
    static inline auto get_ui()
    {
        return gui;
    }
protected:
    static inline lv_ui_t ui;// 内联变量，C++17特性
    static inline lv_ui_t *gui = &ui;// 可加可不加
};

// 接口

/* 预编译命令 */
#endif
#endif //SIMULATOR_GUI_BASE_HPP