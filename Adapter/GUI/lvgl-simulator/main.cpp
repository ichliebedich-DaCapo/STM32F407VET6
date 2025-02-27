#include <iostream>
#include <SDL2/SDL.h>
#include "simulator.hpp"
#include "lvgl.h"
#include "GUI.hpp"

int lv_tick_thread(void *data);
extern int keyboard_thread(void *data);


// 为了解决错误：undefined reference to `WinMain'
#undef main
int main(int argc, char *argv[])
{
    simulator_init();

    // GUI初始化
    GUI::init<LCD_Color_Fill,touchpad_read_xy>();

    SDL_CreateThread(lv_tick_thread, "lv_tick", nullptr);// lv_tick线程
    SDL_CreateThread(keyboard_thread, "keyboard", nullptr);// 键盘线程

    // 主循环
    while (simulator_is_running())
    {
        simulator_event_Handler();// 模拟器事件处理
        GUI::handler();// lvgl事件处理

        SDL_Delay(10);// 短暂休眠
    }

    // 清理资源
    simulator_quit();
    return 0;
}


/** 初始化时间滴答
* 需要定期调用 'lv_tick_inc()' 以告知 LittlevGL 自上次调用以来经过了多少时间
* 创建一个名为 "tick" 的 SDL 线程，该线程将定期调用 'lv_tick_inc()'
*/
int lv_tick_thread(void *data)
{
    (void) data;

    while (simulator_is_running())
    {
        SDL_Delay(3);
        lv_tick_inc(3);
    }
    return 0;
}

