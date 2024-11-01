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
    static inline auto init(Obj imgbtn) -> void;

    // 默认居中，大部分情况下都用不到旁边两个参数，需要时再补充
    static inline auto set_src(lv_imgbtn_state_t state, ImageSrc src) -> void;

    // 默认发送的事件为LV_EVENT_CLICKED,因为平时用得最多就是这个。后面可自行添加
    static inline auto press(Obj obj = _obj) -> void;

    static inline auto release(Obj obj = _obj) -> void;

    /*初始化器*/
    static inline auto init(Obj imgbtn,Coord x, Coord y, Coord w, Coord h,ImageSrc release_src, ImageSrc press_src) -> void {
        //     图片按钮：停止、播放
        ImageButton::init(imgbtn);
        ImageButton::set_pos_size(x,y,w,h);
        ImageButton::add_flag(LV_OBJ_FLAG_CHECKABLE);
        ImageButton::set_src(LV_IMGBTN_STATE_RELEASED, release_src);// 正常状态
        ImageButton::set_src(LV_IMGBTN_STATE_CHECKED_RELEASED, press_src);// 按下后状态
    }
};

// 初始化函数
 auto ImageButton::init(Obj imgbtn) -> void {
    imgbtn = lv_imgbtn_create(_parent);
    _obj = imgbtn;
}

// 设置图片源
 auto ImageButton::set_src(lv_imgbtn_state_t state, ImageSrc src) -> void {
    lv_imgbtn_set_src(_obj, state, nullptr, src, nullptr);
}

// 按下按钮
 auto ImageButton::press(Obj obj) -> void {
    if (obj == nullptr) {
        obj = _obj;
    }
    lv_imgbtn_set_state(obj, LV_IMGBTN_STATE_CHECKED_RELEASED);
    lv_obj_add_state(obj, LV_STATE_CHECKED); // 选中
    lv_event_send(obj, LV_EVENT_CLICKED, nullptr);
}

// 释放按钮
 auto ImageButton::release(Obj obj) -> void {
    if (obj == nullptr) {
        obj = _obj;
    }
    lv_imgbtn_set_state(obj, LV_IMGBTN_STATE_RELEASED);
    lv_obj_clear_state(obj, LV_STATE_CHECKED); // 取消选中
    lv_event_send(obj, LV_EVENT_CLICKED, nullptr);
}

#endif //SIMULATOR_IMAGEBUTTON_HPP