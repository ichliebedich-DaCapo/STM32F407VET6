//
// Created by 34753 on 2024/9/22.
//

#ifndef FURINA_KEY_H
#define FURINA_KEY_H
#include <cstdint>


/**
 * @brief 按键类
 * @details 适配于矩阵键盘(16个键)
 * @note    基础原理：按下按键时，会触发外部中断来读取键值，此时state会标记为有效，然后由handler_task中的key.is_valid来读取state。
 *          如果有效就将进行二次判断，进而得到短按、长按状态。进而根据这些状态来处理对应的任务
 */

class Key
{
    // 成员函数
    uint8_t is_valid();// 判断按键是否有效


    // 成员变量
    uint16_t value;// 按键值,使用掩码操作
    uint8_t state;
};

extern Key key;
#endif //FURINA_KEY_H
