//
// Created by fairy on 2024/10/17 19:48.
//
#ifndef SIMULATOR_COMPONENT_HPP
#define SIMULATOR_COMPONENT_HPP

// 头文件
#include "lvgl.h"


// 定义别名
using Obj = _lv_obj_t *&;// 组件对象(用于给函数传参）
using Obj_t = _lv_obj_t *;// 组件对象（用于定义组件）
using Coord = lv_coord_t;// 坐标
using Color = lv_color_t;// 颜色
using Selector = lv_style_selector_t;
using Align = lv_text_align_t;// 对齐类型
using Font = const lv_font_t *;// 字体类型
using Strings = const char *;// 字符串
using ImageSrc = const lv_img_dsc_t *;// 图片资源
using ChartAxis = lv_chart_axis_t;// 图表坐标
using ChartSeries = lv_chart_series_t*&;
using ChartSeries_t = lv_chart_series_t*;
using ChartType = lv_chart_type_t;


// 色彩
#define Color_Firefly_Green  lv_color_make(133, 238, 223)// 流萤绿
#define Color_Firefly_Gold   lv_color_make(239, 221, 121)// 流萤金


// 定义常量
constexpr const Selector selector_default = (static_cast<uint32_t >(LV_PART_MAIN) |
                                             static_cast<uint32_t >(LV_STATE_DEFAULT));
constexpr const Selector selector_ticks = (static_cast<uint32_t >(LV_STATE_DEFAULT));


/**
 * @brief 组件初始化基类
 * @note 需要自己去实现create函数.若要使用其他组件静态类，需要自行调用Component类的相关接口来设置parent和object
 */
class Component
{
public:
    // 设置父对象
    static auto set_parent(Obj parent) -> void;

public:
    // 初始化自定义组件
    auto init() -> void;

    auto init(Coord x, Coord y) -> void;

    auto init(Coord x, Coord y, Coord w, Coord h) -> void;

    // 把自身添加到父对象中，供其它组件使用，不过记得更改回去
    auto set_self_to_parent()->void{ set_parent(_obj);}
    auto get_object()->Obj_t {return _obj;}

    // 设置位置和尺寸
    auto set_pos_size(Coord x, Coord y, Coord w, Coord h) -> void;// 设置位置和尺寸
    auto set_pos(Coord x, Coord y) -> void;// 设置位置
    auto set_size(Coord w, Coord h) -> void;// 设置尺寸

    // 显示组件和隐藏组件
    auto appear() -> void;

    auto hidden() -> void;

    // 清除标志、添加标志
    auto clear_flag(lv_obj_flag_t flag) -> void;

    [[maybe_unused]] auto add_flag(lv_obj_flag_t flag) -> void;

    // 设置为裁剪属性，即子对象的内容会被父对象的圆角部分裁剪掉，而不会超出父对象的圆角边界。
    [[maybe_unused]] auto set_clip_corner(Selector selector = selector_default) -> void;

    // 移除所有样式,没有圆角方框
    [[maybe_unused]] auto remove_all_style() -> void;

    // 移动到图层底部
    [[maybe_unused]] auto move_to_background() -> void;

    // 重绘
    auto invalidate() -> void;

    // 设置对齐
    auto
    set_align(lv_align_t align, Coord x_offset = 0, Coord y_offset = 0) -> void;

    // 设置样式大小
    auto set_style_size(Coord w, Coord h, Selector selector) -> void;

    // 设置背景颜色
    auto
    bg_color(Color color, Selector selector = selector_default) -> void;

    // 设置背景透明度
    auto bg_opa(uint8_t opa, Selector selector = selector_default) -> void;

    // 设置背景渐变方向
    auto
    bg_grad_dir(lv_grad_dir_t dir, Selector selector = selector_default) -> void;

    // 设置背景渐变颜色
    auto
    bg_grad_color(Color color, Selector selector = selector_default) -> void;

    // 设置边框宽度
    auto
    border_width(Coord width, Selector selector = selector_default) -> void;

    // 设置边框颜色
    auto
    border_color(Color color, Selector selector = selector_default) -> void;

    // 设置边框透明度
    auto
    border_opa(uint8_t opa, Selector selector = selector_default) -> void;

    // 设置边框xxx
    auto
    border_side(lv_border_side_t side = LV_BORDER_SIDE_FULL, Selector selector = selector_default) -> void;

    // 设置边框圆角半径
    auto
    border_radius(Coord radius, Selector selector = selector_default) -> void;

    // 设置线条宽度
    auto
    line_width(Coord width, Selector selector = selector_default) -> void;

    // 设置阴影宽度
    auto
    shadow_width(Coord width, Selector selector = selector_default) -> void;

    // 设置文本颜色,一般用于图表或刻度盘，现在失效了
    [[maybe_unused]] auto
    text_color(Color color, Selector selector = selector_default) -> void;

    // 设置文本透明度,一般用于图表或刻度盘
    [[maybe_unused]] auto text_opa(uint8_t opa, Selector selector = selector_ticks) -> void;

    // 设置文本字体,一般用于图表或刻度盘
    [[maybe_unused]] auto
    text_font(Font font, Selector selector = selector_ticks) -> void;

    // 设置背景渐变
    auto
    bg_grad(Color color, Color grad_color, uint8_t opa = 255, lv_grad_dir_t grad_dir = LV_GRAD_DIR_HOR) -> void;

    // 设置边框样式
    auto
    border(Color color, Coord radius = 5, Coord width = 1, uint8_t opa = 255) -> void;


    // 发送事件
    auto send_event(lv_event_code_t event,void *param = nullptr) -> void;


protected:
    Obj_t _obj{};
    static inline Obj_t _parent;// 共有的父对象
};


#endif //SIMULATOR_COMPONENT_HPP