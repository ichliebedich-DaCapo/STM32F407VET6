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
#include <pthread.h>
#include <cmath>
#include <cstdlib>
#include <unistd.h>

// lvgl库
#include "lvgl/lvgl.h"

// lvgl显示和输入驱动
#include "lv_drivers/display/monitor.h"
#include "lv_drivers/indev/mouse.h"
#include "lv_drivers/indev/mousewheel.h"
#include "lv_drivers/indev/keyboard.h"

#if LV_USE_FREEMASTER
#include "external_data_init.h"
#endif

// 引入头文件
#include "GUI.hpp"

static void hal_init();

static int tick_thread(void *data);

volatile int keep_running = 0;

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

#if LV_USE_FREEMASTER
pthread_t thread[2];
pthread_mutex_t jsonrpc_mutex;
pthread_mutex_t lvgl_mutex;
pthread_mutex_t gg_edata_ll_mutex;
pthread_cond_t gg_edata_ll_cond;
#endif

/******************自定义函数*******************/
// 测试用的线程
extern int keyboard_thread(void *data);

/******************自定义函数*******************/
int main(int argc, char **argv)
{
#if LV_USE_FREEMASTER
    if(argc > 1)
    {
        int sim_conf(int argc, char *argv[]);
        sim_conf(argc, argv);
    }
#endif
    /* local threads will run */
    keep_running = 1;

    lv_init();

    /*Initialize the HAL (display, input devices, tick) for LittlevGL*/
    hal_init();

#if LV_USE_FREEMASTER
    pthread_mutex_init(&gg_edata_ll_mutex, NULL);
    pthread_cond_init(&gg_edata_ll_cond, NULL);
    /*Initialize the external data */
    external_task_init(&guider_ui);
#endif

    /*Create a Component-Guider app */
    GUI::init();
    // 自定义线程
    SDL_CreateThread(keyboard_thread, "tick", nullptr);

#if LV_USE_FREEMASTER
    pthread_mutex_init(&jsonrpc_mutex, NULL);
    pthread_mutex_init(&lvgl_mutex, NULL);
    memset(&thread, 0, sizeof(thread));
    /*Create a separate thread to loop the linked list*/
    pthread_create(&thread[0], NULL, gg_edata_task_exec, &jsonrpc_mutex);
#endif

    while (keep_running)
    {
#if LV_USE_FREEMASTER
        pthread_mutex_lock(&lvgl_mutex);
#endif
        lv_task_handler();
#if LV_USE_VIDEO
        video_play(&guider_ui);
#endif
#if LV_USE_FREEMASTER
        pthread_mutex_unlock(&lvgl_mutex);
#endif
        SDL_Delay(5);// 5ms休眠一次
    }

#if LV_USE_FREEMASTER
    /* wakeup gg_edata_task_exec thread if it sleeps, let it finish */
    pthread_cond_signal(&gg_edata_ll_cond);
    /* wait for the gg_edata_task_exec thread*/
    pthread_join(thread[0], NULL);

    freemaster_disconnect();
#endif

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
    mouse_init();                           /* 初始化鼠标驱动程序 */
    static lv_indev_drv_t mouse_indev_drv;
    lv_indev_drv_init(&mouse_indev_drv);    /* 基本初始化 */
    mouse_indev_drv.type = LV_INDEV_TYPE_POINTER;  /* 设置输入设备类型为指针（鼠标） */
    mouse_indev_drv.read_cb = mouse_read;   /* 设置读取回调函数，该函数将定期被库调用以获取鼠标位置和状态 */
    lv_indev_drv_register(&mouse_indev_drv);/* 注册鼠标输入设备驱动程序 */

    /*我之所以把下面注释掉，是因为我不使用lvgl的键盘事件，我仅仅只是想要使用键盘事件触发而已
     * 以此模仿我在单片机的按键处理，同时可以进行更自由的调试*/
#if USE_KEYBOARD
//    /* 创建默认的输入设备组
//     * 这个组将用于管理所有的输入设备，如鼠标、键盘等
//     */
//    lv_group_set_default(lv_group_create());
//
//    /* 注册键盘设备
//     * 使用 'keyboard' 驱动程序读取 PC 的键盘输入
//     */
//    static lv_indev_drv_t keyboard_indev_drv;
//    lv_indev_drv_init(&keyboard_indev_drv); /* 基本初始化 */
//    keyboard_indev_drv.type = LV_INDEV_TYPE_KEYPAD;  /* 设置输入设备类型为键盘 */
//    keyboard_indev_drv.read_cb = keyboard_read;      /* 设置读取回调函数，该函数将定期被库调用以获取键盘输入 */
//    lv_indev_t *kb_indev = lv_indev_drv_register(&keyboard_indev_drv);  /* 注册键盘输入设备驱动程序 */
//    lv_indev_set_group(kb_indev, lv_group_get_default());  /* 将键盘输入设备添加到默认的输入设备组 */
#endif

    /* 初始化时间滴答
     * 需要定期调用 'lv_tick_inc()' 以告知 LittlevGL 自上次调用以来经过了多少时间
     * 创建一个名为 "tick" 的 SDL 线程，该线程将定期调用 'lv_tick_inc()'
     */
    SDL_CreateThread(tick_thread, "tick", nullptr);
}


/**
 * 一个用于测量 LittlevGL 经过时间的任务
 * @param data 未使用
 * @return 从不返回
 */
static int tick_thread(void *data)
{
    (void) data;  /* 忽略传递的参数，因为在这个函数中不需要使用它 */

    while (keep_running)
    {
        SDL_Delay(5);   /* 休眠 5 毫秒 */
        lv_tick_inc(5); /* 告诉 LittlevGL 已经过去了 5 毫秒 */
    }

    return 0;  /* 这个函数实际上永远不会返回，因为它一直在循环中运行 */
}


#if LV_USE_FREEMASTER
/**
 * Enable simulator to be linked and used as DLL. This function is invoked by 
 * FreeMASTER from its background thread. This function must finish when 
 * sim_exit() is called asynchronously.
 */
__declspec(dllexport)
int sim_main(int argc, char *argv[])
{
    int ret = main(argc, argv);
    return ret;
}

/**
 * Finish all running threads and terminate the application running as sim_main().
 */
__declspec(dllexport)
int sim_exit(void)
{
    keep_running = 0;
    return 0;
}

/**
 * Enable FreeMASTER to configure the simulator before starting it.
 * This function is the first one called by the FreeMASTER from its Component thread.
 */
__declspec(dllexport)
int sim_conf(int argc, char *argv[])
{
    /* Process the arguments */
    if(argc > 0 && argv != NULL)
    {
        for(int i=0; i<argc; i++)
        {
            /* --rpcs is used to specify FreeMASTER JSON-RPC server address. */
            if(strcmp(argv[i], "--rpcs") == 0)
            {
                i++;
                if(i<argc && strncmp(argv[i], "ws://", 5) == 0)
                {
                    freemaster_server = strdup(argv[i]);
                }
            }
        }
    }
    return 0;
}

/**
 * Enable host application to retrieve DLL information
 */
__declspec(dllexport)
const void * sim_info(void)
{
#define SIM_F_RESIZE_UP        0x0001   /* Enable growing size within parent view */
#define SIM_F_RESIZE_DOWN      0x0002   /* Enable shrinking size within parent view */
#define SIM_F_KEEP_ASPECT      0x0004   /* When RESIZE used, preserve aspect ratio */
#define SIM_F_CENTER_H         0x0008   /* Center horizontally */
#define SIM_F_CENTER_V         0x0010   /* Center vertically */
#define SIM_F_CENTER_FORCE     0x0020   /* Keep centered even if parent is smaller (move beyond top/left border) */
#define SIM_F_AUTO_BACKCOLOR   0x0040   /* Determine background color automatically */

    typedef struct
    {
        uint32_t sz;
        const char* name;
        const char* desc;
        const char* copyright;
        const char* server;
        uint32_t width;
        uint32_t height;
        uint32_t flags;
        uint32_t background;
        uint32_t reserved;
    } sim_info_t;

    /* Constant options: */
    static sim_info_t info =
    {
        .sz = sizeof(sim_info_t),
        .name = "NXP Component-Guider Project",
        .desc = "Designed to enable Component designs created in GG to be hosted in FreeMASTER",
        .copyright = "Copyright 2024 NXP",
        .server = "",
        .width = MONITOR_HOR_RES,
        .height = MONITOR_VER_RES,
        .flags = SIM_F_CENTER_H,
        .background = 0,
    };

    /* Runtime-defined options: */
    info.server = freemaster_server;

    return &info;
}

#endif
