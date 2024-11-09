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
constexpr const Selector selector_ticks = (static_cast<uint32_t >(LV_PART_TICKS) |
                                           static_cast<uint32_t >(LV_STATE_DEFAULT));


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
    static inline auto set_parent(Obj parent) -> void;

    // 设置组件
    [[maybe_unused]] static inline auto set_object(Obj object) -> void;

    // 初始化自定义组件
    static inline auto init(Obj component) -> void;

    // 设置位置和尺寸
    static inline auto set_pos_size(Coord x, Coord y, Coord w, Coord h) -> void;// 设置位置和尺寸
    static inline auto set_pos(Coord x, Coord y) -> void;// 设置位置
    static inline auto set_size(Coord w, Coord h) -> void;// 设置尺寸

    // 隐藏组件
    static inline auto hidden(Obj obj = _obj) -> void;

    // 显示组件
    static inline auto appear(Obj obj = _obj) -> void;

    // 添加标志
    [[maybe_unused]] static inline auto add_flag(lv_obj_flag_t flag, Obj obj = _obj) -> void;

    // 清除标志
    static inline auto clear_flag(lv_obj_flag_t flag, Obj obj = _obj) -> void;

    // 设置为裁剪属性，即子对象的内容会被父对象的圆角部分裁剪掉，而不会超出父对象的圆角边界。
    [[maybe_unused]] static inline auto set_clip_corner(Selector selector = selector_default) -> void;

    // 移除所有样式,没有圆角方框
    [[maybe_unused]] static inline auto remove_all_style() -> void;

    // 移动到图层底部
    [[maybe_unused]] static inline auto move_to_background(Obj obj = _obj) -> void;

    // 重绘
    static inline auto invalidate(Obj obj = _obj) -> void;

    // 设置对齐
    static inline auto
    set_align(lv_align_t align, Coord x_offset = 0, Coord y_offset = 0, Obj obj = _obj) -> void;

    // 设置样式大小
    static inline auto set_style_size(Coord size, Selector selector) -> void;

    // 设置背景颜色
    static inline auto
    bg_color(lv_color_t color, Selector selector = selector_default) -> void;

    // 设置背景透明度
    static inline auto bg_opa(uint8_t opa, Selector selector = selector_default) -> void;

    // 设置背景渐变方向
    static inline auto
    bg_grad_dir(lv_grad_dir_t dir, Selector selector = selector_default) -> void;

    // 设置背景渐变颜色
    static inline auto
    bg_grad_color(lv_color_t color, Selector selector = selector_default) -> void;

    // 设置边框宽度
    static inline auto
    border_width(Coord width, Selector selector = selector_default) -> void;

    // 设置边框颜色
    static inline auto
    border_color(lv_color_t color, Selector selector = selector_default) -> void;

    // 设置边框透明度
    static inline auto
    border_opa(uint8_t opa, Selector selector = selector_default) -> void;

    // 设置边框xxx
    static inline auto
    border_side(lv_border_side_t side = LV_BORDER_SIDE_FULL, Selector selector = selector_default) -> void;

    // 设置边框圆角半径
    static inline auto
    border_radius(Coord radius, Selector selector = selector_default) -> void;

    // 设置线条宽度
    static inline auto
    line_width(Coord width, Selector selector = selector_default) -> void;

    // 设置阴影宽度
    static inline auto
    shadow_width(Coord width, Selector selector = selector_default) -> void;

    // 设置文本颜色,一般用于图表或刻度盘，现在失效了
    [[maybe_unused]] static inline auto
    text_color(lv_color_t color, Selector selector = selector_default) -> void;

    // 设置文本透明度,一般用于图表或刻度盘
    [[maybe_unused]] static inline auto text_opa(uint8_t opa, Selector selector = selector_ticks) -> void;

    // 设置文本字体,一般用于图表或刻度盘
    [[maybe_unused]] static inline auto
    text_font(lv_font_t *font, Selector selector = selector_ticks) -> void;

    // 设置背景渐变
    static inline auto
    bg_grad(Color color, Color grad_color, uint8_t opa = 255, lv_grad_dir_t grad_dir = LV_GRAD_DIR_HOR) -> void;

    // 设置边框样式
    static inline auto
    border(Color color, Coord radius = 5, Coord width = 1, uint8_t opa = 255) -> void;


    /*初始化器*/


protected:
    static inline _lv_obj_t *_obj;
    static inline _lv_obj_t *_parent;

    /*初始化器*/
};

// 设置父对象
auto Component::set_parent(Obj parent) -> void
{
    _parent = parent;
}

// 设置组件
[[maybe_unused]] auto Component::set_object(Obj object) -> void
{
    _obj = object;
}

// 初始化自定义组件
auto Component::init(Obj component) -> void
{
    component = lv_obj_create(_parent);
    _obj = component;
}

/**
 * @brief 设置图片位置和尺寸
 * @param x,y 默认起始坐标为左上角（参考系起点也在左上角）
 * @param w,h 图片宽高
 */
auto Component::set_pos_size(Coord x, Coord y, Coord w, Coord h) -> void
{
    set_pos(x, y);
    set_size(w, h);
}

// 设置位置
auto Component::set_pos(Coord x, Coord y) -> void
{
    lv_obj_set_pos(_obj, x, y);
}

// 设置尺寸
auto Component::set_size(Coord w, Coord h) -> void
{
    lv_obj_set_size(_obj, w, h);
}

// 添加标志
[[maybe_unused]] auto Component::add_flag(lv_obj_flag_t flag, Obj obj) -> void
{
    lv_obj_add_flag(obj, flag);
}

// 清除标志
auto Component::clear_flag(lv_obj_flag_t flag, Obj obj) -> void
{
    lv_obj_clear_flag(obj, flag);
}

// 设置为裁剪属性
[[maybe_unused]] auto Component::set_clip_corner(Selector selector) -> void
{
    lv_obj_set_style_clip_corner(_obj, true, selector);
}

// 移除所有样式
[[maybe_unused]] auto Component::remove_all_style() -> void
{
    lv_obj_remove_style_all(_obj);
}

// 移动到图层底部
[[maybe_unused]] auto Component::move_to_background(Obj obj) -> void
{
    lv_obj_move_background(obj);
}

// 重绘
auto Component::invalidate(Obj obj) -> void
{
    lv_obj_invalidate(obj);  // 使频谱区域无效，触发重绘
}

// 设置对齐
auto Component::set_align(lv_align_t align, Coord x_offset, Coord y_offset, Obj obj) -> void
{
    lv_obj_align(obj, align, x_offset, y_offset);
}

// 设置样式大小
auto Component::set_style_size(Coord size, Selector selector) -> void
{
    lv_obj_set_style_size(_obj, size, selector);
}

// 设置背景颜色
auto Component::bg_color(lv_color_t color, Selector selector) -> void
{
    lv_obj_set_style_bg_color(_obj, color, selector);
}

// 设置背景透明度
auto Component::bg_opa(uint8_t opa, Selector selector) -> void
{
    lv_obj_set_style_bg_opa(_obj, opa, selector);
}

// 设置背景渐变方向
auto Component::bg_grad_dir(lv_grad_dir_t dir, Selector selector) -> void
{
    lv_obj_set_style_bg_grad_dir(_obj, dir, selector);
}

// 设置背景渐变颜色
auto Component::bg_grad_color(lv_color_t color, Selector selector) -> void
{
    lv_obj_set_style_bg_grad_color(_obj, color, selector);
}

// 设置边框宽度
auto Component::border_width(Coord width, Selector selector) -> void
{
    lv_obj_set_style_border_width(_obj, width, selector);
}

// 设置边框颜色
auto Component::border_color(lv_color_t color, Selector selector) -> void
{
    lv_obj_set_style_border_color(_obj, color, selector);
}

// 设置边框透明度
auto Component::border_opa(uint8_t opa, Selector selector) -> void
{
    lv_obj_set_style_border_opa(_obj, opa, selector);
}

// 设置边框xxx
auto Component::border_side(lv_border_side_t side, Selector selector) -> void
{
    lv_obj_set_style_border_side(_obj, side, selector);
}

// 设置边框圆角半径
auto Component::border_radius(Coord radius, Selector selector) -> void
{
    lv_obj_set_style_radius(_obj, radius, selector);
}

// 设置线条宽度
auto Component::line_width(Coord width, Selector selector) -> void
{
    lv_obj_set_style_line_width(_obj, width, selector);
}

// 设置阴影宽度
auto Component::shadow_width(Coord width, Selector selector) -> void
{
    lv_obj_set_style_shadow_width(_obj, width, selector);
}

// 设置文本颜色
[[maybe_unused]] auto Component::text_color(lv_color_t color, Selector selector) -> void
{
    lv_obj_set_style_text_color(_obj, color, selector);
}

// 设置文本透明度
[[maybe_unused]] auto Component::text_opa(uint8_t opa, Selector selector) -> void
{
    lv_obj_set_style_text_opa(_obj, opa, selector);
}

// 设置文本字体
[[maybe_unused]] auto Component::text_font(lv_font_t *font, Selector selector) -> void
{
    lv_obj_set_style_text_font(_obj, font, selector);
}


/**
 * @brief 设置背景渐变
 * @param color 背景起始颜色
 * @param grad_color 背景渐变颜色
 * @param opa 背景透明度
 * @param grad_dir 渐变方向
 */
auto Component::bg_grad(Color color, Color grad_color, uint8_t opa, lv_grad_dir_t grad_dir) -> void
{
    bg_color(color);
    bg_opa(opa);
    bg_grad_dir(grad_dir);// 设置垂直渐变
    bg_grad_color(grad_color);// 设置渐变结束颜色
}

/**
 * @brief 设置边框样式
 * @param color   边框颜色
 * @param radius  边框圆角半径
 * @param width   边框宽度
 * @param opa     边框透明度
 */
auto Component::border(Color color, Coord radius, Coord width, uint8_t opa) -> void
{
    border_radius(radius);
    border_color(color);
    border_opa(opa);
    border_width(width);
}

auto Component::hidden(Obj obj) -> void
{
    add_flag(LV_OBJ_FLAG_HIDDEN, obj);
}

auto Component::appear(Obj obj) -> void
{
    clear_flag(LV_OBJ_FLAG_HIDDEN, obj);
}


#endif //SIMULATOR_COMPONENT_HPP