//
// Created by fairy on 2025/1/2 16:36.
//

#include "button.hpp"


/**
 * @brief 自定义按钮类
 * @details 鉴于平时创建按钮都大同小异，为了方便创建按钮，仿照声明式创建，但创建时会占用一定的栈开销。
 *          可以通过设置背景颜色、字体、文字颜色、圆角等来创建不同风格的按钮。
 */
auto Button::init() -> void
{
    _obj = lv_btn_create(_parent);

}

/**
 * @brief 自定义风格按钮
 * \注意事项 父对象一定要设置,使用静态函数set_parent来设置！！
 * @details 使用前可以调用init开头的函数来设置按钮的属性，否则会默认为默认风格。
 * @param btn 组件对象
 * @param text 文本对象
 * @param x
 * @param y
 * @param w
 * @param h
 * @param string 文本
 */
auto Button::init(Coord x, Coord y, Coord w, Coord h, Strings text, Font font) -> void
{
    Button::init();
    Button::set_pos_size(x, y, w, h);
    Button::border_radius(5);
    Button::bg_color(Color_Firefly_Green);
    Button::bg_opa(125);


    label.init(text, font, _obj);
    label.set_align(LV_ALIGN_CENTER);
}


auto Button::click() -> void
{
    send_event(LV_EVENT_CLICKED);
}

auto Button::set_font(Font font) -> void
{
    label.set_font(font);
}

auto Button::set_text(Strings text) -> void
{
    label.set_text(text);
}
