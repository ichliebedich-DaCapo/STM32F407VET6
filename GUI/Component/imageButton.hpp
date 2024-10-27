//
// Created by fairy on 2024/10/27 14:57.
//
#ifndef SIMULATOR_IMAGEBUTTON_HPP
#define SIMULATOR_IMAGEBUTTON_HPP

#include "component.hpp"
/* 预编译命令 */
#if 1
// 按钮类
class ImageButton : public Component
{
public:
    // 使用前必须设置父对象
    static inline auto init(_lv_obj_t *&component) -> void
    {
        component = lv_imgbtn_create(_parent);
        _obj = component;
    }

    // 默认居中，大部分情况下都用不到旁边两个参数，需要时再补充
    static inline auto set_src(lv_imgbtn_state_t state, const lv_img_dsc_t *src) -> void
    {
        lv_imgbtn_set_src(_obj, state, nullptr, src, nullptr);
    }

    //默认发送的事件为LV_EVENT_CLICKED,因为平时用得最多就是这个。后面可自行添加
    static inline auto press(_lv_obj_t*&obj=_obj)-> void
    {
        lv_imgbtn_set_state(obj, LV_IMGBTN_STATE_CHECKED_RELEASED);
        lv_obj_add_state(obj, LV_STATE_CHECKED);// 选中
        lv_event_send(obj, LV_EVENT_CLICKED, nullptr);

    }
    static inline auto release(_lv_obj_t*&obj=_obj)-> void
    {
        lv_imgbtn_set_state(obj, LV_IMGBTN_STATE_RELEASED);
        lv_obj_clear_state(obj, LV_STATE_CHECKED);// 取消选中
        lv_event_send(obj, LV_EVENT_CLICKED, nullptr);
    }
};
#endif
#endif //SIMULATOR_IMAGEBUTTON_HPP