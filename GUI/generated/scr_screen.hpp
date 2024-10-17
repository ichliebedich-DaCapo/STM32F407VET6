//
// Created by fairy on 2024/10/17 19:48.
//
#ifndef SIMULATOR_SCR_SCREEN_HPP
#define SIMULATOR_SCR_SCREEN_HPP

/* 预编译命令 */
#if 1
// 头文件
#include "gui_guider.h"
// 宏定义


/**
 * @brief 屏幕类
 */
class Screen
{
public:
    static auto init(lv_ui *ui) -> void;// 初始化屏幕
};


/**
 * @brief 组件初始化基类
 * @note 需要自己去实现create函数
 */
class ComponentInitializer
{
public:
    /**
     * @brief 设置图片位置和尺寸
     * @param x,y 默认起始坐标为左上角（参考系起点也在左上角）
     * @param w,h 图片宽高
     */
    inline auto set_pos_size(lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h) -> void
    {
        lv_obj_set_pos(obj, x, y);
        lv_obj_set_size(obj, w, h);
    }

    inline auto add_flag(lv_obj_flag_t flag) -> void
    {
        lv_obj_add_flag(obj, flag);
    }

    // 设置为裁切圆角
    inline auto set_style_clip_corner(lv_style_selector_t selector) -> void
    {
        lv_obj_set_style_clip_corner(obj, true, selector);
    }

protected:
    _lv_obj_t *obj;
};



// 图片类
class ImageInitializer : public ComponentInitializer
{
public:
    inline auto init(_lv_obj_t *&parent, _lv_obj_t *&component) -> void
    {
        component = lv_img_create(parent);
        obj = component;
    }

    //  设置图片资源
    inline auto set_src(const lv_img_dsc_t *src) -> void
    {
        lv_img_set_src(obj, src);
    }
};

// 按钮类
class ImageButtonInitializer : public ComponentInitializer
{
public:
    inline auto init(_lv_obj_t *&parent, _lv_obj_t *&component) -> void
    {
        component = lv_imgbtn_create(parent);
        obj = component;
    }

    // 默认居中，大部分情况下都用不到旁边两个参数，需要时再补充
    inline auto set_src(lv_imgbtn_state_t state, const lv_img_dsc_t *src) -> void
    {
        lv_imgbtn_set_src(obj, state, nullptr, src, nullptr);
    }
};


/* 预编译命令 */
#endif
#endif //SIMULATOR_SCR_SCREEN_HPP