//
// Created by fairy on 2024/11/9 18:14.
//
#ifndef SIMULATOR_FPS_HPP
#define SIMULATOR_FPS_HPP

#include "text.hpp"

#define SIMPLE_FPS 1// 使用简单模式

/**
 * @brief 工具类
 */
class FPS
{
public:
    static inline auto init(Font font, Coord x = 0, Coord y = 40, Coord width = 60, Coord height = 20) -> void;

    // 显示fps
    static inline auto print(bool time = true) -> void;

    static inline auto restart() -> void;

    static inline auto set_right() -> void;

    static inline auto set_left() -> void;

    static inline auto set_center() -> void;

    static inline auto clear() -> void;

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
auto FPS::init(Font font, Coord x, Coord y, Coord width, Coord height) -> void
{
    Text label;
    label.init_font(font);
#if SIMPLE_FPS
    label.init(label_fps, x, y, width, height, "");
#else
    label.init(label_fps, x, y, 60, 80, "print\n0");
#endif


}


/**
 * @brief fps显示
 * @note 启用该功能之前必须先调用fps_init进行必要的初始化。启用fps显示，即在需要的地方调用本函数
 *      默认显示一帧需要的时间单位为ms
 */
auto FPS::print(bool time) -> void
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
    sprintf(buf, "print\n%.2f", 1.0*get_tick() / (count++));
    // 显示帧率
//    sprintf(buf, "print\n%.2f", 1000.0*(count++))/get_tick();
    Text::set_text(buf, label_fps);
#endif
}

auto FPS::get_tick() -> uint32_t
{
    uint32_t temp_tick = lv_tick_get();
    // 防止溢出
    if (temp_tick < tick)
        temp_tick += (0xFFFF'FFFF - tick);
    else
        temp_tick -= tick;

    return temp_tick;
}

auto FPS::update_tick() -> void
{
    tick = lv_tick_get();
}

/**
 * @brief 重启fps
 */
auto FPS::restart() -> void
{
    update_tick();
    count = 0;
}

auto FPS::set_right() -> void
{
    Text::set_text_align(LV_TEXT_ALIGN_RIGHT, label_fps);
}

auto FPS::set_center() -> void
{
    Text::set_text_align(LV_TEXT_ALIGN_CENTER, label_fps);
}

auto FPS::set_left() -> void
{
    Text::set_text_align(LV_TEXT_ALIGN_LEFT, label_fps);
}

auto FPS::clear() -> void
{
    Text::set_text("", label_fps);

}
#endif //SIMULATOR_FPS_HPP