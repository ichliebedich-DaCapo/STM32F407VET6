#include "JYZQ_Conf.h"
#include "baseInit.hpp"
#ifdef FreeRTOS_ENABLE // 是否启用RTOS
#include "cmsis_os2.h"
#endif// FreeRTOS_ENABLE

#ifndef GUI_DISABLE
#include "lcd.h"
#include "GUI.hpp"
#endif

#include "key.hpp"


extern void app_init();// 应用程序初始化函数,强制定义

#ifndef FreeRTOS_ENABLE

extern void background_handler();// 后台处理函数
__attribute__((weak)) void background_handler() {}

#else
#define background_handler() void(0)
#endif

int main()
{
    /*基础初始化*/
    BaseInit(); // 基础驱动初始化

#ifdef FreeRTOS_ENABLE
    osKernelInitialize();// FreeRTOS内核初始化
#endif// FreeRTOS_ENABLE

#ifndef GUI_DISABLE
    GUI::init<lcd_flush>();
#endif
    app_init();

    /*主事件循环或调度器*/
#ifdef FreeRTOS_ENABLE
    osKernelStart();
#else
    for (;;)
    {
#ifndef GUI_DISABLE
        GUI::handler();
#endif
        Key::handler();
        background_handler();
    }
#endif
}