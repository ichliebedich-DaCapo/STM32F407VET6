//
// Created by fairy on 2024/10/17 18:53.
//
#ifndef FURINA_GUI_HPP
#define FURINA_GUI_HPP

#include <cstdio>

/* 预编译命令 */
#if 1
// 头文件
#include "LV_Timer.hpp"
#include "events.hpp"
#include "component.hpp"
#include "chart.hpp"
#include "slider.hpp"
#include "image.hpp"
#include "text.hpp"
#include "button.hpp"
#include "imageButton.hpp"
// 宏定义
#define SIMPLE_FPS 1 // 启用简单FPS，减少一点点FPS显示对实际帧率的影响。不严谨地对于某个简单的测试来说，从30ms减少到了21ms

/*匿名命名空间，专治各种函数变量暴露狂*/
//extern lv_disp_drv_t disp_drv;

namespace
{
    constexpr uint16_t MY_DISP_HOR_RES = 480;
    constexpr uint16_t MY_DISP_VER_RES = 320;
    constexpr uint16_t MY_DISP_BUF_SIZE = 20;

    lv_disp_drv_t disp_drv{};
}
static inline auto LVGL_LCD_FSMC_DMA_pCallback() -> void
{
    disp_drv.draw_buf->flushing = 0;
    disp_drv.draw_buf->flushing_last = 0;
}



/**
 * @brief GUI类
 */
class GUI : public GUI_Base
{
public:
    template<void (*disp_flush)(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, const uint16_t *color_p)>
    static auto init() -> void;

    static inline auto handler()->void ;

    // 刷新回调
    static inline auto LVGL_LCD_FSMC_DMA_pCallback()->void;

private:
    static auto resource_init() -> void;// 初始化界面

    template<void (*flush)(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, const uint16_t *color_p)>
    static inline auto disp_drv_init() -> void;
private:
   static inline lv_disp_drv_t disp_drv{};
};


/**
 * @brief GUI事件循环
 */
auto GUI::handler() -> void
{
    lv_task_handler();
}


/**
 * @brief 回调函数
 * @note 用于DMA等刷新完成后通知LVGL
 */
auto GUI::LVGL_LCD_FSMC_DMA_pCallback() -> void
{
    disp_drv.draw_buf->flushing = 0;
    disp_drv.draw_buf->flushing_last = 0;
}


/**
 * @brief 初始化显示驱动
 * @tparam flush 涂色函数，有LCD驱动提供
 * @note 为了让lambda表达式可以不用捕获外部函数，只能使用函数模板。如果使用函数指针来传递就必须要显示捕获
 */
template<void (*flush)(uint16_t, uint16_t, uint16_t, uint16_t, const uint16_t *)>
auto GUI::disp_drv_init() -> void
{
    // 在缓冲数组总大小同等的情况下，双缓冲明显优于单缓冲
    static lv_disp_draw_buf_t draw_buf_dsc;
    static lv_color_t buf_2_1[MY_DISP_HOR_RES * MY_DISP_BUF_SIZE];
    static lv_color_t buf_2_2[MY_DISP_HOR_RES * MY_DISP_BUF_SIZE];
    lv_disp_draw_buf_init(&draw_buf_dsc, buf_2_1, buf_2_2,
                          MY_DISP_HOR_RES * MY_DISP_BUF_SIZE);   /*Initialize the display buffer*/

    lv_disp_drv_init(&disp_drv);                    /*Basic initialization*/

    disp_drv.hor_res = MY_DISP_HOR_RES;
    disp_drv.ver_res = MY_DISP_VER_RES;
    disp_drv.flush_cb = [](lv_disp_drv_t *, const lv_area_t *area, lv_color_t *color_p)
    {
        flush(area->x1, area->y1, area->x2, area->y2, (const uint16_t *) color_p);
    };
    disp_drv.draw_buf = &draw_buf_dsc;

    lv_disp_drv_register(&disp_drv);
}


template<void (*disp_flush)(uint16_t, uint16_t, uint16_t, uint16_t, const uint16_t *)>
auto GUI::init() -> void
{
    /*****初始化设备*****/
#ifdef ARM_MATH_CM4
    disp_drv_init<disp_flush>();
#endif

    /*****初始化GUI组件*****/
    GUI::resource_init();
}

/**
 * @brief 工具类
 */
class Tools
{
public:
    static inline auto fps_init(Font font, Coord x = 0, Coord y = 40, Coord width = 60, Coord height = 20) -> void;

    // 显示fps
    static inline auto fps(bool time = true) -> void;

    static inline auto restart_fps() -> void;

    static inline auto set_right() -> void;

    static inline auto set_left() -> void;

    static inline auto set_center() -> void;

    static inline auto clear_fps() -> void;

private:
    // 获取时间
    static inline auto get_tick() -> uint32_t;

    // 单线程更新事件
    static inline auto update_tick() -> void;

private:
    static inline Obj_t label_fps{};
    static inline uint32_t count = 0;
    static inline uint32_t tick = 0;
};

/**
 * @brief fps功能初始化
 * @param font 指定字库中要有fps和十位数字，字体大小为13即可
 * @param x x轴
 * @param y y轴
 * @note 默认文本框为60*20，即宽60，高20，且文本为左对齐。
 */
auto Tools::fps_init(Font font, Coord x, Coord y, Coord width, Coord height) -> void
{
    Text label;
    label.init_font(font);
#if SIMPLE_FPS
    label.init(label_fps, x, y, width, height, "");
#else
    label.init(label_fps, x, y, 60, 80, "fps\n0");
#endif


}

/**
 * @brief fps显示
 * @note 启用该功能之前必须先调用fps_init进行必要的初始化。启用fps显示，即在需要的地方调用本函数
 *      默认显示一帧需要的时间单位为ms
 */
auto Tools::fps(bool time) -> void
{
#if SIMPLE_FPS
    char buf[7];

    if (time)
    {
        // 显示一帧的时间
        sprintf(buf, "%.2fms", 1.0 * get_tick() / (count++));
    } else
    {
        // 显示帧率
        sprintf(buf, "%.2f", 1000.0 * (count++) / get_tick());
    }
    Text::set_text(buf, label_fps);
#else
    char buf[9];
    // 显示一帧的时间
    sprintf(buf, "fps\n%.2f", 1.0*get_tick() / (count++));
    // 显示帧率
//    sprintf(buf, "fps\n%.2f", 1000.0*(count++))/get_tick();
    Text::set_text(buf, label_fps);
#endif
}

auto Tools::get_tick() -> uint32_t
{
    uint32_t temp_tick = lv_tick_get();
    // 防止溢出
    if (temp_tick < tick)
        temp_tick += (0xFFFF'FFFF - tick);
    else
        temp_tick -= tick;

    return temp_tick;
}

auto Tools::update_tick() -> void
{
    tick = lv_tick_get();
}

/**
 * @brief 重启fps
 */
auto Tools::restart_fps() -> void
{
    update_tick();
    count = 0;
}

auto Tools::set_right() -> void
{
    Text::set_text_align(LV_TEXT_ALIGN_RIGHT, label_fps);
}

auto Tools::set_center() -> void
{
    Text::set_text_align(LV_TEXT_ALIGN_CENTER, label_fps);
}

auto Tools::set_left() -> void
{
    Text::set_text_align(LV_TEXT_ALIGN_LEFT, label_fps);
}

auto Tools::clear_fps() -> void
{
    Text::set_text("", label_fps);

}


#endif
#endif //FURINA_GUI_HPP