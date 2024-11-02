#include <cmath>
#include "lvgl.h"
#include "GUI.hpp"
#include "lv_drivers/display/monitor.h"
#include "WaveCurve.hpp"

/*******************************自定义代码**********************************/
constexpr uint16_t Sample_Size = 256;
constexpr uint16_t chart_width = 320;
constexpr uint16_t chart_height = 200;
constexpr uint32_t COLOR_BLACK = 0x0000;
constexpr uint32_t COLOR_WHITE = 0xFFFFFF;
constexpr uint16_t start_x = 80;
constexpr uint16_t start_y = 60;
constexpr uint16_t max_value = 255;

uint16_t Buf[Sample_Size];
const uint8_t sine_wave[128] = {
        128, 134, 140, 146, 152, 158, 165, 170, 176, 182, 188, 193, 198, 203, 208, 213, 218, 222, 226, 230, 234, 237,
        240, 243, 245, 248, 250, 251, 253, 254, 254, 255, 255, 255, 254, 254, 253, 251, 250, 248, 245, 243, 240, 237,
        234, 230, 226, 222, 218, 213, 208, 203, 198, 193, 188, 182, 176, 170, 165, 158, 152, 146, 140, 134, 127, 121,
        115, 109, 103, 97, 90, 85, 79, 73, 67, 62, 57, 52, 47, 42, 37, 33, 29, 25, 21, 18, 15, 12, 10, 7, 5, 4, 2, 1, 1,
        0, 0, 0, 1, 1, 2, 4, 5, 7, 10, 12, 15, 18, 21, 25, 29, 33, 37, 42, 47, 52, 57, 62, 67, 73, 79, 85, 90, 97, 103,
        109, 115, 121
};



// 用于在main中执行
void test_handler()
{
    static uint8_t cnt = 0;
    WaveCurve::draw_curve<uint16_t, uint32_t, uint16_t, 4>(WaveCurve::draw_CatmullRomSp_line, Buf,
                                                           sine_wave[(cnt++) % 128],
                                                           Sample_Size, start_x, start_y, chart_width,
                                                           chart_height, max_value, COLOR_WHITE, COLOR_BLACK);
}

