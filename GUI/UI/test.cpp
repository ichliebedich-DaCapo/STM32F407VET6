/**
 *  @brief 测试用的文件
 *  @note 本文件一是为了能加载出UI这个文件夹，二为测试，给模拟器开一些“洞”，以更自由地测试。
 */


#include <cmath>
#include <unistd.h>
#include "lvgl.h"
#include "GUI.hpp"
#include "ui.hpp"
#include "simulator.hpp"
#include "WaveCurve.hpp"
#include <SDL2/SDL.h>
#include <windows.h>
#include <iostream>
#include <unordered_map>
#include <functional>
#include <utility>

#undef main


class KeyboardInput
{
public:
    // 构造函数
    KeyboardInput() = default;

    // 添加按键检测
    void addKeyCheck(char key, std::function<void()> callback)
    {
        keyStates[key] = false; // 初始化按键状态
        keyCallbacks[key] = std::move(callback);
    }

    // 更新按键状态
    void update()
    {
        for (auto &[key, state]: keyStates)
        {
            bool currentState = (GetKeyState(key) & 0x8000) != 0;

            // 检测按键按下事件
            if (currentState && !state)
            {
                if (keyCallbacks.find(key) != keyCallbacks.end())
                {
                    keyCallbacks[key](); // 调用回调函数
                }
            }

            state = currentState; // 更新状态
        }
    }

private:
    std::unordered_map<char, bool> keyStates; // 按键状态
    std::unordered_map<char, std::function<void()>> keyCallbacks; // 按键回调函数
};

/**
 * 键盘线程
 * @param data
 * @return
 * @note 这样可以使得我调试的手段更多了，可以不必大费周章得绑定组件到对应的事件回调里。当然这也是因为我的单片机上是矩阵键盘，
 *      暂时不太方便添加到lvgl的输入设备里，因为按键我是采用外部中断触发，并且需要完成一些外设的控制和部分组件的属性调整或部分组件的开启。
 */
int keyboard_thread(void *data)
{
    // 创建 KeyboardInput 对象
    KeyboardInput keyboard;

    // 添加按键检测
    keyboard.addKeyCheck('A', []()
    {
        std::cout << "A pressed" << std::endl;
        UI::pressA();
    });

    keyboard.addKeyCheck('S', []()
    {
        std::cout << "S pressed" << std::endl;
        UI::pressS();
    });

    keyboard.addKeyCheck('D', []()
    {

    });

    keyboard.addKeyCheck('F', []()
    {

    });

    keyboard.addKeyCheck('G', []()
    {

    });

    keyboard.addKeyCheck('H', []()
    {

    });

    keyboard.addKeyCheck('J', []()
    {

    });



    // 主循环
    while (simulator_is_running())
    {
        keyboard.update(); // 更新按键状态
        SDL_Delay(30); // 避免 CPU 占用过高
    }
    return 0;
}
