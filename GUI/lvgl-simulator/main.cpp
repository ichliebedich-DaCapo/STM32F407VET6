/*
 * SPDX-License-Identifier: MIT
 * Copyright 2023 NXP
 */

/*********************
 *      INCLUDES
 *********************/
#define SDL_MAIN_HANDLED        /*To fix SDL's "undefined reference to WinMain" issue*/

// 标准库
#include <SDL2/SDL.h>
#include <unistd.h>

// lvgl库
#include "lvgl/lvgl.h"

// lvgl显示和输入驱动
#include "lv_drivers/display/monitor.h"
#include "lv_drivers/indev/mouse.h"

#if LV_USE_FREEMASTER
#include "external_data_init.h"
#endif

// 引入头文件
#include "GUI.hpp"

static void hal_init();

static int tick_thread(void *data);

static inline void video_handler()
{
#if LV_USE_VIDEO
    video_play(&guider_ui);
#endif
}

volatile int keep_running = 1;

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"


/******************自定义函数*******************/
// 测试用的线程
extern int keyboard_thread(void *data);

/******************自定义函数*******************/
int main(int argc, char **argv)
{
    lv_init();
    hal_init();
    GUI::resource_init();

    // 自定义线程
    SDL_CreateThread(keyboard_thread, "tick", nullptr);

    while (keep_running)
    {
        lv_task_handler();
        video_handler();
        SDL_Delay(5);// 5ms休眠一次
    }
    SDL_Quit();
    return 0;
}


/**
 * 初始化 Littlev 图形库的硬件抽象层 (HAL)
 */
static void hal_init()
{
    /* 创建默认的输入设备组
     * 这个组将用于管理所有的输入设备，如鼠标、键盘等
     */
    lv_group_set_default(lv_group_create());

    /* 使用 'monitor' 驱动程序
     * 该驱动程序在 PC 的显示器上创建一个窗口，模拟一个显示设备
     */
    monitor_init();

    /* 创建显示缓冲区
     * 显示缓冲区用于存储要显示的内容，这里创建了一个大小为 MONITOR_HOR_RES * MONITOR_VER_RES 的缓冲区
     */
    static lv_disp_draw_buf_t disp_buf1;
    static lv_color_t buf1_1[MONITOR_HOR_RES * MONITOR_VER_RES];
    lv_disp_draw_buf_init(&disp_buf1, buf1_1, nullptr, MONITOR_HOR_RES * MONITOR_VER_RES);

    /* 创建显示设备
     * 初始化显示驱动程序，并设置其属性，如缓冲区、刷新回调函数、水平分辨率和垂直分辨率
     */
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);            /* 基本初始化 */
    disp_drv.draw_buf = &disp_buf1;         /* 设置显示缓冲区 */
    disp_drv.flush_cb = monitor_flush;      /* 设置刷新回调函数，用于将缓冲区内容刷新到显示设备 */
    disp_drv.hor_res = MONITOR_HOR_RES;     /* 设置水平分辨率 */
    disp_drv.ver_res = MONITOR_VER_RES;     /* 设置垂直分辨率 */
    lv_disp_drv_register(&disp_drv);        /* 注册显示驱动程序 */

    /* 添加鼠标作为输入设备
     * 使用 'mouse' 驱动程序读取 PC 的鼠标位置和状态
     */
    static lv_indev_drv_t mouse_indev_drv;
    lv_indev_drv_init(&mouse_indev_drv);    /* 基本初始化 */
    mouse_indev_drv.type = LV_INDEV_TYPE_POINTER;  /* 设置输入设备类型为指针（鼠标） */
    mouse_indev_drv.read_cb = mouse_read;   /* 设置读取回调函数，该函数将定期被库调用以获取鼠标位置和状态 */
    lv_indev_drv_register(&mouse_indev_drv);/* 注册鼠标输入设备驱动程序 */

    /*我之所以把下面注释掉，是因为我不使用lvgl的键盘事件，我仅仅只是想要使用键盘事件触发而已
     * 以此模仿我在单片机的按键处理，同时可以进行更自由的调试*/
#if !USE_KEYBOARD
    /* 创建默认的输入设备组
     * 这个组将用于管理所有的输入设备，如鼠标、键盘等
     */
    lv_group_set_default(lv_group_create());

    /* 注册键盘设备
     * 使用 'keyboard' 驱动程序读取 PC 的键盘输入
     */
    static lv_indev_drv_t keyboard_indev_drv;
    lv_indev_drv_init(&keyboard_indev_drv); /* 基本初始化 */
    keyboard_indev_drv.type = LV_INDEV_TYPE_KEYPAD;  /* 设置输入设备类型为键盘 */
    keyboard_indev_drv.read_cb = keyboard_read;      /* 设置读取回调函数，该函数将定期被库调用以获取键盘输入 */
    lv_indev_t *kb_indev = lv_indev_drv_register(&keyboard_indev_drv);  /* 注册键盘输入设备驱动程序 */
    lv_indev_set_group(kb_indev, lv_group_get_default());  /* 将键盘输入设备添加到默认的输入设备组 */
#endif

    /* 初始化时间滴答
     * 需要定期调用 'lv_tick_inc()' 以告知 LittlevGL 自上次调用以来经过了多少时间
     * 创建一个名为 "tick" 的 SDL 线程，该线程将定期调用 'lv_tick_inc()'
     */
    SDL_CreateThread([](void *data)
                     {
                         (void) data;  /* 忽略传递的参数，因为在这个函数中不需要使用它 */

                         while (keep_running)
                         {
                             SDL_Delay(5);   /* 休眠 5 毫秒 */
                             lv_tick_inc(5); /* 告诉 LittlevGL 已经过去了 5 毫秒 */
                         }

                         return keep_running;
                     }, "tick", nullptr);
}


