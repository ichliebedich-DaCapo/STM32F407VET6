#include <cmath>
#include <unistd.h>
#include "lvgl.h"
#include "GUI.hpp"
#include "GUI_Base.hpp"
#include "lv_drivers/display/monitor.h"
#include "WaveCurve.hpp"
#include "lv_drivers/indev/keyboard.h"

// 真它宝贝的阴险，一个该死的宏从SDL_main里泄露到这里，fxxk
#undef main


extern volatile int keep_running;
/*******************************自定义代码**********************************/
constexpr uint16_t Sample_Size = 128;
constexpr uint16_t chart_width = 320;
constexpr uint16_t chart_height = 200;
constexpr uint32_t COLOR_BLACK = 0x0000;
constexpr uint32_t COLOR_WHITE = 0xFFFFFF;
constexpr uint16_t start_x = 80;
constexpr uint16_t start_y = 60;
constexpr uint16_t max_value = 255;

uint8_t TestBuf[Sample_Size];
const uint8_t sine_wave[128] = {
        128, 134, 140, 146, 152, 158, 165, 170, 176, 182, 188, 193, 198, 203, 208, 213, 218, 222, 226, 230, 234, 237,
        240, 243, 245, 248, 250, 251, 253, 254, 254, 255, 255, 255, 254, 254, 253, 251, 250, 248, 245, 243, 240, 237,
        234, 230, 226, 222, 218, 213, 208, 203, 198, 193, 188, 182, 176, 170, 165, 158, 152, 146, 140, 134, 127, 121,
        115, 109, 103, 97, 90, 85, 79, 73, 67, 62, 57, 52, 47, 42, 37, 33, 29, 25, 21, 18, 15, 12, 10, 7, 5, 4, 2, 1, 1,
        0, 0, 0, 1, 1, 2, 4, 5, 7, 10, 12, 15, 18, 21, 25, 29, 33, 37, 42, 47, 52, 57, 62, 67, 73, 79, 85, 90, 97, 103,
        109, 115, 121
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
    (void) data;  /* 忽略传递的参数，因为在这个函数中不需要使用它 */
    static bool is_fps_mode = false;
    bool is_fps = false;
    bool is_generate = false;
    while (keep_running)
    {
        uint32_t keycode;
        if (keyboard_get_input(&keycode))
        {
            while (keyboard_get_state());//等待按键释放
            switch (keycode)
            {
                case 'a':
                    is_fps = !is_fps;
                    uiInterface::show_fps(is_fps);
                    printf("FPS显示\n");
                    break;

                case 's':
                    is_fps_mode = !is_fps_mode;
                    uiInterface::set_fps_mode(is_fps_mode);
                    printf("FPS模式\n");
                    break;

                case 'd':
                    uiInterface::sub_period();
                    printf("上一个波形\n");
                    break;

                case 'f':
                    uiInterface::add_period();
                    printf("下一个波形\n");
                    break;


                case 'g':
                    uiInterface::switch_wave_type();
                    printf("E pressed\n");
                    break;

                case 'h':
                    uiInterface::add_wave_cnt();
                    printf("F pressed\n");
                    break;

                case 'j':
                    is_generate = !is_generate;
                    if (is_generate)
                        uiInterface::wave_is_generate();
                    else
                        uiInterface::wave_is_not_generate();

                default:
                    break;
            }
        }
    }

    return 0;  /* 这个函数实际上永远不会返回，因为它一直在循环中运行 */
}

