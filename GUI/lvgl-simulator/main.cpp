#include <iostream>
#include <SDL2/SDL.h>
#include "simulator.hpp"
#include "lvgl.h"
#include "lv_port_disp.h"

int lv_tick_thread(void *data);

// 为了解决错误：undefined reference to `WinMain'
#undef main
int main(int argc, char *argv[])
{

    // 模拟器初始化
    simulator_init(argc, argv);

    // lvgl初始化
    lv_init();
    lv_port_disp_init();

    // lv_tick线程
    SDL_CreateThread(lv_tick_thread, "lv_tick", nullptr);


    // 创建一个屏幕对象
    lv_obj_t * scr = lv_obj_create(nullptr);
    lv_obj_set_size(scr, 480,320);
    lv_obj_set_style_bg_color(scr, lv_color_white(), LV_PART_MAIN); //设置屏幕背景色
    lv_obj_t * label = lv_label_create(scr);
    lv_label_set_text(label, "Hello World!");
    lv_obj_update_layout(scr);
    lv_scr_load(scr);


    // 主循环
    while (simulator_is_running())
    {
        // 模拟器事件处理
        simulator_event_Handler();

        // lvgl事件处理
        lv_timer_handler();

    }

    // 清理资源
    simulator_quit();
    return 0;
}


/* 初始化时间滴答
* 需要定期调用 'lv_tick_inc()' 以告知 LittlevGL 自上次调用以来经过了多少时间
* 创建一个名为 "tick" 的 SDL 线程，该线程将定期调用 'lv_tick_inc()'
*/
int lv_tick_thread(void *data)
{
    (void) data;  /* 忽略传递的参数，因为在这个函数中不需要使用它 */

    while (simulator_is_running())
    {
        SDL_Delay(5);   /* 休眠 5 毫秒 */
        lv_tick_inc(5); /* 告诉 LittlevGL 已经过去了 5 毫秒 */
    }
    return 0;
}

