#include <cmath>
#include "lvgl.h"
#include "GUI.hpp"
#include "lv_drivers/display/monitor.h"


/*******************************自定义代码**********************************/
constexpr uint16_t Sample_Size = 256;
constexpr uint16_t chart_width = 320;
constexpr uint16_t chart_height = 200;
constexpr uint32_t COLOR_BLACK = 0x0000;
constexpr uint32_t COLOR_WHITE = 0xFFFFFF;
constexpr uint16_t start_x = 80;
constexpr uint16_t start_y = 60;
constexpr uint16_t max_value = 255;

uint16_t Data[Sample_Size];
const uint8_t sine_wave[128] = {
        128, 134, 140, 146, 152, 158, 165, 170, 176, 182, 188, 193, 198, 203, 208, 213, 218, 222, 226, 230, 234, 237,
        240, 243, 245, 248, 250, 251, 253, 254, 254, 255, 255, 255, 254, 254, 253, 251, 250, 248, 245, 243, 240, 237,
        234, 230, 226, 222, 218, 213, 208, 203, 198, 193, 188, 182, 176, 170, 165, 158, 152, 146, 140, 134, 127, 121,
        115, 109, 103, 97, 90, 85, 79, 73, 67, 62, 57, 52, 47, 42, 37, 33, 29, 25, 21, 18, 15, 12, 10, 7, 5, 4, 2, 1, 1,
        0, 0, 0, 1, 1, 2, 4, 5, 7, 10, 12, 15, 18, 21, 25, 29, 33, 37, 42, 47, 52, 57, 62, 67, 73, 79, 85, 90, 97, 103,
        109, 115, 121
};

#include <cstring> // 引入string.h以使用memmove


void roll_and_update(uint16_t newdata, uint16_t N)
{
    if (N > 1)
    {
        memmove(&Data[0], &Data[1], (N - 1) * sizeof(Data[0]));
        Data[N - 1] = newdata;
    }
}

// 插值画线函数
void
draw_interpolated_line(uint16_t *data, uint16_t N, uint16_t Start_x, uint16_t Start_y, uint16_t Width, uint16_t Height,
                       uint16_t Max_Value, uint32_t color)
{
    if (N < 2) return; // 如果数据点少于2个，则无法画线

    // 计算每个数据点之间的水平距离
    uint16_t step = Width / (N - 1);

    for (uint16_t i = 0; i < N - 1; ++i)
    {
        // 当前点和下一个点的坐标
        uint16_t x0 = Start_x + i * step;
        uint16_t y0 = Start_y + Height - (data[i] * (Height - 1) / Max_Value); // 反转y轴以匹配屏幕坐标系
        uint16_t x1 = Start_x + (i + 1) * step;
        uint16_t y1 = Start_y + Height - (data[i + 1] * (Height - 1) / Max_Value);

        // Bresenham's line algorithm
        int16_t dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
        int16_t dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
        int16_t err = dx + dy, e2;

        for (;;)
        {
            LCD_Set_Pixel(x0, y0, color);
            if (x0 == x1 && y0 == y1) break;
            e2 = 2 * err;
            if (e2 >= dy)
            {
                err += dy;
                x0 += sx;
            }
            if (e2 <= dx)
            {
                err += dx;
                y0 += sy;
            }
        }
    }
}

//void update_wave(uint16_t value)
//{
//    // 更新数据
//    draw_interpolated_line(Data, Sample_Size, start_x, start_y, chart_width, chart_height, max_value, COLOR_WHITE);
//    roll_and_update(value, Sample_Size);
//    draw_interpolated_line(Data, Sample_Size, start_x, start_y, chart_width, chart_height, max_value, COLOR_BLACK);
//}
void update_wave(uint16_t value, uint16_t N, uint16_t Start_x, uint16_t Start_y, uint16_t Width, uint16_t Height,
                 uint16_t Max_Value)
{
    /******清除前面数据******/
    if (N < 2) return; // 如果数据点少于2个，则无法画线

    // 计算每个数据点之间的水平距离
    uint16_t step = Width / (N - 1);

    for (uint16_t i = 0; i < N - 1; ++i)
    {
        // 当前点和下一个点的坐标
        uint16_t x0 = Start_x + i * step;
        uint16_t y0 = Start_y + Height - (Data[i] * (Height - 1) / Max_Value); // 反转y轴以匹配屏幕坐标系
        uint16_t x1 = Start_x + (i + 1) * step;
        uint16_t y1 = Start_y + Height - (Data[i + 1] * (Height - 1) / Max_Value);

        // Bresenham's line algorithm
        int16_t dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
        int16_t dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
        int16_t err = dx + dy, e2;

        for (;;)
        {
            LCD_Set_Pixel(x0, y0, COLOR_WHITE);
            if (x0 == x1 && y0 == y1) break;
            e2 = 2 * err;
            if (e2 >= dy)
            {
                err += dy;
                x0 += sx;
            }
            if (e2 <= dx)
            {
                err += dx;
                y0 += sy;
            }
        }
    }

    /******更新数据******/

    memmove(&Data[0], &Data[1], (N - 1) * sizeof(Data[0]));
    Data[N - 1] = value;

    /******绘制数据******/
    for (uint16_t i = 0; i < N - 1; ++i)
    {
        // 当前点和下一个点的坐标
        uint16_t x0 = Start_x + i * step;
        uint16_t y0 = Start_y + Height - (Data[i] * (Height - 1) / Max_Value); // 反转y轴以匹配屏幕坐标系
        uint16_t x1 = Start_x + (i + 1) * step;
        uint16_t y1 = Start_y + Height - (Data[i + 1] * (Height - 1) / Max_Value);

        // Bresenham's line algorithm
        int16_t dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
        int16_t dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
        int16_t err = dx + dy, e2;

        for (;;)
        {
            LCD_Set_Pixel(x0, y0, COLOR_BLACK);
            if (x0 == x1 && y0 == y1) break;
            e2 = 2 * err;
            if (e2 >= dy)
            {
                err += dy;
                x0 += sx;
            }
            if (e2 <= dx)
            {
                err += dx;
                y0 += sy;
            }
        }
    }
}

// 用于在main中执行
void test_handler()
{
    static uint8_t cnt = 0;
//    update_wave(sine_wave[(cnt++) % 128]);
    update_wave(sine_wave[(cnt++) % 128], Sample_Size, start_x, start_y, chart_width, chart_height, max_value);
}

