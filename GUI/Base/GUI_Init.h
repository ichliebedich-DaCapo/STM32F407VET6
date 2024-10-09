//
// Created by 34753 on 2024/9/22.
//

#ifndef FURINA_GUI_INIT_H
#define FURINA_GUI_INIT_H
#include "main.h"
#include "lv_port_disp.h"
#ifndef APP_NO_RTOS
void GUI_Init();
#else
inline void GUI_Init()
{
    lv_port_disp_init();
}
inline void GUI_handler()
{
    lv_task_handler();
}
#endif


#endif //FURINA_GUI_INIT_H
