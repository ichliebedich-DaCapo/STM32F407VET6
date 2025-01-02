//
// Created by fairy on 2025/1/2 17:14.
//

// 头文件
#include "timer.hpp"

// 默认为关闭状态
auto Timer::create(lv_timer_cb_t timer_cb, uint32_t period, void *user_data) -> void
{
    if(_timer== nullptr)
    {
        _timer = lv_timer_create(timer_cb, period, user_data);
        pause();
    }
}

auto Timer::remove() -> void
{
    lv_timer_del(_timer);
    _timer = nullptr;
}

// 恢复定时器
auto Timer::resume() -> void
{
    lv_timer_resume(_timer);
}

// 暂停定时器
auto Timer::pause() -> void
{
    lv_timer_pause(_timer);
}

auto Timer::set_period(uint32_t period) -> void
{
    lv_timer_set_period(_timer, period);
}
