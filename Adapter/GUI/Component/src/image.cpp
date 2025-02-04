//
// Created by fairy on 2025/1/2 17:00.
//

#include "image.hpp"

auto Image::set_src(const lv_img_dsc_t *src) -> void
{
    lv_img_set_src(_obj, src);
}

auto Image::init() -> void
{
    _obj = lv_img_create(_parent);
}

auto Image::init( Coord x, Coord y, Coord w, Coord h, ImageSrc src) -> void
{
    init();
    set_src(src);
    set_pos_size(x,y,w,h);
    add_flag(LV_OBJ_FLAG_CLICKABLE);
}