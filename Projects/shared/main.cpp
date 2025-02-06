#include <project_config.h>
#include "baseInit.h"
#include "key.hpp"

extern void app_init();// 应用程序初始化函数,强制定义
extern void background_handler();// 后台处理函数
__attribute__((weak)) void background_handler() {}


#ifdef FreeRTOS_ENABLE
void backgroundTask(void *argument)// 后台线程
{
    for (;;)
    {
        background_handler();
    }
}
#endif


int main()
{
    /*基础初始化*/
    BaseInit(); // 基础驱动初始化

#ifdef FreeRTOS_ENABLE
    osKernelInitialize();// FreeRTOS内核初始化
#endif// FreeRTOS_ENABLE

    Key::init();

#ifdef GUI_ENABLE
    GUI::init<lcd_flush>();
#endif

    app_init();

    /*主事件循环或调度器*/
#ifdef FreeRTOS_ENABLE
    // 创建后台线程
    const osThreadAttr_t backgroundTask_attributes = {
            .name = "backgroundTask",
            .stack_size = 256 * 4,
            .priority = (osPriority_t) osPriorityLow,
    };
    osThreadNew(backgroundTask, nullptr, &backgroundTask_attributes);

    // 创建GUI线程
#ifndef GUI_DISABLE
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
        Key::handler();
        background_handler();
    }
#endif
}