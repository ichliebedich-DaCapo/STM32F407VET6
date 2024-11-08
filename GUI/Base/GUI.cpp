//
// Created by fairy on 2024/10/17 18:53.
//
#include "GUI.hpp"
#include "component.hpp"


// 头文件
#include "events.hpp"


#ifdef ARM_MATH_CM4

#include "JYZQ_Conf.h"





/*Descriptor of a display driver*/

#ifndef APP_NO_RTOS
//#include "cmsis_os2.h"
//
//const osThreadAttr_t voiceTask_attributes = {
//        .name = "GUITask",
//        .stack_size = 512 * 4,
//        .priority = (osPriority_t) osPriorityNormal,
//};
#endif
#endif

// 函数
auto GUI::resource_init() -> void
{
#ifdef ARM_MATH_CM4
//    __disable_irq();
//    lv_init();// 混账，搞了半天是因为漏加你才死机
//    lv_port_disp_init();// 进入临界保护区
//    __enable_irq();

#endif
#ifdef ARM_MATH_CM4
#ifndef APP_NO_RTOS
    //    osThreadNew([](void *argument) -> void
    //                {
    //                    osDelay(5);
    //                    lv_timer_handler();
    //                }, nullptr, &voiceTask_attributes);
#endif
#endif

    gui->main.screen = lv_obj_create(nullptr);
    lv_obj_set_size(gui->main.screen, 480, 320);
    Screen::init();// 初始化屏幕
    lv_obj_update_layout(gui->main.screen);
    Events::init();
    lv_scr_load(gui->main.screen);
}

volatile bool disp_flush_enabled = true;
/* Enable updating the screen (the flushing process) when disp_flush() is called by lvgl
 */
void disp_enable_update()
{
    disp_flush_enabled = true;
}

/* Disable updating the screen (the flushing process) when disp_flush() is called by lvgl
 */
void disp_disable_update()
{
    disp_flush_enabled = false;
}