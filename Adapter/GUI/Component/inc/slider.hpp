//
// Created by fairy on 2024/10/27 15:00.
//
#ifndef SIMULATOR_SLIDER_HPP
#define SIMULATOR_SLIDER_HPP

#include "widget.hpp"

class Slider : public Widget<Slider>
{
public:
    // 使用前必须设置父对象
    Slider& init(Obj_t parent =parent_)
    {
        create_obj(&lv_slider_class,parent);
        return *this;
    }

    Slider& init(Coord x, Coord y, Coord w, Coord h, Coord min = 0, Coord max = 255,
                 Color bg_color = lv_color_hex(0x2195f6), uint8_t opa = 125, ImageSrc src = nullptr)
    {
        init();
        range(min, max);
        pos_size(x, y, w, h);
        Slider::bg_color(bg_color, opa);
        if (src)
            bg_src(src);

        return *this;
    }


    Slider& range(Coord min, Coord max)
    {
        lv_slider_set_range(obj_, min, max);
        return *this;
    }

    Slider& value(int32_t value, lv_anim_enable_t anim_en = LV_ANIM_OFF)
    {
        lv_slider_set_value(obj_, value, anim_en);
        return *this;
    }

    Slider& mode(lv_slider_mode_t mode=LV_SLIDER_MODE_NORMAL)
    {
        lv_slider_set_mode(obj_, mode);
        return *this;
    }

};




#endif //SIMULATOR_SLIDER_HPP