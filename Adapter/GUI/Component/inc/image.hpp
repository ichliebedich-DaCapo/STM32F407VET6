//
// Created by fairy on 2024/10/27 15:01.
//
#ifndef SIMULATOR_IMAGE_HPP
#define SIMULATOR_IMAGE_HPP

#include "widget.hpp"

// 图片类
class Image : public Widget<Image>
{
public:
    // 使用前必须设置父对象
   inline Image& init(Obj_t parent= parent_)
    {
        create_obj(&lv_image_class,parent);
        return *this;
    }

    Image& init(Coord x,Coord y,Coord w,Coord h,ImageSrc src= nullptr)
    {
        init();
        Image::src(src);
        pos_size(x,y,w,h);
        add_flag(LV_OBJ_FLAG_CLICKABLE);
        return *this;
    }

    Image& init(Coord x,Coord y,Coord w,Coord h,ImageSrc_t& src)
    {
        init();
        Image::src(&src);
        pos_size(x,y,w,h);
        add_flag(LV_OBJ_FLAG_CLICKABLE);
        return *this;
    }

    //  设置图片资源
    Image& src(ImageSrc src)
    {
        lv_image_set_src(obj_,src);
        return *this;
    }

    inline Image& pivot(int32_t x=0,int32_t y=0)
    {
        lv_image_set_pivot(obj_,x,y);
        return *this;
    }

    // 设置图片旋转角度
    inline Image& rotation(int32_t angle)
    {
        lv_image_set_rotation(obj_,angle);
        return *this;
    }
};




#endif //SIMULATOR_IMAGE_HPP