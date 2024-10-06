//
// Created by 34753 on 2024/9/22.
//

#include "voiceStorageAndPlay.h"
#if ENABLE_VOICE_STORAGE_AND_PLAY
#include "Base/osInit.h"


const osThreadAttr_t voiceTask_attributes = {
        .name = "voiceTask",
        .stack_size = 128 * 4,
        .priority = (osPriority_t) osPriorityNormal,
};
void VoiceTask(void *argument);

/*语音存储与回放初始化*/
void voiceStorageAndPlayInit()
{
    osThreadNew(VoiceTask, nullptr, &voiceTask_attributes);
}

/*语音存储与回放处理函数,用于处理各种按键响应*/
void voiceStorageAndPlay_handler()
{

}


void VoiceTask(void *argument)
{
    for(;;)
    {
        osDelay(1);
    }
}

#endif