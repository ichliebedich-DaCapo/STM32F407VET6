//
// Created by fairy on 2024/10/27 14:57.
//
#ifndef SIMULATOR_IMAGEBUTTON_HPP
#define SIMULATOR_IMAGEBUTTON_HPP

#include "component.hpp"


/**
 * @brief 图片按钮初始化
 */
class ImageButton : public Widget<ImageButton>
{
public:
    // 使用前必须设置父对象
    ImageButton& init()
    {
        create_obj(&lv_imagebutton_class);
        return *this;
    }

    ImageButton& init(Coord x, Coord y, Coord w, Coord h, ImageSrc release_src = nullptr,
                      ImageSrc press_src = nullptr)
    {
        init();
        pos_size(x, y, w, h);
        add_flag(LV_OBJ_FLAG_CHECKABLE);
        src(LV_IMGBTN_STATE_RELEASED, release_src);// 正常状态
        src(LV_IMGBTN_STATE_CHECKED_RELEASED, press_src);// 按下后状态
        return *this;
    }



    // 默认居中，大部分情况下都用不到旁边两个参数，需要时再补充
    ImageButton& src(lv_imagebutton_state_t  state, ImageSrc src)
    {
        lv_imgbtn_set_src(obj_, state, nullptr, src, nullptr);
        return *this;
    }

    // 默认发送的事件为LV_EVENT_CLICKED,因为平时用得最多就是这个。后面可自行添加
    ImageButton& press(lv_event_cb_t event_cb)
    {
        lv_imgbtn_set_state(obj_, LV_IMGBTN_STATE_CHECKED_RELEASED);
        lv_obj_add_state(obj_, LV_STATE_CHECKED); // 选中
        send_event<LV_EVENT_CLICKED>();
        return *this;
    }


    ImageButton& release()
    {
        lv_imgbtn_set_state(obj_, LV_IMGBTN_STATE_RELEASED);
        lv_obj_clear_state(obj_, LV_STATE_CHECKED); // 取消选中
        send_event<LV_EVENT_CLICKED>();
        return *this;
    }


};



#endif //SIMULATOR_IMAGEBUTTON_HPP