#include "baseInit.h"
// 是否启用RTOS
#ifndef APP_NO_RTOS
#include "cmsis_os2.h"
#endif// APP_NO_RTOS

#include "lvgl_init.hpp"
#include "keyTaskHandler.hpp"
#include "GUI.hpp"

// 应用程序初始化函数
extern void app_init();

__weak void app_init() {}

// 后台处理函数
#ifdef APP_NO_RTOS

extern void background_handler();

__weak void background_handler() {}

#endif

/**预编译指令多了还挺烦*/
int main()
{
/*基础初始化*/
    BaseInit(); // 基础驱动初始化
#ifndef APP_NO_RTOS
    osKernelInitialize();// FreeRTOS内核初始化
#endif// APP_NO_RTOS
    lvgl_init();// 进行LVGL模块初始化
    keyTaskHandler_init();
    GUI::init();
/*应用程序初始化*/
    app_init();

/*启动调度器*/
#ifndef APP_NO_RTOS
    osKernelStart();
#else
    for (;;)
    {
        GUI_handler();
        Key::handler();
        background_handler();
    }
#endif
}





