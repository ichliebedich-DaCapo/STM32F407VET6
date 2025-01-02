//
// Created by fairy on 2024/10/17 19:48.
//
#ifndef SIMULATOR_COMPONENT_HPP
#define SIMULATOR_COMPONENT_HPP

// 头文件
#include "GUI_Base.hpp"


// 定义常量
constexpr const Selector selector_default = (static_cast<uint32_t >(LV_PART_MAIN) |
                                             static_cast<uint32_t >(LV_STATE_DEFAULT));
constexpr const Selector selector_ticks = (static_cast<uint32_t >(LV_STATE_DEFAULT));


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
    static auto set_parent(Obj parent) -> void;

    // 设置组件
    [[maybe_unused]] static auto set_object(Obj object) -> void;

    // 初始化自定义组件
    static auto init(Obj component) -> void;

    static auto init(Obj component, Coord x, Coord y) -> void;

    static auto init(Obj component, Coord x, Coord y, Coord w, Coord h) -> void;

    // 设置位置和尺寸
    static auto set_pos_size(Coord x, Coord y, Coord w, Coord h) -> void;// 设置位置和尺寸
    static auto set_pos(Coord x, Coord y) -> void;// 设置位置
    static auto set_size(Coord w, Coord h) -> void;// 设置尺寸

    // 显示组件和隐藏组件
    static auto appear(Obj obj = _obj) -> void;

    static auto hidden(Obj obj = _obj) -> void;

    // 清除标志、添加标志
    static auto clear_flag(lv_obj_flag_t flag, Obj obj = _obj) -> void;

    [[maybe_unused]] static auto add_flag(lv_obj_flag_t flag, Obj obj = _obj) -> void;

    // 设置为裁剪属性，即子对象的内容会被父对象的圆角部分裁剪掉，而不会超出父对象的圆角边界。
    [[maybe_unused]] static auto set_clip_corner(Selector selector = selector_default) -> void;

    // 移除所有样式,没有圆角方框
    [[maybe_unused]] static auto remove_all_style(Obj obj = _obj) -> void;

    // 移动到图层底部
    [[maybe_unused]] static auto move_to_background(Obj obj = _obj) -> void;

    // 重绘
    static auto invalidate(Obj obj = _obj) -> void;

    // 设置对齐
    static auto
    set_align(lv_align_t align, Coord x_offset = 0, Coord y_offset = 0, Obj obj = _obj) -> void;

    // 设置样式大小
    static auto set_style_size(Coord w, Coord h, Selector selector, Obj obj = _obj) -> void;

    // 设置背景颜色
    static auto
    bg_color(Color color, Selector selector = selector_default) -> void;

    // 设置背景透明度
    static auto bg_opa(uint8_t opa, Selector selector = selector_default) -> void;

    // 设置背景渐变方向
    static auto
    bg_grad_dir(lv_grad_dir_t dir, Selector selector = selector_default) -> void;

    // 设置背景渐变颜色
    static auto
    bg_grad_color(Color color, Selector selector = selector_default) -> void;

    // 设置边框宽度
    static auto
    border_width(Coord width, Selector selector = selector_default) -> void;

    // 设置边框颜色
    static auto
    border_color(Color color, Selector selector = selector_default) -> void;

    // 设置边框透明度
    static auto
    border_opa(uint8_t opa, Selector selector = selector_default) -> void;

    // 设置边框xxx
    static auto
    border_side(lv_border_side_t side = LV_BORDER_SIDE_FULL, Selector selector = selector_default) -> void;

    // 设置边框圆角半径
    static auto
    border_radius(Coord radius, Selector selector = selector_default) -> void;

    // 设置线条宽度
    static auto
    line_width(Coord width, Selector selector = selector_default) -> void;

    // 设置阴影宽度
    static auto
    shadow_width(Coord width, Selector selector = selector_default) -> void;

    // 设置文本颜色,一般用于图表或刻度盘，现在失效了
    [[maybe_unused]] static auto
    text_color(Color color, Selector selector = selector_default) -> void;

    // 设置文本透明度,一般用于图表或刻度盘
    [[maybe_unused]] static auto text_opa(uint8_t opa, Selector selector = selector_ticks) -> void;

    // 设置文本字体,一般用于图表或刻度盘
    [[maybe_unused]] static auto
    text_font(Font font, Selector selector = selector_ticks) -> void;

    // 设置背景渐变
    static auto
    bg_grad(Color color, Color grad_color, uint8_t opa = 255, lv_grad_dir_t grad_dir = LV_GRAD_DIR_HOR) -> void;

    // 设置边框样式
    static auto
    border(Color color, Coord radius = 5, Coord width = 1, uint8_t opa = 255) -> void;


    /*初始化器*/


protected:
    static inline Obj_t _obj;
    static inline Obj_t _parent;
};




#endif //SIMULATOR_COMPONENT_HPP