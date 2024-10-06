//
// Created by 34753 on 2024/9/22.
//

#include "key.h"

// 实例化，由于我更喜欢通过“.”来访问成员变量，所以没有使用单例模式
Key key;

enum
{
    KEY_RELEASED=0,// 松开
    KEY_PRESSED=1,// 按下

};

// 判断按键是否被按下
unsigned char Key::is_valid()
{
    switch (state)
    {
        case KEY_PRESSED:
            state =KEY_RELEASED;
            return KEY_PRESSED;

        case KEY_RELEASED:
        default:
            return KEY_RELEASED;
    }
}
