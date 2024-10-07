//
// Created by 34753 on 2024/9/22.
//

#include "GUI_Init.h"
#include "lv_port_disp.h"
#include "cmsis_os2.h"

void GUITask(void *argument);

const osThreadAttr_t voiceTask_attributes = {
        .name = "GUITask",
        .stack_size = 512 * 4,
        .priority = (osPriority_t) osPriorityNormal,
};

void GUI_Init()
{
    lv_port_disp_init();
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