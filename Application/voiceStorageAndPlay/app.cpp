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

enum class PlayerState : uint8_t
{
    Normal,// 正常状态，可以播放或录音
    Play,// 播放状态
    Record,// 录音状态,转为Normal状态
    Erase,// 擦除状态，不能做任何事情
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

    static auto set_speed(PlaySpeed speed) -> void;

    static auto set_record_sample_rate(RecordSampleRate sample_rate) -> void;

    static auto saveInfo() -> void;

    static auto erase() -> void;

    static auto readData() -> uint16_t;

    static auto get_player_state() -> PlayerState;

    static auto set_player_state(PlayerState state) -> void;

    static auto reset_play() -> void;

    // 获取是否录过音
    static auto get_recorded_state()->bool{return is_recorded;}

    // 设置是否录过音
    static auto set_recorded_state(bool state)->void{is_recorded = state;}

    static auto get_save_state()->bool{return save_state;}
    static auto set_save_state(bool state)->void{save_state = state;}


private:
    static uint8_t inline is_recorded = 0;// 是否录过音，如果录过，那么采样率不能再变了
    static uint32_t inline play_addr = 256;// 播放地址
    static RecordSampleRate inline record_sample_rate = RecordSampleRate::SAMPLE_RATE_8K;// 录音采样率
    static inline bool save_state = false;// 是否保存信息
    static inline PlayerState player_state = PlayerState::Normal;
    static inline uint32_t play_time = 0;
    static inline uint32_t record_time = 0;
};

auto Player::on() -> void
{
    ImageButton::press(GUI_Base::get_ui()->main.imgbtn_play);
    UI_Interface::set_time(play_time);// 设置当前播放时间
    timer6_start_it();
    dac_start();
}

auto Player::off() -> void
{
    ImageButton::release(GUI_Base::get_ui()->main.imgbtn_play);
    play_time = UI_Interface::get_time();// 获取当前播放时间
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
    UI_Interface::set_time(record_time);// 设置当前录音时间
    timer2_start();
    adc1_start_it();// 开启ADC采集
}

auto Player::record_off() -> void
{
    UI_Interface::pause_record();
    record_time = UI_Interface::get_time();// 获取当前录音时间
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
            timer6_set_arr(timer6_get_arr() * 2/3);
            break;

        default:
            break;


    }
}

// 存储信息
auto Player::saveInfo() -> void
{
    W25QXXFlashStorage::saveInfo(static_cast<uint8_t>(record_sample_rate));// 保存信息
    if (!save_state)
    {
        save_state = true;
        UI_Interface::saveInfo(save_state);
    }
}

auto Player::erase() -> void
{
    // 显示正在擦除
    UI_Interface::erasing();

    // 重置时间
    play_time =0;

    // 整片擦除，省得我闹心
    w25qxx_async_chip_erase();

}

auto Player::readData() -> uint16_t
{
    if(play_addr >= W25QXXFlashStorage::get_addr())
    {
        Player::off();
    }

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
            timer6_set_freq(FREQ_84M_to_8K);
            break;

        case RecordSampleRate::SAMPLE_RATE_16K:
            timer2_set_freq(FREQ_84M_to_16K);
            timer6_set_freq(FREQ_84M_to_16K);
            break;

        default:
            break;
    }
}

auto Player::get_player_state() -> PlayerState
{
    return player_state;
}

auto Player::set_player_state(PlayerState state) -> void
{
    player_state = state;
}

auto Player::reset_play() -> void
{
    play_addr = 256;
    play_time = 0;
    UI_Interface::reset_time();
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
            if (Player::get_player_state() == PlayerState::Erase)
                return;

            if (Key::stateHandler(2))
            {
                if (Player::get_player_state() == PlayerState::Record)
                {
                    Player::record_off();// 关闭录音
                    Key::resetState(keyk1);// 重置按键1的状态
                }
                Player::set_player_state(PlayerState::Play);
                Player::on();
            } else
            {
                Player::set_player_state(PlayerState::Normal);
                Player::off();
            }
            break;

        case keyk1:// 录音
            if (Player::get_player_state() == PlayerState::Erase)
                return;

            if(Player::get_save_state())
                return;

            if (Key::stateHandler(2))
            {
                if (Player::get_player_state() == PlayerState::Play)
                {
                    Player::off();// 关闭播放
                    Key::resetState(keyk0);// 重置按键0的状态
                }
                Player::set_player_state(PlayerState::Record);

                // 设置为录过音
                if(!Player::get_recorded_state())
                {
                    Player::set_recorded_state(true);
                }

                Player::record_on();
            } else
            {
                Player::set_player_state(PlayerState::Normal);
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
            break;

        case keyk3:// 快放
            if (Key::stateHandler(2))
            {
                Player::set_speed(PlaySpeed::SPEED_1_5);
            } else
            {
                Player::set_speed(PlaySpeed::SPEED_NORMAL);
            }
            break;
            // 录音采样率切换
        case keyk4:
            // 如果已经录制过，则不能切换采样率
            if(Player::get_recorded_state())
                return;

            if (Key::stateHandler(2))
            {
                Player::set_record_sample_rate(RecordSampleRate::SAMPLE_RATE_16K);
            } else
            {
                //
                Player::set_record_sample_rate(RecordSampleRate::SAMPLE_RATE_8K);
            }
            break;

            // 保存信息
        case keyk5:
            Player::saveInfo();
            break;

            // 重置播放
        case keyk8:
            Player::reset_play();
            break;

        case keykF:
            if(Player::get_player_state()==PlayerState::Erase) { return; }
            else if(Player::get_player_state()==PlayerState::Play) { Player::off(); }
            else if (Player::get_player_state()==PlayerState::Record){ Player::record_off();}

            // 重置按键状态
            Key::resetState(keyk0);
            Key::resetState(keyk1);

            Player::set_player_state(PlayerState::Erase);
            Player::erase();
            break;

        default:
            break;
    }

}

static uint32_t temp_addr;
void background_handler()
{
    W25QXXFlashStorage::background_processing();
    // 检查是否写满完毕
    if (W25QXXFlashStorage::check_write_full_sign())
    {
        Player::record_off();
    }
    // 检测是否擦除状态是否已完成
    if (Player::get_player_state() == PlayerState::Erase)
    {
        if (!w25qxx_async_is_busy())
        {
            Player::set_player_state(PlayerState::Normal);

            // 显示擦除完毕
            UI_Interface::erase_done();
        }
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

