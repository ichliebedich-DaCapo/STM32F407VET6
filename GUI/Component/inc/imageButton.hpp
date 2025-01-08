//
// Created by fairy on 2024/10/27 14:57.
//
#ifndef SIMULATOR_IMAGEBUTTON_HPP
#define SIMULATOR_IMAGEBUTTON_HPP

#include "component.hpp"


/**
 * @brief 图片按钮初始化
 */
class ImageButton : public Component
{
public:
    // 使用前必须设置父对象
     auto init() -> void;
    auto init(Coord x, Coord y, Coord w, Coord h, ImageSrc release_src = nullptr,
              ImageSrc press_src = nullptr) -> void;

    // 默认居中，大部分情况下都用不到旁边两个参数，需要时再补充

     auto set_src(lv_imagebutton_state_t  state, ImageSrc src) -> void;

    // 默认发送的事件为LV_EVENT_CLICKED,因为平时用得最多就是这个。后面可自行添加
     auto press() -> void;

     auto release() -> void;


};



#endif //SIMULATOR_IMAGEBUTTON_HPP