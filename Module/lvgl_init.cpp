//
// Created by fairy on 2024/9/22.
//
#include "lvgl_init.h"
lv_ui guider_ui;
#ifndef APP_NO_RTOS
#include "cmsis_os2.h"

void GUITask(void *argument);

const osThreadAttr_t voiceTask_attributes = {
        .name = "GUITask",
        .stack_size = 512 * 4,
        .priority = (osPriority_t) osPriorityNormal,
};

void GUI_Init()
{
    __disable_irq();
    lv_init();// 混账，搞了半天是因为漏加你才死机
    lv_port_disp_init();// 进入临界保护区
    __enable_irq();

    osThreadNew(GUITask, nullptr, &voiceTask_attributes);
}

void GUITask(void *argument)
{
    for (;;)
    {
        osDelay(5);
        lv_timer_handler();
    }
}
#endif
