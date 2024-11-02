//
// Created by fairy on 2024/11/3 05:52.
//
#ifndef SIMULATOR_WAVECURVE_HPP
#define SIMULATOR_WAVECURVE_HPP

#include "Module_Conf.h"
#ifdef ARM_MATH_CM4
#include "lcd.h"
#else
#include "lv_drivers/display/monitor.h"
#endif
/* 预编译命令 */
#include <cstring> // 引入string.h以使用memmove
#include <valarray>

/**
 * @brief 绘制波形曲线
 */
class WaveCurve
{
public:
    // 绘制曲线_插值画线
    template<typename Data, typename Color,  typename Coord=uint16_t, uint8_t once_points = 2>
    static auto
    draw_curve(void(*DrawFunc)(const Data *, const Data *, Color), Data data[], Data value, Coord N, Coord Start_x,
               Coord Start_y,
               Coord Width,
               Coord Height, Data Max_Value, uint32_t bg_color, uint32_t color) -> void;

    // 绘制函数
    template<typename Data,typename Color>
    static inline auto draw_interpolated_line(const Data *x, const Data *y, Color color) -> void;

    template<typename Data,typename Color>
    static inline auto draw_BezierCurve2(const Data *x, const Data *y, Color color) -> void;

    template<typename Data,typename Color>
    static inline auto draw_BezierCurve3(const Data *x, const Data *y, Color color) -> void;

    template<typename Data,typename Color>
    static inline auto draw_CatmullRomSp_line(const Data *x, const Data *y, Color color)-> void;

private:
//    static constexpr uint16_t ax_Value = 4096;
};

// 插值画线函数
template<typename Data,typename Color>
auto WaveCurve::draw_interpolated_line(const Data *x, const Data *y, Color color) -> void
{
    // Bresenham's line algorithm
    uint16_t x0 = x[0], y0 = y[0], x1 = x[1], y1 = y[1];
    int dx = (abs(x1 - x0)), sx = x0 < x1 ? 1 : -1;
    int dy = (-abs(y1 - y0)), sy = y0 < y1 ? 1 : -1;
    int err = (dx + dy), e2;

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

/**
 * @brief 绘制曲线  辣鸡！辣鸡！辣鸡！ 辣鸡模板，难用得一批
 * @tparam Data 缓冲数组类型,也是数据类型
 * @辣鸡 DrawFunc 绘制函数 说得就是这个，连个函数指针都tm推导不出来
 * @tparam Coord 坐标类型
 * @tparam once_points 一次绘制的点数
 * @param value 待绘制的值
 * @param N  总显示点数
 * @param Start_x 绘制区域的起点横坐标
 * @param Start_y 绘制区域的起点纵坐标
 * @param Width 绘制区域的宽度
 * @param Height 绘制区域的高度
 * @param Max_Value 样本数据的最大值
 */
template<typename Data, typename Color, typename Coord, uint8_t once_points>
auto
WaveCurve::draw_curve(void(*DrawFunc)(const Data *, const Data *, Color), Data data[], Data value, Coord N,
                      Coord Start_x,
                      Coord Start_y,
                      Coord Width,
                      Coord Height, Data Max_Value,uint32_t bg_color,uint32_t color) -> void
{
    if (N < once_points) return; // 如果数据点过少，则无法画线

    // 计算每个数据点之间的水平距离
    Coord step = Width / (N - 1);// 步长
    Data x[once_points], y[once_points + 1];

    // 从右往左刷新，更符合直观上的感受
    for (int i = N - once_points; i >= 0; --i)
    {
        // 确认旧点
        for (int j = 0; j < once_points; ++j)
        {
            x[j] = (Coord) (Start_x + (i + j) * step);
            y[j] = Start_y + Height - (data[i + j] * (Height - 1) / Max_Value);
        }

        // 确认新点
        if (i == N - once_points)
        {
            y[once_points] = Start_y + Height - (value * (Height - 1) / Max_Value);// 反转y轴以匹配屏幕坐标系
        } else
        {
            y[once_points] = Start_y + Height - (data[i + once_points] * (Height - 1) / Max_Value);
        }

        // 更新曲线
        DrawFunc(x, y, bg_color);// 清除前面曲线
        DrawFunc(x, y + 1, color);// 绘制新曲线
    }

    /******更新数据******/
    memmove(&data[0], &data[1], (N - 1) * sizeof(data[0]));
    data[N - 1] = value;
}


/**
 * @brief 绘制二次贝塞尔曲线
 * @note 需要把模板参数里的once_points参数设置为3，因为二次贝塞尔曲线需要三个控制点
 * @param x
 * @param y
 * @param color
 */
template<typename Data,typename Color>
auto WaveCurve::draw_BezierCurve2(const Data *x, const Data *y, Color color) -> void
{
    // 定义步长，决定曲线的平滑度
    constexpr float step = 0.01;
    for (float t = 0.0; t <= 1.0; t += step)
    {
        // 转换浮点数坐标到整数
        uint16_t px, py;
        px = (uint16_t) ((1 - t) * (1 - t) * x[0] + 2 * (1 - t) * t * x[1] + t * t * x[2]);
        py = (uint16_t) ((1 - t) * (1 - t) * y[0] + 2 * (1 - t) * t * y[1] + t * t * y[2]);
        // 设置像素点
        LCD_Set_Pixel(px, py, color);
    }
}

/**
 * @brief 绘制三次贝塞尔曲线
* @note 需要把模板参数里的once_points参数设置为3，因为三次贝塞尔曲线需要四个控制点
 * @param x
 * @param y
 * @param color
 */
template<typename Data,typename Color>
auto WaveCurve::draw_BezierCurve3(const Data *x, const Data *y, Color color) -> void
{
    // 定义步长，决定曲线的平滑度
    constexpr float step = 0.01;
    for (float t = 0.0; t <= 1.0; t += step)
    {
        // 转换浮点数坐标到整数
        uint16_t px, py;
        px = (uint16_t) ((1 - t) * (1 - t) * (1 - t) * x[0] + 3 * (1 - t) * (1 - t) * t * x[1] +
                         3 * (1 - t) * t * t * x[2] + t * t * t * x[3]);
        py = (uint16_t) ((1 - t) * (1 - t) * (1 - t) * y[0] + 3 * (1 - t) * (1 - t) * t * y[1] +
                         3 * (1 - t) * t * t * y[2] + t * t * t * y[3]);

        // 设置像素点
        LCD_Set_Pixel(px, py, color);
    }
}

/**
 * @brief 绘制 Catmull-Rom 样条曲线
 * @note 需要把模板参数里的once_points参数设置为4，因为 Catmull-Rom 样条曲线需要四个控制点
 * @param x
 * @param y
 * @param color
 */
template<typename Data,typename Color>
auto WaveCurve::draw_CatmullRomSp_line(const Data *x, const Data *y, Color color) -> void
{
    constexpr float step = 0.01;

    for (float t = 0.0; t <= 1.0; t += step)
    {
        float t2 = t * t;
        float t3 = t2 * t;
        // 转换浮点数坐标到整数
        auto px = (uint16_t) (0.5 *
                              (2 * x[1] + (-x[0] + x[1]) * t + (2 * x[0] - 5 * x[1] + 4 * x[2] - x[3]) * t2 +
                               (-x[0] + 3 * x[1] - 3 * x[2] + x[3]) * t3));
        auto py = (uint16_t) (0.5 * (2 * y[1] +
                                     (-y[0] + y[2]) * t +
                                     (2 * y[0] - 5 * y[1] + 4 * y[2] - y[3]) * t2 +
                                     (-y[0] + 3 * y[1] - 3 * y[2] + y[3]) * t3));
        // 设置像素点
        LCD_Set_Pixel(px, py, color);
    }
}
#endif //SIMULATOR_WAVECURVE_HPP