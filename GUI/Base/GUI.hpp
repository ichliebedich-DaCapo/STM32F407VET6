//
// Created by fairy on 2024/10/17 18:53.
//
#ifndef FURINA_GUI_HPP
#define FURINA_GUI_HPP

#include <cstdio>
#include "JYZQ_Conf.h"
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

/**
 * @brief GUI类
 */
class GUI : public GUI_Base
{
public:
    static auto init() -> void;// 初始化界面
};

#ifdef APP_NO_RTOS

inline void GUI_handler()
{
    lv_task_handler();
}

#else
#define GUI_handler() void(0)
#endif

/**
 * @brief 工具类
 */
class Tools
{
public:
    static inline auto fps_init(Font font, Coord x = 0, Coord y = 40) -> void;

    // 显示fps
    static inline auto fps(bool time = true) -> void;

    static inline auto restart_fps() -> void;

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
 */
auto Tools::fps_init(Font font, Coord x, Coord y) -> void
{
    Text label;
    label.init_font(font);
#if SIMPLE_FPS
    label.init(label_fps, x, y, 60, 20, "0");
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
    char buf[5];

    if (time)
    {
        // 显示一帧的时间
        sprintf(buf, "%.2f", 1.0 * get_tick() / (count++));
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


#endif
#endif //FURINA_GUI_HPP