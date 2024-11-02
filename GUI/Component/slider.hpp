//
// Created by fairy on 2024/10/27 15:00.
//
#ifndef SIMULATOR_SLIDER_HPP
#define SIMULATOR_SLIDER_HPP

#include "component.hpp"
/* 预编译命令 */
#if 1

class Slider : public Component
{
public:
    // 使用前必须设置父对象
    static inline auto init(Obj component) -> void;

    static inline auto set_range(Coord min, Coord max) -> void;

    static inline auto set_value(int32_t value, lv_anim_enable_t anim_en = LV_ANIM_OFF, Obj obj = _obj) -> void;

    static inline auto set_bg_color(Color color, lv_opa_t opa = 50, Selector selector = LV_PART_MAIN) -> void;

    static inline auto set_bg_src(ImageSrc src, Obj obj = _obj, Selector selector = LV_PART_KNOB) -> void;

    static inline auto init(Obj component, Coord x, Coord y, Coord w, Coord h, Coord min = 0, Coord max = 255,
                            Color bg_color = lv_color_hex(0x2195f6), uint8_t opa = 125, ImageSrc src = nullptr) -> void;

};

auto Slider::init(Obj component) -> void
{
    component = lv_slider_create(_parent);
    _obj = component;
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

auto Slider::set_value(int32_t value, lv_anim_enable_t anim_en, _lv_obj_t *&obj) -> void
{
    lv_slider_set_value(obj, value, anim_en);
}

auto Slider::set_bg_src(ImageSrc src, Obj obj, Selector selector) -> void
{
    lv_obj_set_style_bg_img_src(obj, src, selector);
}

auto Slider::init(Obj component, Coord x, Coord y, Coord w, Coord h, Coord min, Coord max, Color bg_color, uint8_t opa,
                  ImageSrc src) -> void
{
    init(component);
    set_range(min, max);
    set_pos_size(x, y, w, h);
    set_bg_color(bg_color, opa);
    if(src)
        set_bg_src(src);
}

#endif
#endif //SIMULATOR_SLIDER_HPP