//
// Created by fairy on 2024/10/27 14:57.
//
#ifndef SIMULATOR_IMAGEBUTTON_HPP
#define SIMULATOR_IMAGEBUTTON_HPP

#include "widget.hpp"


/**
 * @brief 图片按钮初始化
 */
class ImageButton : public Widget<ImageButton>
{
public:
    // 使用前必须设置父对象
    ImageButton &init(Obj_t parent = parent_)
    {
        create_obj(&lv_imagebutton_class,parent);
        return *this;
    }

    ImageButton &init(Coord x, Coord y, Coord w, Coord h, ImageSrc release_src = nullptr,
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
    ImageButton &src(ImageButton_State state, ImageSrc src)
    {
        lv_imgbtn_set_src(obj_, state, nullptr, src, nullptr);
        return *this;
    }

    // 设置按压图片
    ImageButton &pressed_src(ImageSrc src_left,ImageSrc src_mid= nullptr, ImageSrc src_right= nullptr)
    {
        lv_imgbtn_set_src(obj_, LV_IMGBTN_STATE_CHECKED_RELEASED, src_left, src_mid, src_right);
        return *this;
    }

    // 设置释放图片
    ImageButton &released_src(ImageSrc src_left,ImageSrc src_mid= nullptr, ImageSrc src_right= nullptr)
    {
        lv_imgbtn_set_src(obj_, LV_IMGBTN_STATE_RELEASED, src_left, src_mid, src_right);
        return *this;
    }

    // 设置选中按压图片
    ImageButton &checked_pressed_src(ImageSrc src_left,ImageSrc src_mid= nullptr, ImageSrc src_right= nullptr)
    {
        lv_imgbtn_set_src(obj_, LV_IMGBTN_STATE_CHECKED_RELEASED, src_left, src_mid, src_right);
        return *this;
    }

    // 设置选中释放图片
    ImageButton &checked_released_src(ImageSrc src_left,ImageSrc src_mid= nullptr, ImageSrc src_right= nullptr)
    {
        lv_imgbtn_set_src(obj_, LV_IMGBTN_STATE_CHECKED_RELEASED, src_left, src_mid, src_right);
        return *this;
    }



    // 默认发送的事件为LV_EVENT_CLICKED,因为平时用得最多就是这个。后面可自行添加
    ImageButton &press()
    {
        lv_imgbtn_set_state(obj_, LV_IMGBTN_STATE_CHECKED_RELEASED);
        lv_obj_add_state(obj_, LV_STATE_CHECKED); // 选中
        send_event<LV_EVENT_CLICKED>();
        return *this;
    }

    ImageButton &release()
    {
        lv_imgbtn_set_state(obj_, LV_IMGBTN_STATE_RELEASED);
        lv_obj_remove_state(obj_, LV_STATE_CHECKED); // 取消选中
        send_event<LV_EVENT_CLICKED>();
        // 这个发送事件逻辑并未修复
        return *this;
    }






};


#endif //SIMULATOR_IMAGEBUTTON_HPP