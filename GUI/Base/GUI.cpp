//
// Created by fairy on 2024/10/17 18:53.
//
#include "GUI.hpp"
#include "component.hpp"

#if 1
// 头文件
//#include "stm32f4xx_hal.h"
#include "events.hpp"
//#include "lv_port_disp.h"
#ifdef ARM_MATH_CM4
#ifndef APP_NO_RTOS


#include "cmsis_os2.h"

const osThreadAttr_t voiceTask_attributes = {
        .name = "GUITask",
        .stack_size = 512 * 4,
        .priority = (osPriority_t) osPriorityNormal,
};
#endif
#endif

// 函数
auto GUI::init() -> void
{
//    __disable_irq();
//    lv_init();// 混账，搞了半天是因为漏加你才死机
//    lv_port_disp_init();// 进入临界保护区
//    __enable_irq();

#ifdef ARM_MATH_CM4
#ifndef APP_NO_RTOS
    osThreadNew([](void *argument) -> void
                {
                    osDelay(5);
                    lv_timer_handler();
                }, nullptr, &voiceTask_attributes);
#endif
#endif

    gui->main.screen = lv_obj_create(nullptr);
    lv_obj_set_size(gui->main.screen, 480, 320);
    Screen::init();// 初始化屏幕
    lv_obj_update_layout(gui->main.screen);
    Events::init();
    lv_scr_load(gui->main.screen);
}



#endif