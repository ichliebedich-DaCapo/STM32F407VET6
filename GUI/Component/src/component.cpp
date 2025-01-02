//
// Created by fairy on 2024/10/27 14:59.
//

// 头文件
#include "component.hpp"
// 函数

// 设置父对象
auto Component::set_parent(Obj parent) -> void
{
    _parent = parent;
}


// 初始化自定义组件
auto Component::init() -> void
{
    _obj = lv_obj_create(_parent);
}

auto Component::init(Coord x, Coord y) -> void
{
    init();
    set_pos(x, y);
}

auto Component::init(Coord x, Coord y, Coord w, Coord h) -> void
{
    init();
    set_pos(x, y);
    set_size(w, h);
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

auto Component::hidden() -> void
{
    add_flag(LV_OBJ_FLAG_HIDDEN);
}

auto Component::appear() -> void
{
    clear_flag(LV_OBJ_FLAG_HIDDEN);
}


// 添加标志
[[maybe_unused]] auto Component::add_flag(lv_obj_flag_t flag) -> void
{
    lv_obj_add_flag(_obj, flag);
}

// 清除标志
auto Component::clear_flag(lv_obj_flag_t flag) -> void
{
    lv_obj_clear_flag(_obj, flag);
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
[[maybe_unused]] auto Component::move_to_background() -> void
{
    lv_obj_move_background(_obj);
}

// 重绘
auto Component::invalidate() -> void
{
    lv_obj_invalidate(_obj);  // 使频谱区域无效，触发重绘
}

// 设置对齐
auto Component::set_align(lv_align_t align, Coord x_offset, Coord y_offset) -> void
{
    lv_obj_align(_obj, align, x_offset, y_offset);
}

// 设置样式大小
auto Component::set_style_size(Coord w, Coord h, Selector selector) -> void
{
    lv_obj_set_style_size(_obj, w, h, selector);

}

// 设置背景颜色
auto Component::bg_color(Color color, Selector selector) -> void
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
auto Component::bg_grad_color(Color color, Selector selector) -> void
{
    lv_obj_set_style_bg_grad_color(_obj, color, selector);
}

// 设置边框宽度
auto Component::border_width(Coord width, Selector selector) -> void
{
    lv_obj_set_style_border_width(_obj, width, selector);
}

// 设置边框颜色
auto Component::border_color(Color color, Selector selector) -> void
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
[[maybe_unused]] auto Component::text_color(Color color, Selector selector) -> void
{
    lv_obj_set_style_text_color(_obj, color, selector);
}

// 设置文本透明度
[[maybe_unused]] auto Component::text_opa(uint8_t opa, Selector selector) -> void
{
    lv_obj_set_style_text_opa(_obj, opa, selector);
}

// 设置文本字体
[[maybe_unused]] auto Component::text_font(Font font, Selector selector) -> void
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

// 发送事件
auto Component::send_event(lv_event_code_t event, void *param) -> void
{
    lv_obj_send_event(_obj, event, param);
}


