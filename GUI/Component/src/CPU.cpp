//
// Created by fairy on 2025/1/11 15:18.
//
#include "CPU.hpp"
// 头文件


// 函数
auto CPU::init(Coord x, Coord y, Coord width, Coord height) -> void
{
    label_cpu.init(x, y, width, height, "CPU: -", LV_FONT_DEFAULT);
}