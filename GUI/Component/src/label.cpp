//
// Created by fairy on 2025/1/2 16:41.
//

#include "label.hpp"


/**
 * @brief 初始化文本框
 * @param component    文本框对象指针
 * @param text  文本字符串
 * @param font  文本框字体
 */
auto Label::init() -> void
{
    _obj = lv_label_create(_parent);
}

auto Label::init(Strings text, Font font) -> void
{
    init();
    // 初始化文本内容
    set_text(text);
    set_font(font);
}
auto Label::init(Strings text, Font font, Obj parent) -> void
{
    _obj = lv_label_create(parent);
    // 初始化文本内容
    set_text(text);
    set_font(font);
}

auto Label::init( Coord x, Coord y, Coord w, Coord h, Strings string, Font font, Color text_color) -> void
{
    init(string, font);
    set_pos_size(x, y, w, h);
    set_text_color(text_color);
}

/**
 * @brief 设置字体和字间距
 * @param font 字体格式
 * @param space 字间距，默认为0
 * @return
 */
auto
Label::set_font(Font font, Selector selector) -> void
{
    lv_obj_set_style_text_font(_obj, font, selector);
}

[[maybe_unused]] auto Label::set_space(lv_coord_t space, Selector selector) -> void
{
    lv_obj_set_style_text_letter_space(_obj, space, selector);
}

auto Label::set_text_color(lv_color_t color) -> void
{
    lv_obj_set_style_text_color(_obj, color, selector_default);
}

[[maybe_unused]] auto Label::set_text_align(Align align) -> void
{
    lv_obj_set_style_text_align(_obj, align, selector_default);
}


/**
 * @brief 设置格式化文本内容
 * @param fmt 格式字符串
 * @param args 变长参数
 * @note 莫名其妙出现混账的乱码问题，感觉不出有什么错误，可确实显示不对，真是魂淡
 *      使用auto简化之后就没有任何问题了，只能说auto既能简化，还不用管这些乱七八糟的的事，真神
 *      使用前一定要设置好对象，该函数由于变长参数，无法在后面追加Obj参数,所以无法设置缺省.
 *          现在发现无法转义浮点数%f，这并非是这种格式的问题，而是lv_label_set_text_fmt本身就具备这个问题
 */
void Label::set_text(Strings fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    lv_label_set_text_fmt(_obj, fmt, args);
    va_end(args);
}




