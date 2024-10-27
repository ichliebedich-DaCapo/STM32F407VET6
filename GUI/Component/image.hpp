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
    static inline auto init(_lv_obj_t *&component) -> void
    {
        component = lv_img_create(_parent);
        _obj = component;
    }

    //  设置图片资源
    static inline auto set_src(const lv_img_dsc_t *src) -> void
    {
        lv_img_set_src(_obj, src);
    }
};

/* 预编译命令 */
#endif
#endif //SIMULATOR_IMAGE_HPP