//
// Created by fairy on 2025/1/2 17:00.
//

#include "imageButton.hpp"

// 初始化函数
auto ImageButton::init() -> void
{
    _obj = lv_imgbtn_create(_parent);
}

auto ImageButton::init(Coord x, Coord y, Coord w, Coord h, ImageSrc release_src, ImageSrc press_src) -> void
{
    //     图片按钮：停止、播放
    ImageButton::init();
    ImageButton::set_pos_size(x, y, w, h);
    ImageButton::add_flag(LV_OBJ_FLAG_CHECKABLE);
    ImageButton::set_src(LV_IMGBTN_STATE_RELEASED, release_src);// 正常状态
    ImageButton::set_src(LV_IMGBTN_STATE_CHECKED_RELEASED, press_src);// 按下后状态
}


// 设置图片源
auto ImageButton::set_src(lv_imagebutton_state_t state, ImageSrc src) -> void
{
    lv_imgbtn_set_src(_obj, state, nullptr, src, nullptr);
}

// 按下按钮
auto ImageButton::press() -> void
{
    lv_imgbtn_set_state(_obj, LV_IMGBTN_STATE_CHECKED_RELEASED);
    lv_obj_add_state(_obj, LV_STATE_CHECKED); // 选中
    send_event(LV_EVENT_CLICKED);
}

// 释放按钮
auto ImageButton::release() -> void
{
    lv_imgbtn_set_state(_obj, LV_IMGBTN_STATE_RELEASED);
    lv_obj_clear_state(_obj, LV_STATE_CHECKED); // 取消选中
    send_event(LV_EVENT_CLICKED);
}

