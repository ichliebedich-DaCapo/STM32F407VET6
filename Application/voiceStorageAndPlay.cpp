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
    timer2_init(49, 104);// 分频为16KHz
    timer6_init(49, 104);
}

/*语音存储与回放处理函数,用于处理各种按键响应*/
uint8_t test[256];
volatile uint32_t test_addr=0;
volatile uint8_t test_var = 0;
void key_handler()
{
    uint8_t test2[256];
    switch (Key::getCode())
    {
        case keyk0:// 写入1字节
            w25qxx_write_byte(test_addr, 0xAA);
            __BKPT(0);// 设置一个断点
            break;
        case keyk1:// 读取1字节
            test_var=w25qxx_read_byte(test_addr);
            __BKPT(0);// 设置一个断点
            break;
        case keyk2:// 读取256字节
            w25qxx_buffer_read(test, test_addr, 256);
            __BKPT(0);// 设置一个断点
            break;

        case keyk3:
            __BKPT(0);// 设置一个断点
            for (auto & elem : test2)
                elem=0xAA;
            w25qxx_page_write(test2, test_addr, 256);
            __BKPT(1);

        case keyk8:
            test_addr-=256;
            break;
        case keyk9:
            test_addr+=256;
            break;

        case keykE:
            --test_addr;
            break;
        case keykF:
            ++test_addr;
            break;
        default:
            break;
    }





/*    switch (Key::getCode())
    {
        case keyk0:// 播放
*//*            if (Key::stateHandler(2))
            {
            ImageButton::press(GUI_Base::get_ui()->main.imgbtn_play);
//                FlashStorage::read_isr_ready();// 预加载
//                Player::on();
            } else
            {
              ImageButton::release(GUI_Base::get_ui()->main.imgbtn_play);
//                Player::off();
            }*//*
            break;

        case keyk1:// 录音

////            ITM_SendChar('#');
//            if (Key::stateHandler(2))
//            {
//                FlashStorage::write_isr_ready();// 预加载
//                Player::record_on();
//            } else
//            {
//                Player::record_off();
//            }
            break;
//
//        case keyk2:// 慢放
//            Player::set_speed(PlaybackSpeed::Speed_0_75x);
//
//        case keyk3:// 快放
//            Player::set_speed(PlaybackSpeed::Speed_1_5x);

        default:
            break;
    }*/
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