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
 * @note 需要自己去实现create函数.若要使用其他组件静态类，需要自行调用Component类的相关接口来设置parent和object
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

    // 初始化自定义组件
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

    // 添加标志
    static inline auto add_flag(lv_obj_flag_t flag) -> void
    {
        lv_obj_add_flag(_obj, flag);
    }

    // 设置为边框圆角
    static inline auto border_set_clip_corner(lv_style_selector_t selector = (LV_PART_MAIN | LV_STATE_DEFAULT)) -> void
    {
        lv_obj_set_style_clip_corner(_obj, true, selector);
    }

    // 移除所有样式,没有圆角方框
    static inline auto remove_all_style() -> void
    {
        lv_obj_remove_style_all(_obj);
    }

    // 移动到图层底部
    static inline auto move_to_background(_lv_obj_t *&obj = _obj) -> void
    {
        lv_obj_move_background(obj);
    }

    // 重绘
    static inline auto invalidate(_lv_obj_t *&obj = _obj) -> void
    {
        lv_obj_invalidate(obj);  // 使频谱区域无效，触发重绘
    }

    // 设置对齐
    static inline auto
    set_align(lv_align_t align, lv_coord_t x_offset, lv_coord_t y_offset, _lv_obj_t *&obj = _obj) -> void
    {
        lv_obj_align(obj, align, x_offset, y_offset);
    }

    // 设置样式大小
    static inline auto set_style_size(lv_coord_t size, lv_style_selector_t selector) -> void
    {
        lv_obj_set_style_size(_obj, size, selector);
    }

    // 设置背景颜色
    static inline auto
    bg_color(lv_color_t color, lv_style_selector_t selector = (LV_PART_MAIN | LV_STATE_DEFAULT)) -> void
    {
        lv_obj_set_style_bg_color(_obj, color, selector);
    }

    // 设置背景透明度
    static inline auto
    bg_opa(uint8_t opa, lv_style_selector_t selector = (LV_PART_MAIN | LV_STATE_DEFAULT)) -> void
    {
        lv_obj_set_style_bg_opa(_obj, opa, selector);
    }

    // 设置背景渐变方向
    static inline auto
    bg_grad_dir(lv_grad_dir_t dir, lv_style_selector_t selector = (LV_PART_MAIN | LV_STATE_DEFAULT)) -> void
    {
        lv_obj_set_style_bg_grad_dir(_obj, dir, selector);
    }

    // 设置背景渐变颜色
    static inline auto
    bg_grad_color(lv_color_t color, lv_style_selector_t selector = (LV_PART_MAIN | LV_STATE_DEFAULT)) -> void
    {
        lv_obj_set_style_bg_grad_color(_obj, color, selector);
    }

    // 设置边框宽度
    static inline auto
    border_width(lv_coord_t width, lv_style_selector_t selector = (LV_PART_MAIN | LV_STATE_DEFAULT)) -> void
    {
        lv_obj_set_style_border_width(_obj, width, selector);
    }

    // 设置边框颜色
    static inline auto
    border_color(lv_color_t color, lv_style_selector_t selector = (LV_PART_MAIN | LV_STATE_DEFAULT)) -> void
    {
        lv_obj_set_style_border_color(_obj, color, selector);
    }

    // 设置边框透明度
    static inline auto
    border_opa(uint8_t opa, lv_style_selector_t selector = (LV_PART_MAIN | LV_STATE_DEFAULT)) -> void
    {
        lv_obj_set_style_border_opa(_obj, opa, selector);
    }

    // 设置边框xxx
    static inline auto
    border_side(lv_border_side_t side = LV_BORDER_SIDE_FULL,
                lv_style_selector_t selector = (LV_PART_MAIN | LV_STATE_DEFAULT)) -> void
    {
        lv_obj_set_style_border_side(_obj, side, selector);
    }

    // 设置圆角半径
    static inline auto
    radius(lv_coord_t radius, lv_style_selector_t selector = (LV_PART_MAIN | LV_STATE_DEFAULT)) -> void
    {
        lv_obj_set_style_radius(_obj, radius, selector);
    }

    // 设置线条宽度
    static inline auto
    line_width(lv_coord_t width, lv_style_selector_t selector = (LV_PART_MAIN | LV_STATE_DEFAULT)) -> void
    {
        lv_obj_set_style_line_width(_obj, width, selector);
    }

    // 设置阴影宽度
    static inline auto
    shadow_width(lv_coord_t width, lv_style_selector_t selector = (LV_PART_MAIN | LV_STATE_DEFAULT)) -> void
    {
        lv_obj_set_style_shadow_width(_obj, width, selector);
    }

    // 设置文本颜色,一般用于图表或刻度盘
    static inline auto
    text_color(lv_color_t color, lv_style_selector_t selector = (LV_PART_TICKS|LV_STATE_DEFAULT)) -> void
    {
        lv_obj_set_style_text_color(_obj, color, selector);
    }

    // 设置文本透明度,一般用于图表或刻度盘
    static inline auto text_opa(uint8_t opa, lv_style_selector_t selector = (LV_PART_TICKS|LV_STATE_DEFAULT)) -> void
    {
        lv_obj_set_style_text_opa(_obj, opa, selector);
    }

    // 设置文本字体,一般用于图表或刻度盘
    static inline auto
    text_font(lv_font_t *font, lv_style_selector_t selector = (LV_PART_TICKS|LV_STATE_DEFAULT)) -> void
    {
        lv_obj_set_style_text_font(_obj, font, selector);
    }


protected:
    static inline _lv_obj_t *_obj;
    static inline _lv_obj_t *_parent;
};














/* 预编译命令 */
#endif
#endif //SIMULATOR_COMPONENT_HPP