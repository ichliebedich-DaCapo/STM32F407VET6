//
// Created by 34753 on 2024/9/22.
//

#ifndef FURINA_KEY_HPP
#define FURINA_KEY_HPP

#include <cstdint>
#include "main.h"

#define KEY_STATE_NONE 0x0  // 无状态,无需使用
#define KEY_STATE_TWO 0x1   // 存储两个状态
#define KEY_STATE_THREE 0x2 // 存储三个状态
#define KEY_STATE_FOUR 0x3  // 存储四个状态

#define keyk0 0x0
#define keyk1 0x1
#define keyk2 0x2
#define keyk3 0x3
#define keyk4 0x4
#define keyk5 0x5
#define keyk6 0x6
#define keyk7 0x7
#define keyk8 0x8
#define keyk9 0x9
#define keykA 0xA
#define keykB 0xB
#define keykC 0xC
#define keykD 0xD
#define keykE 0xE
#define keykF 0xF

/**
 * @brief 按键类
 * @details 适配于矩阵键盘(16个键)
 * @note    先设计一个简单的短按判断，后面再说
 *          构想：
 *              按键中断（外部中断）设为双边沿触发。当按下按键时，先由下降沿触发，此时由外部中断释放信号量，按键任务接收到了信号量并记录时刻，然后每次阻塞50ms，持续4次。
 *          当松开按键时，由上升沿触发，此时由外部中断释放信号量，按键任务接收到了信号量并记录时刻。若200ms内接受到了上升沿触发得到的信号量，那么就判断为短按。如果超过200ms，
 *          那么之后每100ms判断1次长按
 */

class Key
{
public:
#ifndef APP_NO_RTOS
  constexpr  Key() : code(0), state(0) {}
#else
    constexpr  Key() : code(0), state(0),sign(0) {}
#endif
    ~Key() = default;

    // 成员函数
    static void init();

    auto setCode(uint8_t keycode) { code = keycode; }

    [[nodiscard]] auto getCode() const { return code; }// 获取键值

    [[nodiscard]] auto getState(uint8_t keycode) const { return state & (0x3 << keycode * 2); }// 获取键位状态

#ifdef APP_NO_RTOS
    auto setSign()->void {  sign = 0xFF; }// 设置标志，表明已读取
    auto handler()->void;// 反正都是阻塞式等待，也就无所谓多一步取反
#endif// APP_NO_RTOS
    auto resetState(uint8_t keycode) { state &= ~(0x3 << (keycode * 2));  /*清除指定键位的状态*/}

    uint8_t stateHandler(uint8_t maxKeyStates);

    // 私有成员变量
private:
    uint32_t state;// 按键状态,使用掩码操作，给每个按键分配两个位，用于存储最多4个状态
    uint8_t code;// 当前键值
#ifdef APP_NO_RTOS
    uint8_t sign;// 标志
#endif// APP_NO_RTOS
};

extern Key key;
#endif //FURINA_KEY_HPP