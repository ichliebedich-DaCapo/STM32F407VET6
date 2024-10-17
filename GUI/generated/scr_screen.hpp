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
 * @brief 组件基类
 */
// class ComponentInitializer
//{
//public:
//    inline auto init(_lv_obj_t *&parent, _lv_obj_t *&obj) -> void
//    {
//        obj = lv_obj_create(parent);
//    }
//
//    inline auto set_pos_size(lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h) -> void
//    {
//        lv_obj_set_pos(obj, x, y);
//    }
// private:
//    _lv_obj_t *obj;
//}


/**
 * @brief 图片类
 */
class ImageInitializer
{
public:
    // 初始化ui对象
    inline auto init(_lv_obj_t *&parent, _lv_obj_t *&obj) -> void
    {
        obj = lv_img_create(parent);
        img = obj;
    }

    inline auto set_Src(const lv_img_dsc_t *src) -> void
    {
        lv_img_set_src(img, src);
    }

    /**
     * @brief 设置图片位置和尺寸
     * @param x,y 默认起始坐标为左上角（参考系起点也在左上角）
     * @param w,h 图片宽高
     */
    inline auto set_pos_size(lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h) -> void
    {
        lv_obj_set_pos(img, x, y);
        lv_obj_set_size(img, w, h);
    }

    // 把图片风格设置为裁切圆角
    inline auto set_style_clip_corner(lv_style_selector_t selector) -> void
    {
        lv_obj_set_style_clip_corner(img, true, selector);
    }

    inline auto add_flag(lv_obj_flag_t flag) -> void
    {
        lv_obj_add_flag(img, flag);
    }

private:
    _lv_obj_t *img;
};

/**
 * @brief 按钮类
 * @note 默认不加文本框，为啥？因为有了图片，正经人谁还会用文字（除了+*-/）
 */
class ImageButtonInitializer
{
public:
    inline auto init(_lv_obj_t *&parent, _lv_obj_t *&obj) -> void
    {
        obj = lv_imgbtn_create(parent);
        img_btn = obj;
    }

    // 默认居中，大部分情况下都用不到旁边两个参数，需要时再补充
    inline auto set_Src(lv_imgbtn_state_t state, const lv_img_dsc_t *src) -> void
    {
        lv_imgbtn_set_src(img_btn, state, nullptr, src, nullptr);
    }

    inline auto set_pos_size(lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h) -> void
    {
        lv_obj_set_pos(img_btn, x, y);
        lv_obj_set_size(img_btn, w, h);
    }

    inline auto add_flag(lv_obj_flag_t flag) -> void
    {
        lv_obj_add_flag(img_btn, flag);
    }

    inline auto set_style_radius(lv_coord_t radius, lv_style_selector_t selector) -> void
    {
        lv_obj_set_style_radius(img_btn, radius, selector);
    }

    inline auto set_style_bg_opa(lv_opa_t opa, lv_style_selector_t selector) -> void
    {
        lv_obj_set_style_bg_opa(img_btn, opa, selector);
    }

    inline auto set_style_clip_corner(lv_style_selector_t selector) -> void
    {
        lv_obj_set_style_clip_corner(img_btn, true, selector);
    }

private:
    _lv_obj_t *img_btn;
};


/* 预编译命令 */
#endif
#endif //SIMULATOR_SCR_SCREEN_HPP