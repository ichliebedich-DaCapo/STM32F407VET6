#include <project_config.h>
import baseInit;
import key_adapter;
import key_exit;
import app;
#ifdef GUI_ENABLE
import lcd;
import touch;
import gui;
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
    gui::Render::init<bsp::lcd::init,bsp::lcd::flush,bsp::touch::read_single_point>();
#endif

    app::Control::init();


#ifdef FREERTOS_ENABLE
    /*主事件循环或调度器*/
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
        gui::Render::handler();
#endif
        PlatformKey::poll();
        app::Control::background_process();
    }
#endif
}