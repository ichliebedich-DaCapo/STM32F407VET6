//
// Created by fairy on 2024/10/28 13:41.
//
#ifndef SIMULATOR_BUTTON_HPP
#define SIMULATOR_BUTTON_HPP

#include "component.hpp"

class [[maybe_unused]] Button : public Component
{
public:
    Button()
    {
        btn_bg_color = Color_Firefly_Green;
        btn_text_color = lv_color_hex(0);
        btn_opa = 125;
        btn_shadow_width = 1;
        btn_radius = 5;
    }

    // 使用前必须设置父对象
    static inline auto init(Obj component) -> void;

    // 自定义风格按钮
    inline auto init(Obj btn, Obj text, Coord x, Coord y, Coord w, Coord h, Strings string = nullptr) -> void;


    // 设置字体类型
    [[maybe_unused]] inline auto init_font(Font font) -> void;

    // 设置背景颜色
    [[maybe_unused]] inline auto init_bg_color(Color color) -> void;

    // 设置文本颜色
    [[maybe_unused]] inline auto init_text_color(Color color) -> void;

    // 设置透明度
    [[maybe_unused]] inline auto init_bg_opa(uint8_t opa) -> void;

    // 设置阴影宽度
    [[maybe_unused]] inline auto init_shadow_width(uint8_t width) -> void;

private:
    Color btn_bg_color{};// 背景颜色
    Color btn_text_color{};
    Font btn_font{};// 文本字体
    Coord btn_radius{};
    uint8_t btn_opa{};
    uint8_t btn_shadow_width{};
};


/**
 * @brief 自定义按钮类
 * @details 鉴于平时创建按钮都大同小异，为了方便创建按钮，仿照声明式创建，但创建时会占用一定的栈开销。
 *          可以通过设置背景颜色、字体、文字颜色、圆角等来创建不同风格的按钮。
 */
auto Button::init(Obj component) -> void
{
    component = lv_btn_create(_parent);
    _obj = component;
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
auto Button::init(Obj btn, Obj text, Coord x, Coord y, Coord w, Coord h, Strings string) -> void
{
    Button::init(btn);
    Button::set_pos_size(x, y, w, h);
    Button::border_radius(btn_radius);
    Button::bg_color(btn_bg_color);
    Button::bg_opa(btn_opa);
    Button::shadow_width(btn_shadow_width);

    Text::init(text, string, btn_font,btn);
    Text::set_align(LV_ALIGN_CENTER);
    Text::set_text_color(btn_text_color);
}

[[maybe_unused]] auto Button::init_font(Font font) -> void
{
    btn_font = font;
}

[[maybe_unused]] auto Button::init_bg_color(Color color) -> void
{
    btn_bg_color = color;
}

[[maybe_unused]] auto Button::init_text_color(Color color) -> void
{
    btn_text_color = color;
}

[[maybe_unused]] auto Button::init_bg_opa(uint8_t opa) -> void
{
    btn_opa = opa;

}

[[maybe_unused]] auto Button::init_shadow_width(uint8_t width) -> void
{
    btn_shadow_width = width;

}


#endif //SIMULATOR_BUTTON_HPP