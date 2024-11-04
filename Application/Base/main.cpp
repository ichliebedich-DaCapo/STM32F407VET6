#include "baseInit.h"
#ifndef APP_NO_RTOS // 是否启用RTOS
#include "cmsis_os2.h"
#endif// APP_NO_RTOS

#include "keyTaskHandler.hpp"
#include "GUI.hpp"

extern void app_init();// 应用程序初始化函数,强制定义

#ifdef APP_NO_RTOS

extern void background_handler();// 后台处理函数
__attribute__((weak)) void background_handler() {}

#else
#define background_handler() void(0)
#endif

int main()
{
/*基础初始化*/
    BaseInit(); // 基础驱动初始化
#ifndef APP_NO_RTOS
    osKernelInitialize();// FreeRTOS内核初始化
#endif// APP_NO_RTOS
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