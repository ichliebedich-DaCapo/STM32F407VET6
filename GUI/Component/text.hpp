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
    static inline auto init(_lv_obj_t *&component, const char *text = nullptr) -> void
    {
        component = lv_label_create(_parent);
        _obj = component;
        if (text != nullptr)
            set_text(text);
    }

    /**
     * @brief 设置字体和字间距
     * @param font 字体格式
     * @param space 字间距，默认为0
     * @return
     */
    static inline auto set_text_font_space(const lv_font_t *font, lv_coord_t space = 0) -> void
    {
        lv_obj_set_style_text_font(_obj, font, LV_PART_MAIN | LV_STATE_DEFAULT);
        if (space)
            lv_obj_set_style_text_letter_space(_obj, space, LV_PART_MAIN | LV_STATE_DEFAULT);
    }

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