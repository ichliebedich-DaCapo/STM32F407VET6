// key_rtos_adapter.hpp （FreeRTOS专用适配）
#pragma once


#include "key.hpp"
#include <project_config.h>

#ifdef FREERTOS_ENABLE
#include "cmsis_os2.h"
/**
 * @brief FreeRTOS按键适配器
 */
template<>
struct KeyAdapter<struct FreeRTOS>
{
    // FreeRTOS信号量操作
    static inline void notify_from_isr()
    {
        osSemaphoreRelease(sem_);
    }

    static inline bool acquire()
    {
        return osSemaphoreAcquire(sem_, osWaitForever);
    }


    static inline void init()
    {
        sem_ = osSemaphoreNew(1, 1, nullptr);
    }

    static inline osSemaphoreId_t sem_ = nullptr;
    static inline osThreadId_t thread_id_ = nullptr;
};

#else
/**
 * @brief 裸机按键适配器
 */
template<>
struct KeyAdapter<struct Bare>
{
    static inline void notify_from_isr()
    {
        sign = 0xFF;
    }

    static inline bool acquire()
    {
        if (sign)
        {
            sign = 0;
            return true;
        }
        return false;
    }

    static inline void init()
    {

    }

    static uint8_t inline sign = 0;
};
#endif


#ifdef FREERTOS_ENABLE
// 使用别名简化
using PlatformKey = Key::Core<KeyAdapter<FreeRTOS>>;// RTOS下的按键
#else
using PlatformKey = Key::Core<KeyAdapter<Bare>>;// 裸机下的按键
#endif