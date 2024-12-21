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

static uint32_t temp = 0;


// 定义播放速度枚举体
enum class PlaybackSpeed : unsigned int
{
    Normal,
    Speed_1_5x,// 1.5倍速
    Speed_0_75x// 0.75倍速
};

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

    static auto set_speed(PlaybackSpeed speed) -> void;

    static auto saveInfo() -> void;

    static auto erase() -> void;

    static auto readData()->uint16_t ;

private:
    static uint8_t inline is_recorded = 0;// 是否正在录音
    static uint32_t inline play_addr=256;
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
    } else { is_recorded = 0xFF; }

   UI_Interface::resume_timer();

    timer2_start();
    adc1_start_it();// 开启ADC采集
}

auto Player::record_off() -> void
{
   UI_Interface::stop_timer();
    timer2_stop();
    adc1_stop_it();// 关闭ADC采集
}

auto Player::set_speed(PlaybackSpeed speed) -> void
{

}

// 存储信息
auto Player::saveInfo() -> void
{
    W25QXXFlashStorage::saveInfo();// 保存信息
}

auto Player::erase() -> void
{
    // 显示正在擦除

    // 整片擦除，省得我闹心
    w25qxx_chip_erase();

    // 显示擦除完毕
}

auto Player::readData() -> uint16_t
{
    return w25qxx_read_byte(play_addr++) << 4;
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
            Player::set_speed(PlaybackSpeed::Speed_0_75x);

        case keyk3:// 快放
            Player::set_speed(PlaybackSpeed::Speed_1_5x);

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

