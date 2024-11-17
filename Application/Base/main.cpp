#include "baseInit.hpp"
#ifndef APP_NO_RTOS // 是否启用RTOS
#include "cmsis_os2.h"
#endif// APP_NO_RTOS

#include "GUI.hpp"
#include "key.hpp"
#include "lcd.h"

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

    GUI::init<lcd_flush>();
    app_init();

/*主事件循环或调度器*/
#ifndef APP_NO_RTOS
    osKernelStart();
#else
    for (;;)
    {
        GUI::handler();
        Key::handler();
        background_handler();
    }
#endif
}