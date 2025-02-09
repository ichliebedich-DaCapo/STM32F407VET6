//
// Created by fairy on 2024/10/27 15:00.
//
#ifndef SIMULATOR_LABEL_HPP
#define SIMULATOR_LABEL_HPP

#include "component.hpp"


/**
 * @brief 文本框类
 */
class Label : public Widget<Label>
{
public:
    // 初始化文本框
    inline Label &init(Obj parent = parent_)
    {
        create_obj(&lv_label_class);
        return *this;
    }

    Label &init(Strings text, Font font, Obj parent = parent_)
    {
        init(parent);
        // 初始化文本内容
        lv_label_set_text(obj_, text);
        Label::font(font);
        return *this;
    }

    Label &init(Coord x, Coord y, Coord w, Coord h, Strings string, Font font, Color text_color = lv_color_black())
    {
        init(string, font);
        pos_size(x, y, w, h);
        Label::text_color(text_color);
        return *this;
    }

    // 设置字体
    Label &font(Font font, Selector selector = selector_default)
    {
        lv_obj_set_style_text_font(obj_, font, selector);
        return *this;
    }

    // 设置字间距
    Label &space(Coord space, Selector selector = selector_default)
    {
        lv_obj_set_style_text_letter_space(obj_, space, selector);
        return *this;
    }

    // 设置文本颜色
    Label &text_color(Color color, Selector selector = selector_default)
    {
        lv_obj_set_style_text_color(obj_, color, selector);
        return *this;
    }


    // 设置文本对齐方式
    Label &text_align(Align_text align, Selector selector = selector_default)
    {
        lv_obj_set_style_text_align(obj_, align, selector);
        return *this;
    }


    /**
     * @brief 设置文本
     * @param fmt
     * @param ...
     * @note 不使用auto来简化，因为会导致二进制膨胀
     * @return
     */
    inline Label &text(Strings fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        lv_label_set_text_fmt(obj_, fmt, args);
        va_end(args);
        return *this;
    }

};


#endif //SIMULATOR_LABEL_HPP