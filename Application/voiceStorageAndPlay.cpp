//
// Created by 34753 on 2024/9/22.
//

#include "voiceStorageAndPlay.hpp"

#if ENABLE_VOICE_STORAGE_AND_PLAY

#include "key.hpp"
#include "spi.h"
#include "timer.h"
#include "adc.h"
#include "dac.h"
#include "player.hpp"
#include "flash_storage.hpp"
//const osThreadAttr_t voiceTask_attributes = {
//        .name = "voiceTask",
//        .stack_size = 128 * 4,
//        .priority = (osPriority_t) osPriorityNormal,
//};
//void VoiceTask(void *argument);

/*语音存储与回放初始化*/
void app_init()
{
    spi3_init();
    dac_init();
    adc1_init();
    timer2_init(49, 104);// 分频为16KHz
    timer6_init(49, 104);
//    osThreadNew(VoiceTask, nullptr, &voiceTask_attributes);
}

/*语音存储与回放处理函数,用于处理各种按键响应*/
void key_handler()
{
    switch (key.getCode())
    {
        case keyk0:// 播放
            if (key.stateHandler(2))
            {
                FlashStorage::read_isr_ready();// 预加载
                Player::on();
            } else
            {
                Player::off();
            }
            break;

        case keyk1:// 录音
            if (key.stateHandler(2))
            {
                FlashStorage::write_isr_ready();// 预加载
                Player::record_on();
            } else
            {
                Player::record_off();
            }
            break;

        case keyk2:// 慢放
            Player::set_speed(PlaybackSpeed::Speed_0_75x);

        case keyk3:// 快放
            Player::set_speed(PlaybackSpeed::Speed_1_5x);

        default:
            break;
    }
}

void background_handler()
{
    FlashStorage::background_processing();
}

/**实现中断服务例程*/
// 用于采集ADC数据
void adc1_isr()
{
    // 12位舍弃低4位
    FlashStorage::write_isr(HAL_ADC_GetValue(&hadc1) >> 4);
}

// 用于播放DAC数据
void timer6_isr()
{
    // 把8位数据恢复为12位
    HAL_DAC_SetValue(&hdac, DAC1_CHANNEL_1, DAC_ALIGN_12B_R, FlashStorage::read_isr() << 4);
}


//void VoiceTask(void *argument)
//{
//    for(;;)
//    {
//        osDelay(1);
//    }
//}

#endif