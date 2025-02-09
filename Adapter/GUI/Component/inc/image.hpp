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
   inline Image& init()
    {
        create_obj(&lv_image_class);
        return *this;
    }

    Image& init(Coord x,Coord y,Coord w,Coord h,ImageSrc src= nullptr)
    {
        init();
        set_src(src);
        pos_size(x,y,w,h);
        add_flag(LV_OBJ_FLAG_CLICKABLE);
        return *this;
    }

    Image& init(Coord x,Coord y,Coord w,Coord h,ImageSrc_t& src)
    {
        init();
        set_src(&src);
        pos_size(x,y,w,h);
        add_flag(LV_OBJ_FLAG_CLICKABLE);
        return *this;
    }

    //  设置图片资源
    Image& set_src(ImageSrc src)
    {
        lv_image_set_src(obj_,src);
        return *this;
    }

};




#endif //SIMULATOR_IMAGE_HPP