#include "baseInit.h"
#include "main.h"
// 是否启用RTOS
#ifndef APP_NO_RTOS
#include "cmsis_os2.h"
#endif// APP_NO_RTOS

#include "stm32f4xx_hal.h"
#include "GUI_Init.h"
#include "keyTaskHandler.h"

extern void app_init();

__weak void app_init() {}

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
        key.handler();
    }
#endif
}





