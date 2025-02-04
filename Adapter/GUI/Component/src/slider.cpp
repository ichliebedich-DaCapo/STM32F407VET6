//
// Created by fairy on 2025/1/2 17:12.
//

#include "slider.hpp"

auto Slider::init(Obj component) -> void
{
    _obj = lv_slider_create(_parent);
}

auto Slider::set_range(Coord min, Coord max) -> void
{
    lv_slider_set_range(_obj, min, max);
}

auto Slider::set_bg_color(Color color, lv_opa_t opa, Selector selector) -> void
{
    lv_obj_set_style_bg_color(_obj, color, selector);
    if (opa != 50)
        lv_obj_set_style_bg_opa(_obj, opa, selector);
}

auto Slider::set_value(int32_t value, lv_anim_enable_t anim_en) -> void
{
    lv_slider_set_value(_obj, value, anim_en);
}

auto Slider::set_bg_src(ImageSrc src, Selector selector) -> void
{
    lv_obj_set_style_bg_img_src(_obj, src, selector);
}

auto Slider::init(Obj component, Coord x, Coord y, Coord w, Coord h, Coord min, Coord max, Color bg_color, uint8_t opa,
                  ImageSrc src) -> void
{
    init(component);
    set_range(min, max);
    set_pos_size(x, y, w, h);
    set_bg_color(bg_color, opa);
    if (src)
        set_bg_src(src);
}