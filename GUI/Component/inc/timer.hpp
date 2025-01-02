//
// Created by fairy on 2024/10/19 21:42.
//
#ifndef SIMULATOR_TIMER_HPP
#define SIMULATOR_TIMER_HPP


/* 预编译命令 */
#if 1
// 头文件
#include "component.hpp"

/***********************************定时器**************************************/
/**
 *  @brief 定时器封装类
 *  @note 需要自己调用create函数创建定时器，可以使用remove来删除定时器
 */
class Timer
{
public:
    // 默认为关闭状态
    auto create(lv_timer_cb_t timer_cb, uint32_t period = 1000, void *user_data = nullptr) -> void;

    // 删除定时器
    auto remove() -> void;

    // 恢复定时器
    auto resume() -> void;

    // 暂停定时器
    auto pause() -> void;

    // 设置周期
    auto set_period(uint32_t period) -> void;

private:
    lv_timer_t *_timer{};
};



#endif
#endif //SIMULATOR_TIMER_HPP