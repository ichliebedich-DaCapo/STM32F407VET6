//
// Created by fairy on 2024/10/17 19:48.
//
#ifndef SIMULATOR_COMPONENT_HPP
#define SIMULATOR_COMPONENT_HPP

/* 预编译命令 */
#if 1
// 头文件
#include "gui_guider.hpp"
// 宏定义


/**
 * @brief 屏幕类
 */
class Screen
{
public:
    static auto init() -> void;// 初始化屏幕
};


/**
 * @brief 组件初始化基类
 * @note 需要自己去实现create函数
 */
class ComponentInitializer
{
public:
    // 设置父对象
    static inline auto set_parent(_lv_obj_t *&parent) -> void
    {
        _parent = parent;
    }

    /**
     * @brief 设置图片位置和尺寸
     * @param x,y 默认起始坐标为左上角（参考系起点也在左上角）
     * @param w,h 图片宽高
     */
    inline auto set_pos_size(lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h) -> void
    {
        lv_obj_set_pos(_obj, x, y);
        lv_obj_set_size(_obj, w, h);
    }

    inline auto add_flag(lv_obj_flag_t flag) -> void
    {
        lv_obj_add_flag(_obj, flag);
    }

    // 设置为裁切圆角
    inline auto set_style_clip_corner(lv_style_selector_t selector) -> void
    {
        lv_obj_set_style_clip_corner(_obj, true, selector);
    }

protected:
    _lv_obj_t *_obj;
    static _lv_obj_t *_parent;
};


// 图片类
class ImageInitializer : public ComponentInitializer
{
public:
    inline auto init(_lv_obj_t *&parent, _lv_obj_t *&component) -> void
    {
        component = lv_img_create(parent);
        _obj = component;
    }
    // 使用前必须设置父对象
//    inline auto p_init( _lv_obj_t *&component) -> void
//    {
//        component = lv_img_create(_parent);
//        _obj = component;
//    }

    //  设置图片资源
    inline auto set_src(const lv_img_dsc_t *src) -> void
    {
        lv_img_set_src(_obj, src);
    }
};

// 按钮类
class ImageButtonInitializer : public ComponentInitializer
{
public:
    inline auto init(_lv_obj_t *&parent, _lv_obj_t *&component) -> void
    {
        component = lv_imgbtn_create(parent);
        _obj = component;
    }        // 使用前必须设置父对象
    inline auto p_init(_lv_obj_t *&component) -> void
    {
        component = lv_imgbtn_create(_parent);
        _obj = component;
    }

    // 默认居中，大部分情况下都用不到旁边两个参数，需要时再补充
    inline auto set_src(lv_imgbtn_state_t state, const lv_img_dsc_t *src) -> void
    {
        lv_imgbtn_set_src(_obj, state, nullptr, src, nullptr);
    }
};

/**
 * @brief 文本框类
 */
class TextInitializer : public ComponentInitializer
{
public:

    inline auto init(_lv_obj_t *&parent, _lv_obj_t *&component, const char *text = nullptr) -> void
    {
        component = lv_label_create(parent);
        _obj = component;
        if (text != nullptr)
            lv_label_set_text(_obj, text);
    }

    // 使用前必须设置父对象
    inline auto p_init(_lv_obj_t *&component, const char *text = nullptr) -> void
    {
        component = lv_label_create(_parent);
        _obj = component;
        if (text != nullptr)
            lv_label_set_text(_obj, text);
    }

    /**
     * @brief 设置字体和字间距
     * @param font 字体格式
     * @param space 字间距，默认为0
     * @return
     */
    inline auto set_text_font_space(const lv_font_t *font, lv_coord_t space = 0) -> void
    {
        lv_obj_set_style_text_font(_obj, font, LV_PART_MAIN | LV_STATE_DEFAULT);
        if (space)
            lv_obj_set_style_text_letter_space(_obj, space, LV_PART_MAIN | LV_STATE_DEFAULT);
    }

    inline auto set_text_color(lv_color_t color) -> void
    {
        lv_obj_set_style_text_color(_obj, color, LV_PART_MAIN | LV_STATE_DEFAULT);
    }

    inline auto set_text_align(lv_text_align_t align) -> void
    {
        lv_obj_set_style_text_align(_obj, align, LV_PART_MAIN | LV_STATE_DEFAULT);
    }
};


class SliderInitializer : public ComponentInitializer
{
public:
    inline auto init(_lv_obj_t *&parent, _lv_obj_t *&component) -> void
    {
        component = lv_slider_create(parent);
        _obj = component;
    }

    // 使用前必须设置父对象
    inline auto p_init(_lv_obj_t *&component) -> void
    {
        component = lv_slider_create(_parent);
        _obj = component;
    }

    inline auto set_range(lv_coord_t min, lv_coord_t max) -> void
    {
        lv_slider_set_range(_obj, min, max);
    }

    inline auto set_bg_color(lv_color_t color, lv_opa_t opa = 50,
                             lv_style_selector_t selector = LV_PART_MAIN | LV_PART_MAIN) -> void
    {
        lv_obj_set_style_bg_color(_obj, color, selector);
        if (opa != 50)
            lv_obj_set_style_bg_opa(_obj, opa, selector);
    }
};
/* 预编译命令 */
#endif
#endif //SIMULATOR_COMPONENT_HPP