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
using W25QXXFlashStorage = FlashStorage<w25qxx_page_write, w25qxx_buffer_read>;


/**
 * @brief 播放器
 * @note 实现以FLash为存储载体的小型播放器功能
 * 播放：使用双缓冲机制，将数据从Flash读入到RAM中，再从RAM播放
 */
class Player
{
public:
    static auto on() -> void;

    static auto off() -> void;

    static auto record_on() -> void;

    static auto record_off() -> void;

    static auto set_speed(PlaySpeed speed) -> void;

    static auto set_record_sample_rate(RecordSampleRate sample_rate) -> void;

    static auto saveInfo() -> void;

    static auto erase() -> void;

    static auto readData() -> uint16_t;

private:
    static uint8_t inline is_recorded = 0;// 是否正在录音
    static uint32_t inline play_addr = 256;// 播放地址
    static RecordSampleRate inline record_sample_rate = RecordSampleRate::SAMPLE_RATE_8K;// 录音采样率
    static inline bool save_state = false;// 是否保存信息
};

auto Player::on() -> void
{
    // 重置时间
    UI_Interface::reset_time();
    ImageButton::press(GUI_Base::get_ui()->main.imgbtn_play);
    timer6_start_it();
    dac_start();
}

auto Player::off() -> void
{
    ImageButton::release(GUI_Base::get_ui()->main.imgbtn_play);
    timer6_stop_it();
    dac_stop();
}

auto Player::record_on() -> void
{
    if (is_recorded != 0xFF)
    {
        W25QXXFlashStorage::write_isr_pre();// 预加载
    } else
    {
        // 预加载完毕
        is_recorded = 0xFF;
    }

    UI_Interface::resume_record();

    timer2_start();
    adc1_start_it();// 开启ADC采集
}

auto Player::record_off() -> void
{
    UI_Interface::pause_record();
    timer2_stop();
    adc1_stop_it();// 关闭ADC采集
}

auto Player::set_speed(PlaySpeed speed) -> void
{
    UI_Interface::set_play_speed(speed);
    switch (speed)
    {
        case PlaySpeed::SPEED_NORMAL:
            timer6_set_freq(record_sample_rate == RecordSampleRate::SAMPLE_RATE_8K ? FREQ_84M_to_8K : FREQ_84M_to_16K);
            break;

        case PlaySpeed::SPEED_0_75:
            timer6_set_arr(timer6_get_arr() * 4 / 3);
            break;

        case PlaySpeed::SPEED_1_5:
            timer6_set_arr(timer6_get_arr() * 3 / 2);
            break;

        default:
            break;


    }
}

// 存储信息
auto Player::saveInfo() -> void
{
    W25QXXFlashStorage::saveInfo();// 保存信息
    if(!save_state)
    {
        save_state = true;
        UI_Interface::saveInfo(save_state);
    }
}

auto Player::erase() -> void
{
    // 显示正在擦除
    UI_Interface::erasing();

    // 整片擦除，省得我闹心
    w25qxx_chip_erase();

    // 显示擦除完毕
    UI_Interface::erase_done();
}

auto Player::readData() -> uint16_t
{
    return w25qxx_read_byte(play_addr++) << 4;
}

auto Player::set_record_sample_rate(RecordSampleRate sample_rate) -> void
{
    record_sample_rate = sample_rate;
    UI_Interface::set_record_state(sample_rate);
    switch (sample_rate)
    {
        case RecordSampleRate::SAMPLE_RATE_8K:
            timer2_set_freq(FREQ_84M_to_8K);
            break;

        case RecordSampleRate::SAMPLE_RATE_16K:
            timer2_set_freq(FREQ_84M_to_16K);
            break;

        default:
            break;
    }
}

/*语音存储与回放初始化*/
void app_init()
{
    spi2_init();// 初始化SPI2
    dac_init();
    adc1_init();
    w25qxx_init();// 初始化flash
    timer2_init(FREQ_84M_to_16K);// 分频为16KHz
    timer6_init(FREQ_84M_to_16K);
}

/*语音存储与回放处理函数,用于处理各种按键响应*/
void key_handler()
{
    switch (Key::getCode())
    {
        case keyk0:// 播放
            if (Key::stateHandler(2))
            {
                Player::on();
            } else
            {
                Player::off();
            }
            break;

        case keyk1:// 录音
            if (Key::stateHandler(2))
            {
                Player::record_on();
            } else
            {
                Player::record_off();
            }
            break;

        case keyk2:// 慢放
            if (Key::stateHandler(2))
            {
                Player::set_speed(PlaySpeed::SPEED_0_75);
            } else
            {
                Player::set_speed(PlaySpeed::SPEED_NORMAL);
            }


        case keyk3:// 快放
            if (Key::stateHandler(2))
            {
                Player::set_speed(PlaySpeed::SPEED_1_5);
            } else
            {
                Player::set_speed(PlaySpeed::SPEED_NORMAL);
            }

            // 录音采样率切换
        case keyk4:
            if (Key::stateHandler(2))
            {
                Player::set_record_sample_rate(RecordSampleRate::SAMPLE_RATE_8K);
            } else
            {
                //
                Player::set_record_sample_rate(RecordSampleRate::SAMPLE_RATE_16K);
            }

            // 保存信息
        case keyk5:
            Player::saveInfo();

        case keykF:
            Player::erase();

        default:
            break;
    }

}

void background_handler()
{
    W25QXXFlashStorage::background_processing();
    // 检查是否写满完毕
    if (W25QXXFlashStorage::check_write_full_sign())
    {
        Player::off();
    }
}

/**实现中断服务例程*/
// 用于采集ADC数据
void adc1_isr()
{
    // 12位舍弃低4位
    W25QXXFlashStorage::write_isr(HAL_ADC_GetValue(&hadc1) >> 4);
}

// 用于播放DAC数据
extern "C"
{
void timer6_isr()
{
    // 把8位数据恢复为12位
    dac_set_value(Player::readData());
}
}

