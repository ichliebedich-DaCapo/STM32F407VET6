//
// Created by fairy on 2024/10/28 13:41.
//
#ifndef SIMULATOR_BUTTON_HPP
#define SIMULATOR_BUTTON_HPP

#include "widgets.hpp"

class Button : public Widget<Button>
{
public:

    // 使用前必须设置父对象
    inline Button &init(Strings text = nullptr, Font font = nullptr, Obj parent = parent_)
    {
        create_obj(&lv_button_class, parent);

        // 创建label，以Button为父对象
        label = lv_label_create(obj_);
        Button::font(font);
        lv_label_set_text(label, text);
        lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);// 设置label居中
        return *this;
    }

    inline Button &
    init(Coord x, Coord y, Coord w, Coord h, Strings text = nullptr, Font font = nullptr, Obj parent = parent_)
    {
        init(text, font, parent);
        border_radius(5);
        bg_color(Color_Firefly_Green);
        bg_opa(125);
        return *this;
    }


    // 点击事件
    inline Button &click()
    {
        send_event<LV_EVENT_CLICKED>();
        return *this;
    }

    // 设置字体类型
    inline Button &font(Font font)
    {
        lv_obj_set_style_text_font(label, font, selector_default);
        return *this;
    }

    // 设置文本
    Button &text(Strings fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        lv_label_set_text_fmt(obj_, fmt, args);
        va_end(args);
        return *this;
    }


private:
    Obj_t label;
};


#endif //SIMULATOR_BUTTON_HPP