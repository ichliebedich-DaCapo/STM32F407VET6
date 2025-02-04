//
// Created by fairy on 2024/10/28 13:41.
//
#ifndef SIMULATOR_BUTTON_HPP
#define SIMULATOR_BUTTON_HPP

#include "component.hpp"
#include "label.hpp"

class Button : public Component
{
public:

    // 使用前必须设置父对象
    auto init() -> void;

    // 自定义风格按钮
    auto init(Coord x, Coord y, Coord w, Coord h, Strings text= nullptr ,Font font= nullptr) -> void;

    // 点击事件
    auto click() -> void;

    // 设置字体类型
    [[maybe_unused]] auto set_font(Font font) -> void;

    [[maybe_unused]] auto set_text(Strings text) -> void;

private:
    Label label;
};


#endif //SIMULATOR_BUTTON_HPP