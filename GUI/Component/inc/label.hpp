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
    auto init()->void;
    auto init(Strings text, Font font) -> void;
    auto init(Strings text, Font font,Obj parent)->void ;
    auto init(Coord x, Coord y, Coord w, Coord h,Strings string, Font font,Color text_color=lv_color_black())->void ;

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
    auto set_text(Strings text) -> void;

    // 设置文本内容格式化
    [[maybe_unused]] auto set_text(Strings fmt, auto... args) -> void;


};

/*********************模板函数************************/
/**
 * @brief 设置格式化文本内容
 * @param fmt 格式字符串
 * @param args 变长参数
 * @note 莫名其妙出现混账的乱码问题，感觉不出有什么错误，可确实显示不对，真是魂淡
 *      使用auto简化之后就没有任何问题了，只能说auto既能简化，还不用管这些乱七八糟的的事，真神
 *      使用前一定要设置好对象，该函数由于变长参数，无法在后面追加Obj参数,所以无法设置缺省
 */

[[maybe_unused]] auto Label::set_text(Strings fmt, auto... args) -> void
{
    lv_label_set_text_fmt(_obj, fmt, args...);
}



#endif //SIMULATOR_LABEL_HPP