//
// Created by fairy on 2024/10/28 13:41.
//
#ifndef SIMULATOR_BUTTON_HPP
#define SIMULATOR_BUTTON_HPP

#include "component.hpp"
/* 预编译命令 */
#if 1
class Button : public Component {
public:
    // 使用前必须设置父对象
    static inline auto init(_lv_obj_t *&component) -> void
    {
        component = lv_btn_create(_parent);
        _obj = component;
    }

};


/**
 * @brief 自定义按钮类
 * @details 为了方便创建按钮，仿照声明式创建，但创建时会占用一定的栈开销。
 */
class CustomButton
{
public:
    CustomButton()
    {
        _bg_color = Color_Firefly_Green;
        _text_color = lv_color_hex(0);
        _radius = 5;
        _bg_opa = 125;
    }

    inline auto bg_color(lv_color_t color) -> void
    {
        _bg_color = color;
    }

    inline auto text_color(lv_color_t color) -> void
    {
        _text_color = color;
    }

    inline auto font(const lv_font_t *font) -> void
    {
        _font = font;
    }

    inline auto border_radius(lv_coord_t radius) -> void
    {
        _radius = radius;
    }

    inline auto bg_opa(uint8_t opa) -> void
    {
        _bg_opa = opa;
    }

    inline auto init(_lv_obj_t *&btn, _lv_obj_t *&text, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h,
                     const char *string = nullptr) -> void
    {
        Button::init(btn);
        Button::set_pos_size(x, y, w, h);
        Button::border_radius(_radius);
        Button::bg_color(_bg_color);
        Button::bg_opa(_bg_opa);

        Text::set_parent(btn);// 设置父对象
        Text::init(text, string, _font);
        Text::set_align(LV_ALIGN_CENTER);
        Text::set_text_color(_text_color);
    }

private:
    lv_color_t _bg_color{};
    lv_color_t _text_color{};
    const lv_font_t *_font{};
    lv_coord_t _radius;
    uint8_t _bg_opa;
};
#endif
#endif //SIMULATOR_BUTTON_HPP