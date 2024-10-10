//
// Created by 34753 on 2024/10/10.
//
/**
 * @brief 播放器
 * @note 实现以FLash为存储载体的小型播放器功能
 * 播放：使用双缓冲机制，将数据从Flash读入到RAM中，再从RAM播放
 */
#ifndef FURINA_PLAYER_H
#define FURINA_PLAYER_H

#include "JYZQ_Conf.h"

#if USE_PLAYER

#define BUFFER_SIZE 256

class Player
{
public:
    static auto init() -> void;
//    auto play(uint32_t addr) -> void;

private:
    static uint8_t buffer1[BUFFER_SIZE];
    static uint8_t buffer2[BUFFER_SIZE];
};


#endif


#endif //FURINA_PLAYER_H
