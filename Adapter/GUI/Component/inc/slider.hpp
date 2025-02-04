//
// Created by fairy on 2024/10/27 15:00.
//
#ifndef SIMULATOR_SLIDER_HPP
#define SIMULATOR_SLIDER_HPP

#include "component.hpp"

class Slider : public Component
{
public:
    // 使用前必须设置父对象
    auto init(Obj component) -> void;
    auto init(Obj component, Coord x, Coord y, Coord w, Coord h, Coord min = 0, Coord max = 255,
              Color bg_color = lv_color_hex(0x2195f6), uint8_t opa = 125, ImageSrc src = nullptr) -> void;


    auto set_range(Coord min, Coord max) -> void;

    auto set_value(int32_t value, lv_anim_enable_t anim_en = LV_ANIM_OFF) -> void;

    auto set_bg_color(Color color, lv_opa_t opa = 50, Selector selector = LV_PART_MAIN) -> void;

    auto set_bg_src(ImageSrc src, Selector selector = LV_PART_KNOB) -> void;


};




#endif //SIMULATOR_SLIDER_HPP