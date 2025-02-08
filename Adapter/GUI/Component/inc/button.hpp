//
// Created by fairy on 2024/10/28 13:41.
//
#ifndef SIMULATOR_BUTTON_HPP
#define SIMULATOR_BUTTON_HPP

#include "component.hpp"
#include "label.hpp"

class Button : public Widget<Button>
{
public:

    // 使用前必须设置父对象
    Button& init(Obj parent = parent_)
    {
        create_obj(&lv_button_class);
        return *this;
    }

    Button& init(Coord x, Coord y, Coord w, Coord h, Strings text= nullptr ,Font font= nullptr)
    {
        create_obj(&lv_button_class);
        init(x,y,w,h,text,font);
        border_radius(5);
        bg_color(Color_Firefly_Green);
        bg_opa(125);
        label.init(text, font, obj_);
        label.center();
        return *this;
    }


    // 点击事件
    Button& click()
    {
        send_event<LV_EVENT_CLICKED>();
        return *this;
    }

    // 设置字体类型
    Button& font(Font font)
    {
        label.font(font);
        return *this;
    }


    // 设置文本
    Button& text(Strings text)
    {
        label.text(text);
        return *this;
    }



private:
    Label label;
};


#endif //SIMULATOR_BUTTON_HPP