//
// Created by fairy on 2024/10/27 15:00.
//
#ifndef SIMULATOR_LABEL_HPP
#define SIMULATOR_LABEL_HPP

#include "component.hpp"


/**
 * @brief 文本框类
 */
class Label : public Component
{
public:
    // 初始化文本框
    auto init() -> void;

    auto init(Strings text, Font font) -> void;

    auto init(Strings text, Font font, Obj parent) -> void;

    auto
    init(Coord x, Coord y, Coord w, Coord h, Strings string, Font font, Color text_color = lv_color_black()) -> void;

    // 设置字体
    auto
    set_font(Font font, Selector selector = selector_default) -> void;

    // 设置字间距
    [[maybe_unused]] auto
    set_space(Coord space, Selector selector = selector_default) -> void;

    // 设置文本颜色
    auto set_text_color(Color color) -> void;

    // 设置文本对齐方式
    [[maybe_unused]] auto set_text_align(Align align) -> void;

    // 设置文本内容
    void set_text(Strings fmt, ...);
};






#endif //SIMULATOR_LABEL_HPP