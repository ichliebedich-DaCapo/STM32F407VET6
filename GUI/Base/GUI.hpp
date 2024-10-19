//
// Created by fairy on 2024/10/17 18:53.
//
#ifndef FURINA_GUI_HPP
#define FURINA_GUI_HPP

#include "JYZQ_Conf.h"
/* 预编译命令 */
#if 1
// 头文件
#include "GUI_Base.hpp"
// 宏定义




/**
 * @brief GUI类
 */
class GUI:public GUI_Base
{
public:
    static auto init() -> void;// 初始化界面
};

/* 预编译命令 */
#endif
#endif //FURINA_GUI_HPP