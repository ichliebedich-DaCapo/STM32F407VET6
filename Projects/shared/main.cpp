#include <project_config.h>
#include "baseInit.h"
#include "key_adapter.hpp"

#ifdef GUI_ENABLE

#include "GUI.hpp"

#endif

#ifdef FREERTOS_ENABLE

#include "cmsis_os2.h"

#endif

extern void app_init();// 应用程序初始化函数,强制定义
extern void background_handler();// 后台处理函数
__attribute__((weak)) void background_handler() {}



int main()
{
    /*基础初始化*/
    BaseInit(); // 基础驱动初始化

    PlatformKey::init<key_exti_init>();// 初始化按键

#ifdef GUI_ENABLE
    GUI::init<lcd_init,lcd_flush,touch_read_single_point>();
#endif

    app_init();

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
        background_handler();
    }
#endif
}