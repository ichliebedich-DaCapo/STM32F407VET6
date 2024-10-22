//
// Created by fairy on 2024/9/22.
//

#include "voiceStorageAndPlay.hpp"

#if ENABLE_VOICE_STORAGE_AND_PLAY

#include "lvgl.h"
#include "key.hpp"
#include "spi.h"
#include "timer.h"
#include "adc.h"
#include "dac.h"
#include "player.hpp"
#include "flash_storage.hpp"
#include "GUI.hpp"
#include "w25qxx.h"
//const osThreadAttr_t voiceTask_attributes = {
//        .name = "voiceTask",
//        .stack_size = 128 * 4,
//        .priority = (osPriority_t) osPriorityNormal,
//};
//void VoiceTask(void *argument);

/*语音存储与回放初始化*/
void app_init()
{
    spi2_init();// 初始化SPI2
    dac_init();
    adc1_init();
    w25qxx_init();// 初始化flash
    timer2_init(FREQ_84M_to_100);// 分频为16KHz
    timer6_init(FREQ_84M_to_16K);
}

/*语音存储与回放处理函数,用于处理各种按键响应*/
void key_handler()
{
//    switch (Key::getCode())
//    {
//        case keyk0:// 播放
//            if (Key::stateHandler(2))
//            {
//                ImageButton::press(GUI_Base::get_ui()->main.imgbtn_play);
//                FlashStorage::read_isr_ready();// 预加载
//                Player::on();
//            } else
//            {
//                ImageButton::release(GUI_Base::get_ui()->main.imgbtn_play);
//                Player::off();
//            }
//            break;
//
//        case keyk1:// 录音
//
//////            ITM_SendChar('#');
////            if (Key::stateHandler(2))
////            {
////                FlashStorage::write_isr_ready();// 预加载
////                Player::record_on();
////            } else
////            {
////                Player::record_off();
////            }
//            break;
////
////        case keyk2:// 慢放
////            Player::set_speed(PlaybackSpeed::Speed_0_75x);
////
////        case keyk3:// 快放
////            Player::set_speed(PlaybackSpeed::Speed_1_5x);
//
//        default:
//            break;
//    }
    switch (Key::getCode())
    {
        case keyk0:// 写入1字节
            adc1_start_it();
            __BKPT(0);// 设置一个断点
            break;
        case keyk1:// 读取1字节
            adc1_stop_it();
            __BKPT(0);// 设置一个断点
            break;
        case keyk2:// 读取256字节

            __BKPT(0);// 设置一个断点
            break;

        case keyk3:
            __BKPT(0);// 设置一个断点
        case keyk8:

            break;
        case keyk9:

            break;

        case keykE:

            break;
        case keykF:

            break;
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
    dac_set_value(FlashStorage::read_isr() << 4);
}


#endif