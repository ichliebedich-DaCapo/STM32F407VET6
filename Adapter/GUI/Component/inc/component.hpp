//
// Created by fairy on 2024/10/17 19:48.
//
#ifndef SIMULATOR_COMPONENT_HPP
#define SIMULATOR_COMPONENT_HPP

#include "widget.hpp"

/**
 * @brief 空白组件类
 * @note 需要自己去实现init函数
 */
class Component : public Widget<Component>
{
public:
    // 极度危险！！
    // // 显式引入基类运算符
    //    using Widget<Component>::operator=;

    // 只能调用一次，不然会出现内存泄漏
    // 不主动设置父对象，那么就默认为上一次调用parent函数设置的父对象
    inline Component &init(Obj_t parent = parent_)
    {
        obj_ = lv_obj_create(parent);
        return *this;
    }

    Component &init(Coord x, Coord y, Coord w, Coord h, Obj parent = parent_)
    {
        obj_ = lv_obj_create(parent);
        pos_size(x, y, w, h);
        return *this;
    }

};


#endif //SIMULATOR_COMPONENT_HPP