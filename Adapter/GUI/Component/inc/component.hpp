//
// Created by fairy on 2024/10/17 19:48.
//
#ifndef SIMULATOR_COMPONENT_HPP
#define SIMULATOR_COMPONENT_HPP

// 头文件
#include "lvgl.h"


// 定义别名
using Obj = lv_obj_t *&;// 组件对象(用于给函数传参）
using Obj_t = lv_obj_t *;// 组件对象（用于定义组件）
using Coord = lv_coord_t;// 坐标
using Color = lv_color_t;// 颜色
using Selector = lv_style_selector_t;
using Align_text = lv_text_align_t;// 对齐类型
using Align = lv_align_t;
using Flag_t = lv_obj_flag_t;
using Grad_dir = lv_grad_dir_t;
using Border_side = lv_border_side_t;
using Font = const lv_font_t *;// 字体类型
using Strings = const char *;// 字符串
using ImageSrc = const lv_img_dsc_t *;// 图片资源
using ChartAxis = lv_chart_axis_t;// 图表坐标
using ChartSeries = lv_chart_series_t *&;
using ChartSeries_t = lv_chart_series_t *;
using ChartType = lv_chart_type_t;
using AnimExecCallback = lv_anim_exec_xcb_t;// 动画执行事件回调
using AnimCompletedCallback = lv_anim_completed_cb_t;// 动画执行完毕事件回调
using Anim = lv_anim_t &;
using Anim_t = lv_anim_t;
using AnimPath = lv_anim_path_cb_t;


// 色彩
#define Color_Firefly_Green  lv_color_make(133, 238, 223)// 流萤绿
#define Color_Firefly_Gold   lv_color_make(239, 221, 121)// 流萤金


// 定义常量
constexpr const Selector selector_default = (static_cast<uint32_t >(LV_PART_MAIN) |
                                             static_cast<uint32_t >(LV_STATE_DEFAULT));
constexpr const Selector selector_ticks = (static_cast<uint32_t >(LV_STATE_DEFAULT));


using Event_t = lv_event_t *;
using EventCode = lv_event_code_t;
using Event_Handler = lv_event_cb_t;
using Timer_t = lv_timer_t *;


//******************出于某种简化的目的，我使用了非常邪恶的宏定义*******************//
/**
 * @brief void*类型的"语法糖"，用于生成匿名函数，简化函数定义过程.如果你要调用这个函数那么还需要在后面加上();
 * @param ... 函数体
 * @note 仿Koltin的lambda表达式，只能说宏定义是真滴强大
 * @example 假设现在有一个函数void bond(void*); 需要传一个void*的函数，那么
 *          正常调用：
 *              bond([]()
 *              {
 *              int a=0;
 *              a=a+2;
 *              })
 *
 *          使用宏：
 *           bond(fun(
 *           int a=0;
 *           a=a+2;
 *           ))
 *
 *           调用宏所代表的函数：
 *           fun(
 *           int a=0;
 *           a=a+2;
 *           )();
 *
 */

// 用于调用
#define FUN(...) [](){ __VA_ARGS__}()


// 用于定时器的状态事件回调
#define timer_fun(...) [](Timer_t){__VA_ARGS__}



/**
 * @brief 用于图片按钮的状态事件回调
 * @param handler stateHandler类型的函数形参
 * @details 你需要定义一个stateHandler类型的函数
 * @example 可以使用lambda表达式：
            state_fun([](bool check)
            {
                if(check)
                {
                 int a = 0;
                 a=a+2;
                }
                else
                {
                int a = 0;
                }
           });
 */
#define imgbtn_fun(handler) [](Event_t e){ \
if(lv_event_get_code(e)==LV_EVENT_CLICKED)\
{\
handler(lv_obj_has_state(lv_event_get_target_obj(e), LV_STATE_CHECKED));\
}\
}


// 按钮函数
#define btn_fun(...) [](Event_t e) \
{                                \
    if(lv_event_get_code(e)==LV_EVENT_CLICKED) \
    {                            \
        __VA_ARGS__                 \
    }                                \
}


#define imgbtn_fun2(press_fun, release_fun)     [](Event e){\
if(lv_event_get_code(e)==LV_EVENT_CLICKED)\
{\
if(lv_obj_has_state(lv_event_get_target_obj(e), LV_STATE_CHECKED))\
{\
press_fun\
}\
else\
{\
release_fun\
}\
}\
}

/**
 * @brief 控件基类，采用元编程+CRTP模式
 * @tparam Derived
 */
template<typename Derived>
class Widget
{
protected:
    Obj_t obj_ = nullptr;
    // 方便获取父对象，因为一般而言定义多个组件时往往只有一个公共付对象
    static inline Obj_t parent_ = nullptr;

public:
    Widget() = default;// 删除构造函数，避免在初始化lvgl之前创建lvgl对象
    ~Widget() = default;

    // 隐式转换为原生对象
    operator lv_obj_t *() const noexcept { return obj_; }

    // 没必要且极度危险！！
//    // 重载=运算符，相当于将原生对象赋值给组件对象
//    Derived &operator=(Obj_t obj)noexcept
//    {
//        obj_ = obj;
//        return static_cast<Derived &>(*this);
//    }

    // 返回父对象
    Obj get_parent() const noexcept { return parent_; }

    // 设置父对象
    static void parent(Obj parent)
    {
        parent_ = parent;
    }

    static void parent(Obj_t parent)
    {
        parent_ = parent;
    }

    // 基础属性链式方法
    Derived &size(Coord w, Coord h)
    {
        lv_obj_set_size(obj_, w, h);
        return static_cast<Derived &>(*this);
    }

    Derived &pos(Coord x, Coord y)
    {
        lv_obj_set_pos(obj_, x, y);
        return static_cast<Derived &>(*this);
    }

    Derived &pos_size(Coord x, Coord y, Coord w, Coord h)
    {
        pos(x, y);
        size(w, h);
        return static_cast<Derived &>(*this);
    }

    // 设置样式大小
    Derived &style_size(Coord w, Coord h, Selector selector = selector_default)
    {
        lv_obj_set_style_size(obj_, w, h, selector);
        return static_cast<Derived &>(*this);
    }

    // 设置背景颜色
    Derived &bg_color(Color color, Selector selector = selector_default)
    {
        lv_obj_set_style_bg_color(obj_, color, selector);
        return static_cast<Derived &>(*this);
    }

    // 设置背景透明度
    Derived &bg_opa(uint8_t opa, Selector selector = selector_default)
    {
        lv_obj_set_style_bg_opa(obj_, opa, selector);
        return static_cast<Derived &>(*this);
    }

    // 设置背景渐变颜色
    Derived &bg_grad_color(Color color, Selector selector = selector_default)
    {
        lv_obj_set_style_bg_grad_color(obj_, color, selector);
        return static_cast<Derived &>(*this);
    }

    // 设置背景渐变透明度
    Derived &bg_grad_opa(uint8_t opa, Selector selector = selector_default)
    {
        lv_obj_set_style_bg_grad_opa(obj_, opa, selector);
        return static_cast<Derived &>(*this);
    }

    // 设置背景渐变停止点
    Derived &bg_grad_stop(Coord stop, Selector selector = selector_default)
    {
        lv_obj_set_style_bg_grad_stop(obj_, stop, selector);
        return static_cast<Derived &>(*this);
    }

    // 设置背景渐变方向
    Derived &bg_grad_dir(Grad_dir dir, Selector selector = selector_default)
    {
        lv_obj_set_style_bg_grad_dir(obj_, dir, selector);
        return static_cast<Derived &>(*this);
    }

    // 设置边框宽度
    Derived &border_width(Coord w, Selector selector = selector_default)
    {
        lv_obj_set_style_border_width(obj_, w, selector);
        return static_cast<Derived &>(*this);
    }

    // 设置边框颜色
    Derived &border_color(Color color, Selector selector = selector_default)
    {
        lv_obj_set_style_border_color(obj_, color, selector);
        return static_cast<Derived &>(*this);
    }

    // 设置边框透明度
    Derived &border_opa(uint8_t opa, Selector selector = selector_default)
    {
        lv_obj_set_style_border_opa(obj_, opa, selector);
        return static_cast<Derived &>(*this);
    }

    // 设置边框侧
    Derived &border_side(Border_side side, Selector selector = selector_default)
    {
        lv_obj_set_style_border_side(obj_, side, selector);
        return static_cast<Derived &>(*this);
    }

    // 设置边框圆角半径
    Derived &border_radius(Coord radius, Selector selector = selector_default)
    {
        lv_obj_set_style_radius(obj_, radius, selector);
        return static_cast<Derived &>(*this);
    }

    // 设置线条宽度
    Derived &line_width(Coord w, Selector selector = selector_default)
    {
        lv_obj_set_style_border_width(obj_, w, selector);
        return static_cast<Derived &>(*this);
    }

    // 设置阴影宽度
    Derived &shadow_width(Coord w, Selector selector = selector_default)
    {
        lv_obj_set_style_shadow_width(obj_, w, selector);
        return static_cast<Derived &>(*this);
    }

    // 设置文本颜色
    Derived &text_color(Color color, Selector selector = selector_default)
    {
        lv_obj_set_style_text_color(obj_, color, selector);
        return static_cast<Derived &>(*this);
    }

    // 设置文本透明度
    Derived &text_opa(uint8_t opa, Selector selector = selector_default)
    {
        lv_obj_set_style_text_opa(obj_, opa, selector);
        return static_cast<Derived &>(*this);
    }

    // 设置文本字体
    Derived &text_font(Font font, Selector selector = selector_default)
    {
        lv_obj_set_style_text_font(obj_, font, selector);
        return static_cast<Derived &>(*this);
    }

    /**
    * @brief 设置背景渐变
    * @param color 背景起始颜色
    * @param grad_color 背景渐变颜色
    * @param opa 背景透明度
    * @param grad_dir 渐变方向
    */
    Derived &
    bg_grad(Color color, Color grad_color, uint8_t opa, Grad_dir grad_dir, Selector selector = selector_default)
    {
        bg_color(color, selector);
        bg_opa(opa, selector);
        bg_grad_dir(grad_dir, selector);// 设置垂直渐变
        bg_grad_color(grad_color, selector);// 设置渐变结束颜色
        return static_cast<Derived &>(*this);
    }

    /**
    * @brief 设置边框样式
    * @param color   边框颜色
    * @param radius  边框圆角半径
    * @param width   边框宽度
    * @param opa     边框透明度
    */
    Derived &border(Color color, Coord radius, Coord width, uint8_t opa=255, Selector selector = selector_default)
    {
        border_color(color, selector);
        border_radius(radius, selector);
        border_width(width, selector);
        border_opa(opa, selector);
        return static_cast<Derived &>(*this);
    }

    // 将对象移动到背景层
    Derived &to_background()
    {
        lv_obj_move_background(obj_);
        return static_cast<Derived &>(*this);
    }

    // 将对象移动到前景层
    Derived &to_foreground()
    {
        lv_obj_move_foreground(obj_);
        return static_cast<Derived &>(*this);
    }

    // 高级布局方法
    Derived &align(Align align, Coord x_ofs = 0,
                   Coord y_ofs = 0)
    {
        lv_obj_align(obj_, align, x_ofs, y_ofs);
        return static_cast<Derived &>(*this);
    }

    // 对象居中
    Derived &center(Coord x_ofs = 0, Coord y_ofs = 0)
    {
        lv_obj_align(obj_, LV_ALIGN_CENTER, x_ofs, y_ofs);
        return static_cast<Derived &>(*this);
    }

    // 样式系统（支持普通样式和本地样式）
    template<typename Style>
    Derived &add_style(Style &&style, Selector selector = selector_default)
    {
        lv_obj_add_style(obj_, style.get(), selector);
        return static_cast<Derived &>(*this);
    }

    Derived &remove_style_all()
    {
        lv_obj_remove_style_all(obj_);
        return static_cast<Derived &>(*this);
    }

    // 圆角裁剪
    Derived &clip_corner(Selector selector = selector_default)
    {
        lv_obj_set_style_clip_corner(obj_, true, selector);
        return static_cast<Derived &>(*this);
    }

    // 重绘
    Derived &invalidate()
    {
        lv_obj_invalidate(obj_);  // 使频谱区域无效，触发重绘
        return static_cast<Derived &>(*this);
    };

    // 事件系统（支持17种标准事件）
    template<EventCode Event=LV_EVENT_ALL>
    Derived &bind_event(Event_Handler handler)
    {
        lv_obj_add_event_cb(obj_, handler, Event, this);
        return static_cast<Derived &>(*this);
    }

    /**
     * @brief 移除事件
     * @param handler 即当初绑定的事件回调函数
     * @return
     */
    Derived &remove_event(Event_Handler &handler)
    {
        lv_obj_remove_event_cb(obj_, handler);
        return static_cast<Derived &>(*this);
    }

    /**
     * @brief 移除事件
     * @tparam index 事件的索引
     * @return
     */
    template<uint32_t Index>
    Derived &remove_event()
    {
        lv_obj_remove_event(obj_, Index);
        return static_cast<Derived &>(*this);
    }


    // 发送事件
    template<EventCode Event, void *param = nullptr>
    Derived &send_event()
    {
        lv_obj_send_event(obj_, Event, param);
        return static_cast<Derived &>(*this);
    }

    // 状态管理方法
    Derived &add_flag(Flag_t f)
    {
        lv_obj_add_flag(obj_, f);
        return static_cast<Derived &>(*this);
    }

    Derived &clear_flag(Flag_t f)
    {
        lv_obj_clear_flag(obj_, f);
        return static_cast<Derived &>(*this);
    }

    Derived &appear()
    {
        clear_flag(LV_OBJ_FLAG_HIDDEN);
        return static_cast<Derived &>(*this);
    }

    Derived &hidden()
    {
        add_flag(LV_OBJ_FLAG_HIDDEN);
        return static_cast<Derived &>(*this);
    }

    // 动画系统
    template<auto Prop, typename T>
    Derived &animate(T start, T end, uint16_t time,
                     lv_anim_path_cb_t path = lv_anim_path_linear)
    {
        lv_anim_t a;
        lv_anim_init(&a);
        lv_anim_set_exec_cb(&a, [](void *var, int32_t v)
        {
            (static_cast<Derived *>(var)->*Prop) = v;
        });
        lv_anim_set_var(&a, this);
        lv_anim_start(&a);
        return static_cast<Derived &>(*this);
    }

protected:
    // 公共初始化模板（供派生类调用）
#warning "需要传入一个obj对象，给parent使用"
    void create_obj(const lv_obj_class_t *cls)
    {
        obj_ = lv_obj_class_create_obj(cls, parent_);
        lv_obj_class_init_obj(obj_);
    }
};


/**
 * @brief 空白组件类
 * @note 需要自己去实现init函数
 */
class Component : public Widget<Component>
{
public:
    // 极度危险！！
    //    // 显式引入基类运算符
    //    using Widget<Component>::operator=;

    // 只能调用一次，不然会出现内存泄漏
    // 不主动设置父对象，那么就默认为上一次调用set_parent函数设置的父对象
    Component &init(Obj parent = parent_)
    {
        obj_ = lv_obj_create(parent);
        return *this;
    }

    Component &init(Coord x, Coord y, Coord w, Coord h, Obj parent = parent_)
    {
        obj_ = lv_obj_create(parent);
        pos_size(x, y, w, h);
        return *this;
    }

};


#endif //SIMULATOR_COMPONENT_HPP