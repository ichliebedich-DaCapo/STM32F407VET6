import baseInit;
#include "key_adapter.hpp"
import key_exit;
#include <app.hpp>
#ifdef GUI_ENABLE

#include "lcd.h"
#include "GUI.hpp"
#include "touch.h"

#endif

#ifdef FREERTOS_ENABLE

#include "cmsis_os2.h"

#endif

int main()
{
    /*基础初始化*/
    bsp::baseInit::init(); // 基础驱动初始化

    PlatformKey::init<bsp::key_exit::init>();// 初始化按键

#ifdef GUI_ENABLE
    GUI::init<lcd_init,lcd_flush, touch_read_single_point>();
#endif

    App::init();

    /*主事件循环或调度器*/
#ifdef FREERTOS_ENABLE
    // 创建按键线程
    const osThreadAttr_t keyTask_attributes = {
            .name = "keyTask",
            .stack_size = 256 * 4,
            .priority = (osPriority_t) osPriorityNormal,
    };
    osThreadNew([](void *)
                {
                    for (;;) { PlatformKey::poll(); }
                }, nullptr, &keyTask_attributes);


    // 创建后台线程
    const osThreadAttr_t backgroundTask_attributes = {
            .name = "backgroundTask",
            .stack_size = 256 * 4,
            .priority = (osPriority_t) osPriorityLow,
    };
    osThreadNew([](void *)
                {
                    for (;;)
                    {
                        background_handler();
                    }
                }, nullptr, &backgroundTask_attributes);

    // 创建GUI线程
#ifdef GUI_ENABLE
    const osThreadAttr_t GUITask_attributes = {
            .name = "GUITask",
            .stack_size = 512 * 4,
            .priority = (osPriority_t) osPriorityNormal,
    };
    osThreadNew([](void *)
                {
                    for (;;)
                    {
                        osDelay(5);
                        GUI::handler();
                    }
                }, nullptr, &GUITask_attributes);
#endif

    // 启动调度器
    osKernelStart();
#else
    for (;;)
    {
#ifdef GUI_ENABLE
        GUI::handler();
#endif
        PlatformKey::poll();
        App::background_process();
    }
#endif
}