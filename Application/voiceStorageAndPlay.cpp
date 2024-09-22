//
// Created by 34753 on 2024/9/22.
//

#include "voiceStorageAndPlay.h"
#if ENABLE_VOICE_STORAGE_AND_PLAY
#include "Base/osInit.h"

osThreadId_t defaultTaskHandle;
const osThreadAttr_t GUITask_attributes = {
        .name = "defaultTask",
        .stack_size = 128 * 4,
        .priority = (osPriority_t) osPriorityNormal,
};
void StartDefaultTask(void *argument);

/*语音存储与回放初始化*/
void voiceStorageAndPlayInit()
{
    defaultTaskHandle = osThreadNew(StartDefaultTask, nullptr, &GUITask_attributes);
}



void StartDefaultTask(void *argument)
{
    for(;;)
    {
        osDelay(1);
    }
}

#endif