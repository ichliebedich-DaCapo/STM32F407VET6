//
// Created by fairy on 2024/10/27 15:00.
//
#ifndef SIMULATOR_TEXT_HPP
#define SIMULATOR_TEXT_HPP

#include "component.hpp"
/* 预编译命令 */
#if 1

/**
 * @brief 文本框类
 */
class Text : public Component
{
public:
    // 使用前必须设置父对象
    static inline auto init(_lv_obj_t *&component, const char *text = nullptr, const lv_font_t *font = nullptr) -> void
    {
        component = lv_label_create(_parent);
        _obj = component;

        if (text)
            set_text(text);
        if (font)
        {
            set_font(font);
        }
    }

    /**
     * @brief 设置字体和字间距
     * @param font 字体格式
     * @param space 字间距，默认为0
     * @return
     */
    static inline auto set_font(const lv_font_t *font, lv_style_selector_t selector=LV_PART_MAIN | LV_STATE_DEFAULT) -> void
    {
        lv_obj_set_style_text_font(_obj, font, selector);
    }

    // 设置字间距
    static inline auto set_space(lv_coord_t space,lv_style_selector_t selector=LV_PART_MAIN | LV_STATE_DEFAULT) -> void
    {
        lv_obj_set_style_text_letter_space(_obj, space, selector);
    }

    // 可以设置文本框或者作为文本框父对象的按钮
    static inline auto set_text_color(lv_color_t color) -> void
    {
        lv_obj_set_style_text_color(_obj, color, LV_PART_MAIN | LV_STATE_DEFAULT);
    }

    static inline auto set_text_align(lv_text_align_t align) -> void
    {
        lv_obj_set_style_text_align(_obj, align, LV_PART_MAIN | LV_STATE_DEFAULT);
    }

    static inline auto set_text(const char *text) -> void
    {
        lv_label_set_text(_obj, text);
    }

    static inline auto set_text_fmt(const char *fmt, ...) -> void
    {
        va_list args;
        va_start(args, fmt);
        lv_label_set_text_fmt(_obj, fmt, args);
        va_end(args);
    }
};

#endif
#endif //SIMULATOR_TEXT_HPP