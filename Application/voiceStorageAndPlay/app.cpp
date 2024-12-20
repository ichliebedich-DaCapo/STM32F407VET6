//
// Created by fairy on 2024/9/22.
//

#include "app.hpp"


#include "lvgl.h"
#include "key.hpp"
#include "spi.h"
#include "timer.h"
#include "adc.h"
#include "dac.h"
#include "player.hpp"
#include "GUI.hpp"
#include "w25qxx.h"

// 导入算法类
import flash_storage;
//const osThreadAttr_t voiceTask_attributes = {
//        .name = "voiceTask",
//        .stack_size = 128 * 4,
//        .priority = (osPriority_t) osPriorityNormal,
//};
//void VoiceTask(void *argument);


// 使用别名
using W25QXXFlashStorage = FlashStorage<w25qxx_page_write>;

static uint32_t temp=0;
static uint8_t temp_buffer[512];
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
            // 测试预写入
            W25QXXFlashStorage::write_isr_pre(259);
            break;
        case keyk1:// 读取1字节
            // 测试写入情况
            for (int i = 0; i < 260; ++i)
            {
                W25QXXFlashStorage::write_isr(temp++);
            }
            break;
        case keyk2:// 读取256字节
        // 测试读取情况
            w25qxx_buffer_read(temp_buffer,temp<256?0:temp-256,512);
            __BKPT(0);
            break;

        case keyk3:

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
    W25QXXFlashStorage::background_processing();
}

/**实现中断服务例程*/
// 用于采集ADC数据
void adc1_isr()
{
    // 12位舍弃低4位
//    FlashStorage::write_isr(HAL_ADC_GetValue(&hadc1) >> 4);
}

// 用于播放DAC数据
void timer6_isr()
{
    // 把8位数据恢复为12位
//    dac_set_value(FlashStorage::read_isr() << 4);
}

