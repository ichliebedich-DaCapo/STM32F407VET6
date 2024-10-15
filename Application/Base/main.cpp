#include "baseInit.h"
// 是否启用RTOS
#ifndef APP_NO_RTOS
#include "cmsis_os2.h"
#endif// APP_NO_RTOS
#include "GUI_Init.h"
#include "keyTaskHandler.hpp"

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
    GUI_Init();
    keyTaskHandler_init();
/*应用程序初始化*/
    app_init();

/*启动调度器*/
#ifndef APP_NO_RTOS
    osKernelStart();
#else
    for (;;)
    {
        GUI_handler();
        key.handler();// 不太想优化成静态类，因为我真的很喜欢这个"."
        background_handler();
    }
#endif
}





