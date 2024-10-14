//
// Created by 34753 on 2024/10/10.
//

#ifndef FURINA_PLAYER_HPP
#define FURINA_PLAYER_HPP
#include "JYZQ_Conf.h"
#ifdef USE_PLAYER

// 定义播放速度枚举体
enum class PlaybackSpeed: unsigned int{
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
    static auto on()->void;
    static auto off()->void;
    static auto record_on()->void;
    static auto record_off()->void;
    static auto set_speed(PlaybackSpeed speed)->void;
};


#endif  // USE_PLAYER
#endif //FURINA_PLAYER_HPP
