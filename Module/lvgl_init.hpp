//
// Created by fairy on 2024/9/22.
//

#ifndef FURINA_LVGL_INIT_HPP
#define FURINA_LVGL_INIT_HPP
#include "lv_port_disp.h"
#include "JYZQ_Conf.h"

#include "stm32f4xx_hal.h"
#ifndef APP_NO_RTOS
void GUI_Init();
#else
inline void lvgl_init()
{
    __disable_irq();
    lv_init();// 混账，搞了半天是因为漏加你才死机
    lv_port_disp_init();// 进入临界保护区
    __enable_irq();
}

inline void GUI_handler()
{
    lv_task_handler();
}
#endif


#endif //FURINA_LVGL_INIT_HPP
