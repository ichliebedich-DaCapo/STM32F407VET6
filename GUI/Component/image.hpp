//
// Created by fairy on 2024/10/27 15:01.
//
#ifndef SIMULATOR_IMAGE_HPP
#define SIMULATOR_IMAGE_HPP

#include "component.hpp"
/* 预编译命令 */
#if 1
// 图片类
class Image : public Component
{
public:
    // 使用前必须设置父对象
    static inline auto init(Obj component) -> void;

    //  设置图片资源
    static inline auto set_src(const lv_img_dsc_t *src) -> void;

    static inline auto init(Obj component,Coord x,Coord y,Coord w,Coord h,ImageSrc src= nullptr) -> void;
};

auto Image::set_src(const lv_img_dsc_t *src) -> void
{
    lv_img_set_src(_obj, src);
}

auto Image::init(Obj component) -> void
{
    component = lv_img_create(_parent);
    _obj = component;
}

auto Image::init(Obj component, Coord x, Coord y, Coord w, Coord h, ImageSrc src) -> void
{
    init(component);

    set_src(src);
    set_pos_size(x,y,w,h);
    add_flag(LV_OBJ_FLAG_CLICKABLE);
}

/* 预编译命令 */
#endif
#endif //SIMULATOR_IMAGE_HPP