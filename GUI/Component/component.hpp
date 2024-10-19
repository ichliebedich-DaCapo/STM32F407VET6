//
// Created by fairy on 2024/10/17 19:48.
//
#ifndef SIMULATOR_COMPONENT_HPP
#define SIMULATOR_COMPONENT_HPP

/* 预编译命令 */
#if 1
// 头文件
#include "GUI_Base.hpp"
// 宏定义


/**
 * @brief 屏幕类
 */
class Screen : public GUI_Base
{
public:
    static auto init() -> void;// 初始化屏幕
};


/**
 * @brief 组件初始化基类
 * @note 需要自己去实现create函数
 */
class Component
{
public:
    // 设置父对象
    static inline auto set_parent(_lv_obj_t *&parent) -> void
    {
        _parent = parent;
    }

    // 设置组件
    static inline auto set_object(_lv_obj_t *&object) -> void
    {
        _obj = object;
    }

    static inline auto init(_lv_obj_t *&component) -> void
    {
        component = lv_obj_create(_parent);
        _obj = component;
    }

    /**
     * @brief 设置图片位置和尺寸
     * @param x,y 默认起始坐标为左上角（参考系起点也在左上角）
     * @param w,h 图片宽高
     */
    static inline auto set_pos_size(lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h) -> void
    {
        lv_obj_set_pos(_obj, x, y);
        lv_obj_set_size(_obj, w, h);
    }

    static inline auto add_flag(lv_obj_flag_t flag) -> void
    {
        lv_obj_add_flag(_obj, flag);
    }

    // 设置为裁切圆角
    static inline auto set_style_clip_corner(lv_style_selector_t selector) -> void
    {
        lv_obj_set_style_clip_corner(_obj, true, selector);
    }

    // 移除所有样式,没有圆角方框
    static inline auto remove_all_style() -> void
    {
        lv_obj_remove_style_all(_obj);
    }

    // 移动到图层底部
    static inline auto move_to_background() -> void
    {
        lv_obj_move_background(_obj);
    }

    // 重绘
    static inline auto invalidate() -> void
    {
        lv_obj_invalidate(_obj);  // 使频谱区域无效，触发重绘
    }

protected:
    static inline _lv_obj_t *_obj;
    static inline _lv_obj_t *_parent;
};


// 图片类
class Image : public Component
{
public:
    // 使用前必须设置父对象
    static inline auto init(_lv_obj_t *&component) -> void
    {
        component = lv_img_create(_parent);
        _obj = component;
    }

    //  设置图片资源
    static inline auto set_src(const lv_img_dsc_t *src) -> void
    {
        lv_img_set_src(_obj, src);
    }
};

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
};

/**
 * @brief 文本框类
 */
class Text : public Component
{
public:
    // 使用前必须设置父对象
    static inline auto init(_lv_obj_t *&component, const char *text = nullptr) -> void
    {
        component = lv_label_create(_parent);
        _obj = component;
        if (text != nullptr)
            set_text(text);
    }

    /**
     * @brief 设置字体和字间距
     * @param font 字体格式
     * @param space 字间距，默认为0
     * @return
     */
    static inline auto set_text_font_space(const lv_font_t *font, lv_coord_t space = 0) -> void
    {
        lv_obj_set_style_text_font(_obj, font, LV_PART_MAIN | LV_STATE_DEFAULT);
        if (space)
            lv_obj_set_style_text_letter_space(_obj, space, LV_PART_MAIN | LV_STATE_DEFAULT);
    }

    static inline auto set_text_color(lv_color_t color) -> void
    {
        lv_obj_set_style_text_color(_obj, color, LV_PART_MAIN | LV_STATE_DEFAULT);
    }

    static inline auto set_text_align(lv_text_align_t align) -> void
    {
        lv_obj_set_style_text_align(_obj, align, LV_PART_MAIN | LV_STATE_DEFAULT);
    }

    static inline auto set_text(const char *text) -> void
    {
        lv_label_set_text(_obj, text);
    }

    static inline auto set_text_fmt(const char *fmt, ...) -> void
    {
        va_list args;
        va_start(args, fmt);
        lv_label_set_text_fmt(_obj, fmt, args);
        va_end(args);
    }
};


class Slider : public Component
{
public:
    // 使用前必须设置父对象
    static inline auto init(_lv_obj_t *&component) -> void
    {
        component = lv_slider_create(_parent);
        _obj = component;
    }

    static inline auto set_range(lv_coord_t min, lv_coord_t max) -> void
    {
        lv_slider_set_range(_obj, min, max);
    }

    static inline auto set_value(int32_t value, lv_anim_enable_t anim_en = LV_ANIM_OFF) -> void
    {
        lv_slider_set_value(_obj, value, anim_en);
    }

    static inline auto set_bg_color(lv_color_t color, lv_opa_t opa = 50,
                                    lv_style_selector_t selector = LV_PART_MAIN | LV_PART_MAIN) -> void
    {
        lv_obj_set_style_bg_color(_obj, color, selector);
        if (opa != 50)
            lv_obj_set_style_bg_opa(_obj, opa, selector);
    }

    static inline auto set_bg_src(lv_obj_t *obj, const void *src, lv_style_selector_t selector = LV_PART_KNOB) -> void
    {
        lv_obj_set_style_bg_img_src(obj, src, selector);
    }

};

/***********************************定时器**************************************/
class LV_Timer
{
public:
    // 默认为关闭状态
    auto inline create(lv_timer_cb_t timer_cb, uint32_t period = 1000, void *user_data = nullptr) -> void
    {
        if(_timer== nullptr)
        {
            _timer = lv_timer_create(timer_cb, period, user_data);
            pause();
        }
    }

    auto inline remove() -> void
    {
        lv_timer_del(_timer);
        _timer = nullptr;
    }

    // 恢复定时器
    auto inline resume() -> void
    {
        lv_timer_resume(_timer);
    }

    // 暂停定时器
    auto inline pause() -> void
    {
        lv_timer_pause(_timer);
    }

private:
    lv_timer_t *_timer{};
};



/* 预编译命令 */
#endif
#endif //SIMULATOR_COMPONENT_HPP