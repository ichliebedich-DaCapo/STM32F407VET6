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
    // 初始化文本框
    static inline auto init(Obj component, Strings text = nullptr, Font font = nullptr,Obj parent=_parent) -> void;

    // 设置字体
    static inline auto
    set_font(Font font, Selector selector = selector_default) -> void;

    // 设置字间距
    [[maybe_unused]] static inline auto
    set_space(Coord space, Selector selector = selector_default) -> void;

    // 设置文本颜色
    static inline auto set_text_color(Color color) -> void;

    // 设置文本对齐方式
    [[maybe_unused]] static inline auto set_text_align(Align align,Obj obj=_obj) -> void;

    // 设置文本内容
    static inline auto set_text(Strings text,Obj obj=_obj) -> void;

    // 设置文本内容格式化
    [[maybe_unused]] static inline auto set_text(Strings fmt, auto... args) -> void;
    [[maybe_unused]] static inline auto set_text(Obj obj, Strings fmt, auto... args) -> void;

    /*自定义初始化函数*/
    inline auto init(Obj text,  Coord x, Coord y, Coord w, Coord h,Strings string= nullptr,Color text_color=lv_color_hex(0))->void ;

    // 设置文本框字体
    [[maybe_unused]] inline auto init_font(Font font) -> void;



private:
    Font _font{};

};


/**
 * @brief 初始化文本框
 * @param component    文本框对象指针
 * @param text  文本字符串
 * @param font  文本框字体
 */
inline auto Text::init(Obj component, Strings text, Font font,Obj parent) -> void
{
    component = lv_label_create(parent);
    _obj = component;

    // 初始化文本内容
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
inline auto
Text::set_font(Font font, Selector selector) -> void
{
    lv_obj_set_style_text_font(_obj, font, selector);
}

[[maybe_unused]] inline auto Text::set_space(lv_coord_t space, Selector selector) -> void
{
    lv_obj_set_style_text_letter_space(_obj, space, selector);
}

inline auto Text::set_text_color(lv_color_t color) -> void
{
    lv_obj_set_style_text_color(_obj, color, selector_default);
}

[[maybe_unused]] inline auto Text::set_text_align(lv_text_align_t align,Obj obj) -> void
{
    lv_obj_set_style_text_align(obj, align, selector_default);
}

inline auto Text::set_text(Strings text,Obj obj) -> void
{
    lv_label_set_text(obj, text);
}

/**
 * @brief 设置格式化文本内容
 * @param fmt 格式字符串
 * @param args 变长参数
 * @note 莫名其妙出现混账的乱码问题，感觉不出有什么错误，可确实显示不对，真是魂淡
 *      使用auto简化之后就没有任何问题了，只能说auto既能简化，还不用管这些乱七八糟的的事，真神
 *      使用前一定要设置好对象，该函数由于变长参数，无法在后面追加Obj参数,所以无法设置缺省
 */

[[maybe_unused]] inline auto Text::set_text(Strings fmt, auto... args) -> void
{
    lv_label_set_text_fmt(_obj, fmt, args...);
}
[[maybe_unused]] inline auto Text::set_text(Obj obj, Strings fmt, auto... args) -> void
{
    lv_label_set_text_fmt(obj, fmt, args...);
}

/**
 * @brief 初始化字体
 * @param font
 */
[[maybe_unused]] auto Text::init_font(Font font) -> void
{
    _font = font;
}

auto Text::init(Obj text, Coord x, Coord y, Coord w, Coord h, Strings string,Color text_color) -> void
{
    init(text, string, _font);
    set_pos_size(x, y, w, h);
    set_text_color(text_color);
}

#endif
#endif //SIMULATOR_TEXT_HPP