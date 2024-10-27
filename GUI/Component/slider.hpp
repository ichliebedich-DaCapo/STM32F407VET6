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
    static inline auto init(_lv_obj_t *&component) -> void
    {
        component = lv_slider_create(_parent);
        _obj = component;
    }

    static inline auto set_range(lv_coord_t min, lv_coord_t max) -> void
    {
        lv_slider_set_range(_obj, min, max);
    }

    static inline auto set_value(int32_t value, lv_anim_enable_t anim_en = LV_ANIM_OFF,_lv_obj_t*&obj=_obj) -> void
    {
        lv_slider_set_value(obj, value, anim_en);
    }

    static inline auto set_bg_color(lv_color_t color, lv_opa_t opa = 50,
                                    lv_style_selector_t selector = LV_PART_MAIN | LV_PART_MAIN) -> void
    {
        lv_obj_set_style_bg_color(_obj, color, selector);
        if (opa != 50)
            lv_obj_set_style_bg_opa(_obj, opa, selector);
    }

    static inline auto set_bg_src(lv_obj_t *obj, const void *src, lv_style_selector_t selector = LV_PART_KNOB) -> void
    {
        lv_obj_set_style_bg_img_src(obj, src, selector);
    }

};
#endif
#endif //SIMULATOR_SLIDER_HPP