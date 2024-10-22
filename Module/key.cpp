//
// Created by fairy on 2024/9/22.
//
#include "key.hpp"
#include "key_exit.h"
#include "stm32f4xx_hal.h"

extern void key_handler();
__weak void key_handler(){/*由用户自己实现*/}

/**
 * 设置按键状态函数
 * @param maxKeyStates 设置存储按键状态的最大值，最大为4
 * @return
 * @note 真是见鬼！不知道当初怎么想到的
 *      功能：存储按键状态，每次按下按键，状态值加1，超过最大值后重置为0。
 *      比如你设为0（相当于不用这个函数），那么每次按下按键都会返回1。如果你设为1，那么每两次按下按键就会返回1，就像开关一样
 *      if(key.stateHandler(0)){}
 */
uint8_t Key::stateHandler(uint8_t maxKeyStates)
{
    uint8_t temp = (state >> (code * 2)) & 0x3;//取出对应键位的值
    temp = ((temp + 1) & 0x3) % maxKeyStates;//切换状态
    state = (state & ~(0x3 << (code * 2))) | (temp << (code * 2));//设置新状态，先清零再设置
    return temp;
}

void Key::init()
{
    key_exti_init();
}


#ifdef APP_NO_RTOS
auto Key::handler()->void
{
    if(sign)
    {
        sign = 0;
        key_handler();
    }
}
#endif






