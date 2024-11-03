//
// Created by fairy on 2024/11/3 05:52.
//
#ifndef SIMULATOR_WAVECURVE_HPP
#define SIMULATOR_WAVECURVE_HPP

#include "Module_Conf.h"

/* 预编译命令 */
// 用于区分模拟器和真机
#ifdef ARM_MATH_CM4

#include "lcd.h"

#else

#include "lv_drivers/display/monitor.h"

#endif


#include <cstring> // 引入string.h以使用memmove
#include <valarray>

/**
 * @brief 绘制波形曲线
 */
class WaveCurve
{
public:
    // 绘制曲线_插值画线
    template<typename Data, typename Color, typename Coord=uint16_t, uint8_t once_points = 2>
    static auto
    draw_curve(void(*DrawFunc)(const Coord *, const Coord *, Color), Data data[], Data value, Coord N, Coord Start_x,
               Coord Start_y,
               Coord Width,
               Coord Height, Data Max_Value, Color bg_color, Color color) -> void;

    // 绘制函数
    template<typename Coord, typename Color>
    static inline auto draw_interpolated_line(const Coord *x, const Coord *y, Color color) -> void;

    template<typename Coord, typename Color>
    static inline auto draw_BezierCurve2(const Coord *x, const Coord *y, Color color) -> void;

    template<typename Coord, typename Color>
    static inline auto draw_BezierCurve3(const Coord *x, const Coord *y, Color color) -> void;

    template<typename Coord, typename Color>
    static inline auto draw_CatmullRomSp_line(const Coord *x, const Coord *y, Color color) -> void;

private:
    // 定义步长，决定曲线的平滑度
    static constexpr float smoothness = 0.1;
};


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
WaveCurve::draw_curve(void(*DrawFunc)(const Coord *, const Coord *, Color), Data data[], Data value, Coord N,
                      Coord Start_x,
                      Coord Start_y,
                      Coord Width,
                      Coord Height, Data Max_Value, Color bg_color, Color color) -> void
{
    if (N < once_points) return; // 如果数据点过少，则无法画线

    Coord x[once_points], y[once_points + 1];
    float ratio = (Height - 1.0f) / Max_Value;
    float step = Width / (N - 1.0f);// 步长

    // 从右往左刷新，更符合直观上的感受
    for (int i = N - once_points; i >= 0; --i)
    {
        // 确认旧点
        for (int j = 0; j < once_points; ++j)
        {
            x[j] = (Coord) (Start_x + (float) (i + j) * step);
            y[j] = (Coord) (Start_y + Height - (data[i + j] * ratio));
        }

        // 确认新点
        y[once_points] = (Coord) (Start_y + Height - (i != N - once_points ? data[i + once_points] : value) * ratio);

        // 更新曲线
        DrawFunc(x, y, bg_color);// 清除前面曲线
        DrawFunc(x, y+1, color);// 绘制新曲线
    }

/******更新数据******/
    memmove(data, data + 1, (N - 1) * sizeof(data[0]));
    data[N - 1] = value;
}

// 插值画线函数
template<typename Coord, typename Color>
auto WaveCurve::draw_interpolated_line(const Coord *x, const Coord *y, Color color) -> void
{
    // Bresenham's line algorithm
    Coord x0 = x[0], y0 = y[0], x1 = x[1], y1 = y[1];
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
//    LCD_Set_Pixel(x[0], y[0], color);
//    LCD_Set_Pixel(x[1], y[1], color);
}

/**
 * @brief 绘制二次贝塞尔曲线
 * @note 需要把模板参数里的once_points参数设置为3，因为二次贝塞尔曲线需要三个控制点
 * @param x
 * @param y
 * @param color
 */
template<typename Coord, typename Color>
auto WaveCurve::draw_BezierCurve2(const Coord *x, const Coord *y, Color color) -> void
{
    float t = 0.0f;
    Coord px, py;
    while (t <= 1.0f)
    {
        float t2 = t * t;
        // 转换浮点数坐标到整数
        px = (Coord) ((1 - t) * (1 - t) * x[0] + 2 * (1 - t) * t * x[1] + t2 * x[2]);
        py = (Coord) ((1 - t) * (1 - t) * y[0] + 2 * (1 - t) * t * y[1] + t2 * y[2]);
        // 设置像素点
        LCD_Set_Pixel(px, py, color);
        t += smoothness;
    }
}

/**
 * @brief 绘制三次贝塞尔曲线
* @note 需要把模板参数里的once_points参数设置为3，因为三次贝塞尔曲线需要四个控制点
 * @param x
 * @param y
 * @param color
 */
template<typename Coord, typename Color>
auto WaveCurve::draw_BezierCurve3(const Coord *x, const Coord *y, Color color) -> void
{
    Coord px, py;
    float t = 0.0f;
    while (t <= 1.0f)
    {
        float t2 = t * t;
        float t3 = t2 * t;
        // 转换浮点数坐标到整数

        px = (Coord) ((1 - t) * (1 - t) * (1 - t) * x[0] + 3 * (1 - t) * (1 - t) * t * x[1] +
                      3 * (1 - t) * t2 * x[2] + t3 * x[3]);
        py = (Coord) ((1 - t) * (1 - t) * (1 - t) * y[0] + 3 * (1 - t) * (1 - t) * t * y[1] +
                      3 * (1 - t) * t2 * y[2] + t3 * y[3]);

        // 设置像素点
        LCD_Set_Pixel(px, py, color);
        t += smoothness;
    }
}

/**
 * @brief 绘制 Catmull-Rom 样条曲线
 * @note 需要把模板参数里的once_points参数设置为4，因为 Catmull-Rom 样条曲线需要四个控制点
 * @param x
 * @param y
 * @param color
 */
template<typename Data, typename Color>
auto WaveCurve::draw_CatmullRomSp_line(const Data *x, const Data *y, Color color) -> void
{
    float t = 0.0;
    while (t <= 1.0)
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
        t += smoothness;
    }
}

#endif //SIMULATOR_WAVECURVE_HPP