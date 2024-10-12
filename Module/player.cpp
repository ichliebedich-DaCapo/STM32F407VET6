//
// Created by 34753 on 2024/10/10.
//

#include "player.h"

#if USE_PLAYER
#include "dac.h"
#include "timer.h"
#include "adc.h"

/**
 * @brief 打开播放器
 */
auto Player::on() -> void
{
    dac_start();
    timer6_start_it();
}

/**
 * @brief 关闭播放
 */
auto Player::off() -> void
{
    dac_stop();
    timer6_stop_it();
}

/**
 * @brief 设置播放速度
 * @param speed 由播放速度枚举值决定
 */
auto Player::set_speed(PlaybackSpeed speed) -> void
{
    switch (speed)
    {
        case PlaybackSpeed::Normal:
            timer6_set_freq(83, 63);//
            break;
        case PlaybackSpeed::Speed_1_5x:
            timer6_set_freq(66, 50);//
            break;
        case PlaybackSpeed::Speed_0_75x:
            timer6_set_freq(50, 37);//
            break;
        default:
            break;
    }
}

/**
 * @brief 开启录音
 * @details 开启定时器2触发器，并开启ADC中断
 */
auto Player::record_on() -> void
{
    timer2_start();
    adc1_start_it();
}

/**
 * @brief 关闭录音
 * @details 关闭定时器2触发器，并关闭ADC中断
 */
auto Player::record_off() -> void
{
    timer2_stop();
    adc1_stop_it();
}


#endif