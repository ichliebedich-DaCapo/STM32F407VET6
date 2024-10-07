//
// Created by 34753 on 2024/10/7.
//

#include "keyTaskHandler.h"
#if ENABLE_KEY_TASK_HANDLE
#include "key.h"
#include "cmsis_os2.h"

extern osSemaphoreId_t keySemHandle;

osThreadId_t keyTaskHandler;
const osThreadAttr_t keyTask_attributes = {
        .name = "keyTask",
        .stack_size = 256 * 4,
        .priority = (osPriority_t) osPriorityNormal,
};

void keyTask(void *argument);


// 初始化按键任务
void keyTaskHandler_init()
{
    Key::init();
    keyTaskHandler = osThreadNew(keyTask, nullptr, &keyTask_attributes);
}

// 按键任务处理
void keyTask(void *argument)
{
    for(;;)
    {
        osSemaphoreAcquire(keySemHandle, osWaitForever);
        /* xxx_taskHandle(); */

//        switch (key.getCode())
//        {
//            case keyk0:
//                if(key.stateHandler(1))
//                {
//
//                }
//                break;
//
//            case keyk1:
//                break;
//
//            default:
//                break;
//        }
    }
}

#endif