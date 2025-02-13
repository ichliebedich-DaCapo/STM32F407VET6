//
// Created by fairy on 2025/2/9 13:37.
//
#ifndef SIMULATOR_WIDGETS_HPP
#define SIMULATOR_WIDGETS_HPP
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
using Font_t = const lv_font_t;
using Strings = const char *;// 字符串
using ImageSrc = const lv_img_dsc_t *;// 图片资源
using ImageSrc_t = const lv_img_dsc_t;
using ChartAxis = lv_chart_axis_t;// 图表坐标
using ChartSeries = lv_chart_series_t *&;
using ChartSeries_t = lv_chart_series_t *;
using Scrollbar_mode =lv_scrollbar_mode_t;
using ImageButton_State = lv_imagebutton_state_t;
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


/**
 * @brief 控件基类的数据模型，为了防止实例化多个静态变量而设计的
 */
class WidgetModel
{
protected:
    Obj_t obj_;
    // 方便获取父对象，因为一般而言定义多个组件时往往只有一个公共付对象
    static inline Obj_t parent_ = nullptr;
};


/**
 * @brief 控件基类，采用元编程+CRTP模式
 * @tparam Derived
 */
template<typename Derived>
class Widget : public WidgetModel
{
public:
    Widget() = default;// 删除构造函数，避免在初始化lvgl之前创建lvgl对象
    ~Widget() = default;

    // 隐式转换为原生对象
    operator Obj_t() const noexcept { return obj_; }


    // 返回的是该组件的父对象（不是parent_)
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
    inline Derived &size(Coord w, Coord h)
    {
        lv_obj_set_size(obj_, w, h);
        return static_cast<Derived &>(*this);
    }

    inline Derived &pos(Coord x, Coord y)
    {
        lv_obj_set_pos(obj_, x, y);
        return static_cast<Derived &>(*this);
    }

    inline Derived &width(int32_t w)
    {
        lv_obj_set_width(obj_, w);
        return static_cast<Derived &>(*this);
    }

    inline Derived &pos_size(Coord x, Coord y, Coord w, Coord h)
    {
        pos(x, y);
        size(w, h);
        return static_cast<Derived &>(*this);
    }

    // 设置样式大小
    inline Derived &style_size(Coord w, Coord h, Selector selector = selector_default)
    {
        lv_obj_set_style_size(obj_, w, h, selector);
        return static_cast<Derived &>(*this);
    }

    // 设置背景颜色
    inline Derived &bg_color(Color color, Selector selector = selector_default)
    {
        lv_obj_set_style_bg_color(obj_, color, selector);
        return static_cast<Derived &>(*this);
    }

    // 设置背景透明度
    inline Derived &bg_opa(uint8_t opa, Selector selector = selector_default)
    {
        lv_obj_set_style_bg_opa(obj_, opa, selector);
        return static_cast<Derived &>(*this);
    }

    // 设置背景渐变颜色
    inline Derived &bg_grad_color(Color color, Selector selector = selector_default)
    {
        lv_obj_set_style_bg_grad_color(obj_, color, selector);
        return static_cast<Derived &>(*this);
    }

    // 设置背景渐变透明度
    inline Derived &bg_grad_opa(uint8_t opa, Selector selector = selector_default)
    {
        lv_obj_set_style_bg_grad_opa(obj_, opa, selector);
        return static_cast<Derived &>(*this);
    }
    // 设置背景主停止点
    inline Derived &bg_main_stop(Coord start, Selector selector = selector_default)
    {
        lv_obj_set_style_bg_main_stop(obj_, start, selector);
        return static_cast<Derived &>(*this);
    }

    // 设置背景渐变停止点
    inline Derived &bg_grad_stop(Coord stop, Selector selector = selector_default)
    {
        lv_obj_set_style_bg_grad_stop(obj_, stop, selector);
        return static_cast<Derived &>(*this);
    }

    // 设置背景渐变方向
    inline Derived &bg_grad_dir(Grad_dir dir, Selector selector = selector_default)
    {
        lv_obj_set_style_bg_grad_dir(obj_, dir, selector);
        return static_cast<Derived &>(*this);
    }

    // 设置边框宽度
    inline Derived &border_width(Coord w, Selector selector = selector_default)
    {
        lv_obj_set_style_border_width(obj_, w, selector);
        return static_cast<Derived &>(*this);
    }

    // 设置边框颜色
    inline Derived &border_color(Color color, Selector selector = selector_default)
    {
        lv_obj_set_style_border_color(obj_, color, selector);
        return static_cast<Derived &>(*this);
    }

    // 设置边框透明度
    inline Derived &border_opa(uint8_t opa, Selector selector = selector_default)
    {
        lv_obj_set_style_border_opa(obj_, opa, selector);
        return static_cast<Derived &>(*this);
    }

    // 设置边框侧
    inline Derived &border_side(Border_side side, Selector selector = selector_default)
    {
        lv_obj_set_style_border_side(obj_, side, selector);
        return static_cast<Derived &>(*this);
    }

    // 设置边框圆角半径
    inline Derived &border_radius(Coord radius, Selector selector = selector_default)
    {
        lv_obj_set_style_radius(obj_, radius, selector);
        return static_cast<Derived &>(*this);
    }

    // 设置线条宽度
    inline Derived &line_width(Coord w, Selector selector = selector_default)
    {
        lv_obj_set_style_border_width(obj_, w, selector);
        return static_cast<Derived &>(*this);
    }

    // 设置阴影宽度
    inline Derived &shadow_width(Coord w, Selector selector = selector_default)
    {
        lv_obj_set_style_shadow_width(obj_, w, selector);
        return static_cast<Derived &>(*this);
    }

    inline Derived& image_recolor_opa(uint8_t opa=0, Selector selector = selector_default)
    {
        lv_obj_set_style_image_recolor_opa(obj_, opa, selector);
        return static_cast<Derived &>(*this);
    }

    inline Derived& image_opa(uint8_t opa=0, Selector selector = selector_default)
    {
        lv_obj_set_style_image_opa(obj_, opa, selector);
        return static_cast<Derived &>(*this);
    }

    // 设置字间距
    inline Derived &letter_space(Coord space, Selector selector = selector_default)
    {
        lv_obj_set_style_text_letter_space(obj_, space, selector);
        return static_cast<Derived &>(*this);
    }

    // 设置字体颜色
    inline Derived &text_color(Color color, Selector selector = selector_default)
    {
        lv_obj_set_style_text_color(obj_, color, selector);
        return static_cast<Derived &>(*this);
    }

    // 设置字体格式
    inline Derived &font(::Font font, Selector selector = selector_default)
    {
        lv_obj_set_style_text_font(obj_, font, selector);
        return static_cast<Derived &>(*this);
    }

    // 设置字体透明度
    inline Derived &text_opa(uint8_t opa, Selector selector = selector_default)
    {
        lv_obj_set_style_text_opa(obj_, opa, selector);
        return static_cast<Derived &>(*this);
    }

    /**
    * @brief 设置背景渐变
    * @param color 背景起始颜色
    * @param grad_color 背景渐变颜色
    * @param opa 背景透明度
    * @param grad_dir 渐变方向
    */
    inline Derived &
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
    inline Derived &
    border(Color color, Coord radius, Coord width, uint8_t opa = 255, Selector selector = selector_default)
    {
        border_color(color, selector);
        border_radius(radius, selector);
        border_width(width, selector);
        border_opa(opa, selector);
        return static_cast<Derived &>(*this);
    }

    // 将对象移动到背景层
    inline Derived &to_background()
    {
        lv_obj_move_background(obj_);
        return static_cast<Derived &>(*this);
    }

    // 将对象移动到前景层
    inline Derived &to_foreground()
    {
        lv_obj_move_foreground(obj_);
        return static_cast<Derived &>(*this);
    }

    // 高级布局方法
    inline Derived &align(Align align, Coord x_ofs = 0,
                          Coord y_ofs = 0)
    {
        lv_obj_align(obj_, align, x_ofs, y_ofs);
        return static_cast<Derived &>(*this);
    }

    // 对象居中
    inline Derived &center(Coord x_ofs = 0, Coord y_ofs = 0)
    {
        lv_obj_align(obj_, LV_ALIGN_CENTER, x_ofs, y_ofs);
        return static_cast<Derived &>(*this);
    }

    // 设置半径
    inline Derived &radius(Coord radius, Selector selector = selector_default)
    {
        lv_obj_set_style_radius(obj_, radius, selector);
        return static_cast<Derived &>(*this);
    }

    inline Derived &pad_all(int32_t pad, Selector selector = selector_default)
    {
        lv_obj_set_style_pad_all(obj_, pad, selector);
        return static_cast<Derived  &>(*this);
    }

    // 样式系统（支持普通样式和本地样式）
    template<typename Style>
    inline Derived &add_style(Style &&style, Selector selector = selector_default)
    {
        lv_obj_add_style(obj_, style.get(), selector);
        return static_cast<Derived &>(*this);
    }

    inline Derived &remove_style_all()
    {
        lv_obj_remove_style_all(obj_);
        return static_cast<Derived &>(*this);
    }

    // 圆角裁剪
    inline Derived &clip_corner(Selector selector = selector_default)
    {
        lv_obj_set_style_clip_corner(obj_, true, selector);
        return static_cast<Derived &>(*this);
    }

    // 滚动条
    inline Derived &scrollbar_mode(Scrollbar_mode mode)
    {
        lv_obj_set_scrollbar_mode(obj_, mode);
        return static_cast<Derived &>(*this);
    }


    // 重绘
    inline Derived &invalidate()
    {
        lv_obj_invalidate(obj_);  // 使频谱区域无效，触发重绘
        return static_cast<Derived &>(*this);
    };


    /**
     * @brief 绑定事件
     * @tparam handler 给默认参数nullptr是为了防止刚写就报错
     * @tparam Event 事件类型，默认为全部
     * @note 出于大多数情况的考虑，函数一般都是定义好的，所以用模板参数来传递
     * @return
     */
    template<Event_Handler handler = nullptr, EventCode Event = LV_EVENT_ALL>
    inline Derived &bind_event(Event_Handler lambda_handler = nullptr)
    {
        if constexpr (handler != nullptr)
            lv_obj_add_event_cb(obj_, handler, Event, nullptr);
        else
        {
            lv_obj_add_event_cb(obj_, lambda_handler, Event, nullptr);
        }
        return static_cast<Derived &>(*this);
    }

    /**
     * @brief 移除事件
     * @param handler 即当初绑定的事件回调函数
     * @return
     */
    inline Derived &remove_event(Event_Handler &handler)
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
    inline Derived &remove_event()
    {
        lv_obj_remove_event(obj_, Index);
        return static_cast<Derived &>(*this);
    }


    // 发送事件
    template<EventCode Event, void *param = nullptr>
    inline Derived &send_event()
    {
        lv_obj_send_event(obj_, Event, param);
        return static_cast<Derived &>(*this);
    }

    // 绑定点击事件
    template<void(*handler)() = nullptr>
    inline Derived &OnClicked(Event_Handler lambda_handler = nullptr)
    {

        if constexpr (handler != nullptr)
        {
            bind_event<[](Event_t e)
            {
                if (lv_event_get_code(e) == LV_EVENT_CLICKED)
                {
                    handler();
                }
            }, LV_EVENT_CLICKED>();
        }
        else
        {
            bind_event<nullptr, LV_EVENT_CLICKED>(lambda_handler);
        }

        return static_cast<Derived &>(*this);
    }

    template<void(*handler)(Event_t e) = nullptr>
    inline Derived &OnClicked()
    {
        bind_event<handler, LV_EVENT_CLICKED>();
        return static_cast<Derived &>(*this);
    }


    // 持续按压事件绑定
    template<void(*press)() = nullptr>
    inline Derived &OnPressed()
    {
        bind_event<[](Event_t e)
        {
            if (lv_event_get_code(e) == LV_EVENT_PRESSED)
            {
                press();
            }
        }, LV_EVENT_PRESSED>();

        return static_cast<Derived &>(*this);
    }

    template<void(*pressing)() = nullptr>
    inline Derived &OnPressing()
    {
        bind_event<[](Event_t e)
        {
            if (lv_event_get_code(e) == LV_EVENT_PRESSING)
            {
                pressing();
            }
        }, LV_EVENT_PRESSING>();

        return static_cast<Derived &>(*this);
    }

    template<void(*press)() = nullptr, void(*release)() = nullptr>
    inline Derived &OnPressedReleased()
    {
        bind_event<[](Event_t e)
        {
            if (lv_event_get_code(e) == LV_EVENT_CLICKED)
            {
                if (lv_obj_has_state(lv_event_get_target_obj(e), LV_STATE_CHECKED))
                {
                    press();
                }
                else
                {
                    release();
                }
            }
        }, LV_EVENT_CLICKED>();

        return static_cast<Derived &>(*this);
    }


    // 状态管理方法
    inline Derived &add_flag(Flag_t f)
    {
        lv_obj_add_flag(obj_, f);
        return static_cast<Derived &>(*this);
    }

    inline Derived &clear_flag(Flag_t f)
    {
        lv_obj_clear_flag(obj_, f);
        return static_cast<Derived &>(*this);
    }

    inline Derived& checkable()
    {
        add_flag(LV_OBJ_FLAG_CHECKABLE);
        return static_cast<Derived &>(*this);
    }

    inline Derived &appear()
    {
        clear_flag(LV_OBJ_FLAG_HIDDEN);
        return static_cast<Derived &>(*this);
    }

    inline Derived &hidden()
    {
        add_flag(LV_OBJ_FLAG_HIDDEN);
        return static_cast<Derived &>(*this);
    }

// 更安全的动画实现（暂不可用）
    template<typename T>
    Derived &animate(lv_anim_exec_xcb_t exec_cb, T start, T end, uint16_t time,
                     lv_anim_path_cb_t path = lv_anim_path_linear)
    {
        lv_anim_t a;
        lv_anim_init(&a);
        lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t) exec_cb);
        lv_anim_set_var(&a, obj_); // 直接操作LVGL对象
        lv_anim_set_values(&a, start, end);
        lv_anim_set_time(&a, time);
        lv_anim_set_path_cb(&a, path);
        lv_anim_start(&a);
        return static_cast<Derived &>(*this);
    }

protected:
    // 公共初始化模板（供派生类调用）
    void inline create_obj(const lv_obj_class_t *cls, Obj parent = parent_)
    {
        obj_ = lv_obj_class_create_obj(cls, parent);
        lv_obj_class_init_obj(obj_);
    }
};


#endif //SIMULATOR_WIDGETS_HPP