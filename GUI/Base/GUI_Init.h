//
// Created by fairy on 2024/9/22.
//

#ifndef FURINA_GUI_INIT_H
#define FURINA_GUI_INIT_H
#include "lv_port_disp.h"
#include "JYZQ_Conf.h"
#include "gui_guider.h"
#include "events_init.h"
#include "stm32f4xx_hal.h"

#ifndef APP_NO_RTOS
void GUI_Init();
#else
inline void GUI_Init()
{
    __disable_irq();
    lv_init();// 混账，搞了半天是因为漏加你才死机
    lv_port_disp_init();// 进入临界保护区
    __enable_irq();

    setup_ui(&guider_ui);
    events_init(&guider_ui);
}

inline void GUI_handler()
{
    lv_task_handler();
}
#endif


#endif //FURINA_GUI_INIT_H
