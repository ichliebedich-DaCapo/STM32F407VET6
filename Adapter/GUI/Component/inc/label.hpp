//
// Created by fairy on 2024/10/27 15:00.
//
#ifndef SIMULATOR_LABEL_HPP
#define SIMULATOR_LABEL_HPP

#include "widget.hpp"


/**
 * @brief 文本框类
 * @details 如果文本不设置字体就传入字符串，那么会卡死
 */
class Label : public Widget<Label>
{
public:
    /**
     * @brief 全局设置字体
     * @param font 使用引用类型，在函数内部取地址，避免还要多写一个&符号
     */
    static inline void Font(Font_t &font)
    {
        font_ = &font;
    }

    // 设置全局字体
    static inline void Font(::Font font)
    {
        font_ = font;
    }


    // 初始化文本框，默认为黑色字体
    inline Label &init(Obj parent = parent_)
    {
        create_obj(&lv_label_class, parent);
        Label::text_color(lv_color_black());// 设置文本颜色为黑色
        return *this;
    }

    // 只要设置文本，就默认为黑色字体
    inline Label &init(Strings text, Obj parent = parent_)
    {
        init(parent);
        // 初始化文本内容
        lv_label_set_text(obj_, text);
        if (font_)
            lv_obj_set_style_text_font(obj_, font_, selector_default);
        return *this;
    }


    // 文本颜色默认为黑,感觉这个最通用
    inline Label &init(Coord x, Coord y, Coord w, Coord h,Strings text="",Color text_color=lv_color_black())
    {
        init();
        pos_size(x, y, w, h);
        lv_label_set_text(obj_, text);
        Label::text_color(text_color);// 设置文本颜色为黑色
        return *this;
    }

//    inline Label &init(Coord x, Coord y, Coord w, Coord h, Strings string, Obj parent = parent_)
//    {
//        init(string, parent);
//        pos_size(x, y, w, h);
//        return *this;
//    }


    // 设置字间距
    inline Label &space(Coord space, Selector selector = selector_default)
    {
        lv_obj_set_style_text_letter_space(obj_, space, selector);
        return *this;
    }

    // 设置文本颜色
    inline Label &text_color(Color color, Selector selector = selector_default)
    {
        lv_obj_set_style_text_color(obj_, color, selector);
        return *this;
    }

    // 设置文本透明度
    Label &text_opa(uint8_t opa, Selector selector = selector_default)
    {
        lv_obj_set_style_text_opa(obj_, opa, selector);
        return *this;
    }



    // 设置文本对齐方式
    inline Label &text_align(Align_text align, Selector selector = selector_default)
    {
        lv_obj_set_style_text_align(obj_, align, selector);
        return *this;
    }

    // 设置字体
    inline Label &font(::Font font)
    {
        lv_obj_set_style_text_font(obj_, font, selector_default);
        return *this;
    }

    inline Label &font(Font_t &font)
    {
        lv_obj_set_style_text_font(obj_, &font, selector_default);
        return *this;
    }


    /**
     * @brief 设置文本
     * @param fmt
     * @param ...
     * @note 不使用auto来简化，因为会导致二进制膨胀
     * @return
     */
    Label &text(Strings fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        lv_label_set_text_fmt(obj_, fmt, args);
        va_end(args);
        return *this;
    }

private:
    static inline ::Font font_{};// 避免别名与本地函数名冲突
};


#endif //SIMULATOR_LABEL_HPP