//
// Created by fairy on 2024/10/19 21:42.
//
#ifndef SIMULATOR_LV_TIMER_HPP
#define SIMULATOR_LV_TIMER_HPP


/* 预编译命令 */
#if 1
// 头文件
#include "component.hpp"

/***********************************定时器**************************************/
/**
 *  @brief 定时器封装类
 *  @note 需要自己调用create函数创建定时器，可以使用remove来删除定时器
 */
class LV_Timer
{
public:
    // 默认为关闭状态
    inline auto create(lv_timer_cb_t timer_cb, uint32_t period = 1000, void *user_data = nullptr) -> void;

    // 删除定时器
    inline auto remove() -> void;

    // 恢复定时器
    inline auto resume() -> void;

    // 暂停定时器
    inline auto pause() -> void;

    // 设置周期
    inline auto set_period(uint32_t period) -> void;

private:
    lv_timer_t *_timer{};
};

// 默认为关闭状态
auto LV_Timer::create(lv_timer_cb_t timer_cb, uint32_t period, void *user_data) -> void
{
    if(_timer== nullptr)
    {
        _timer = lv_timer_create(timer_cb, period, user_data);
        pause();
    }
}

auto LV_Timer::remove() -> void
{
    lv_timer_del(_timer);
    _timer = nullptr;
}

// 恢复定时器
auto LV_Timer::resume() -> void
{
    lv_timer_resume(_timer);
}

// 暂停定时器
auto LV_Timer::pause() -> void
{
    lv_timer_pause(_timer);
}

auto LV_Timer::set_period(uint32_t period) -> void
{
    lv_timer_set_period(_timer, period);
}

#endif
#endif //SIMULATOR_LV_TIMER_HPP