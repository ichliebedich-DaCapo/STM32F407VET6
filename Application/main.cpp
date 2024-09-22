#include "cmsis_os2.h"
#include "baseInit.h"
#include "voiceStorageAndPlay.h"
#include "GUI_Init.h"
int main()
{
/*基础初始化*/
    BaseInit(); // 基础驱动初始化
    osKernelInitialize();// FreeRTOS内核初始化
    GUI_Init();
/*应用程序初始化*/
    voiceStorageAndPlayInit();


/* 调度器启动*/
    osKernelStart();

// 正常情况代码不会执行到这步
    for (;;);
}





