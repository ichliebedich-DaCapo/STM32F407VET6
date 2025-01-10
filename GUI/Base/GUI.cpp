//
// Created by fairy on 2024/10/17 18:53.
//

#include "GUI.hpp"

#include "ui.hpp"



#ifndef GUI_DISABLE

#ifdef ARM_MATH_CM4
#include "JYZQ_Conf.h"
#endif




#ifndef FreeRTOS_ENABLE
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
#ifndef GUI_DISABLE
    //    __disable_irq();
    //    lv_init();// 混账，搞了半天是因为漏加你才死机
    //    lv_port_disp_init();// 进入临界保护区
    //    __enable_irq();

#endif
#ifndef GUI_DISABLE
#ifndef APP_NO_RTOS
    //    osThreadNew([](void *argument) -> void
    //                {
    //                    osDelay(5);
    //                    lv_timer_handler();
    //                }, nullptr, &voiceTask_attributes);
#endif
#endif

    gui->main.screen.get_object() = lv_obj_create(nullptr);
    lv_obj_set_size(gui->main.screen.get_object(), DISP_HOR_RES, DISP_VER_RES);
    GUI_Base::screen_init();// 初始化屏幕
    lv_obj_update_layout(gui->main.screen.get_object());
    GUI_Base::events_init();
    lv_scr_load(gui->main.screen.get_object());
}



