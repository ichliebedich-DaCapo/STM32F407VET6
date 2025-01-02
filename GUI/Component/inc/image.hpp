//
// Created by fairy on 2024/10/27 15:01.
//
#ifndef SIMULATOR_IMAGE_HPP
#define SIMULATOR_IMAGE_HPP

#include "component.hpp"

// 图片类
class Image : public Component
{
public:
    // 使用前必须设置父对象
    auto init() -> void;

    //  设置图片资源
    auto set_src(const lv_img_dsc_t *src) -> void;

    auto init(Coord x,Coord y,Coord w,Coord h,ImageSrc src= nullptr) -> void;
};




#endif //SIMULATOR_IMAGE_HPP