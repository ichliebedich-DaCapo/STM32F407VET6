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

uint16_t Buf[Sample_Size];
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
        memmove(&Buf[0], &Buf[1], (N - 1) * sizeof(Buf[0]));
        Buf[N - 1] = newdata;
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

void update_wave2(uint16_t value)
{
    // 更新数据
    draw_interpolated_line(Buf, Sample_Size, start_x, start_y, chart_width, chart_height, max_value, COLOR_WHITE);
    roll_and_update(value, Sample_Size);
    draw_interpolated_line(Buf, Sample_Size, start_x, start_y, chart_width, chart_height, max_value, COLOR_BLACK);
}

static inline void Draw_interpolated_line(uint16_t *x, uint16_t *y, uint32_t color)
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

void update_wave(uint16_t value, uint16_t N, uint16_t Start_x, uint16_t Start_y, uint16_t Width, uint16_t Height,
                 uint16_t Max_Value)
{
    if (N < 2) return; // 如果数据点少于2个，则无法画线

    // 计算每个数据点之间的水平距离
    uint16_t step = Width / (N - 1);// 步长
    uint16_t x[2], y[3];

    // 从右往左刷新，更符合直观上的感受
    for (int i = N - 2; i >= 0; --i)
    {
        // 确认旧点
        for (int j = 0; j < 2; ++j)
        {
            x[j] = (uint16_t) (Start_x + (i + j) * step);
            y[j] = Start_y + Height - (Buf[i + j] * (Height - 1) / Max_Value);
        }

        // 确认新点
        if (i == N - 2)
        {
            y[2] = Start_y + Height - (value * (Height - 1) / Max_Value);// 反转y轴以匹配屏幕坐标系
        } else [[likely]] // 注意这里的写法
        {
            y[2] = Start_y + Height - (Buf[i + 2] * (Height - 1) / Max_Value);
        }

        // 更新曲线
        Draw_interpolated_line(x, y, COLOR_WHITE);// 清除前面曲线
        Draw_interpolated_line(x, y + 1, COLOR_BLACK);// 绘制新曲线
    }

    /******更新数据******/
    memmove(&Buf[0], &Buf[1], (N - 1) * sizeof(Buf[0]));
    Buf[N - 1] = value;
}

/****************二次贝塞尔曲线******************/
// 计算贝塞尔曲线上的一点
static inline void
BezierPoint(float t, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, float *x, float *y)
{
    *x = (1 - t) * (1 - t) * x0 + 2 * (1 - t) * t * x1 + t * t * x2;
    *y = (1 - t) * (1 - t) * y0 + 2 * (1 - t) * t * y1 + t * t * y2;
}

// 绘制贝塞尔曲线
void DrawBezierCurve(const uint16_t *x, const uint16_t *y, uint32_t color, uint8_t Bezier_Times = 2)
{
    // 定义步长，决定曲线的平滑度
    constexpr float step = 0.01;
    for (float t = 0.0; t <= 1.0; t += step)
    {
        // 转换浮点数坐标到整数
        uint16_t px, py;
        if (Bezier_Times == 2)
        {
            px = (uint16_t) ((1 - t) * (1 - t) * x[0] + 2 * (1 - t) * t * x[1] + t * t * x[2]);
            py = (uint16_t) ((1 - t) * (1 - t) * y[0] + 2 * (1 - t) * t * y[1] + t * t * y[2]);
        } else
        {
            px = (uint16_t) ((1 - t) * (1 - t) * (1 - t) * x[0] + 3 * (1 - t) * (1 - t) * t * x[1] +
                             3 * (1 - t) * t * t * x[2] + t * t * t * x[3]);
            py = (uint16_t) ((1 - t) * (1 - t) * (1 - t) * y[0] + 3 * (1 - t) * (1 - t) * t * y[1] +
                             3 * (1 - t) * t * t * y[2] + t * t * t * y[3]);
        }
        // 设置像素点
        LCD_Set_Pixel(px, py, color);
    }
}

/**
 * @brief 绘制波形曲线
 */
class WaveCurve
{
public:
    // 绘制曲线_插值画线
    static inline void
    update_interpolated_line(uint16_t value, uint16_t N, uint16_t Start_x, uint16_t Start_y, uint16_t Width,
                             uint16_t Height, uint16_t Max_Value);

private:
    static inline auto draw_interpolated_line(const uint16_t *x, const uint16_t *y, uint32_t color) -> void;

private:
    static constexpr uint16_t ax_Value = 4096;
};

// 插值画线函数
auto WaveCurve::draw_interpolated_line(const uint16_t *x, const uint16_t *y, uint32_t color) -> void
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
 * @tparam Color 颜色类型
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
template <typename Data,typename Color,typename Coord=uint16_t ,uint8_t once_points=2>
void draw_curve(void(*DrawFunc)(Coord*,Coord*,Color),  Data data[],Data value, Coord N, Coord Start_x, Coord Start_y, Coord Width,
                Coord Height, Data Max_Value)
{
    if (N < once_points) return; // 如果数据点过少，则无法画线

    // 计算每个数据点之间的水平距离
    uint16_t step = Width / (N - 1);// 步长
    uint16_t x[once_points], y[once_points+1];

    // 从右往左刷新，更符合直观上的感受
    for (int i = N - once_points; i >= 0; --i)
    {
        // 确认旧点
        for (int j = 0; j < once_points; ++j)
        {
            x[j] = (uint16_t) (Start_x + (i + j) * step);
            y[j] = Start_y + Height - (Buf[i + j] * (Height - 1) / Max_Value);
        }

        // 确认新点
        if (i == N - once_points)
        {
            y[once_points] = Start_y + Height - (value * (Height - 1) / Max_Value);// 反转y轴以匹配屏幕坐标系
        } else[[likely]]
        {
            y[once_points] = Start_y + Height - (Buf[i + once_points] * (Height - 1) / Max_Value);
        }

        // 更新曲线
        DrawFunc(x, y, COLOR_WHITE);// 清除前面曲线
        DrawFunc(x, y + 1, COLOR_BLACK);// 绘制新曲线
    }

    /******更新数据******/
    memmove(&Buf[0], &Buf[1], (N - 1) * sizeof(Buf[0]));
    Buf[N - 1] = value;
}

/**
 * @brief 绘制曲线_插值画线
 * @param value 待绘制的值
 * @param N  总显示点数
 * @param Start_x 绘制区域的起点横坐标
 * @param Start_y 绘制区域的起点纵坐标
 * @param Width 绘制区域的宽度
 * @param Height 绘制区域的高度
 * @param Max_Value 样本数据的最大值
 */
void WaveCurve::update_interpolated_line(uint16_t value, uint16_t N, uint16_t Start_x, uint16_t Start_y, uint16_t Width,
                                         uint16_t Height, uint16_t Max_Value)
{
    draw_curve<uint16_t ,uint32_t>(Draw_interpolated_line,Buf,value,N, Start_x, Start_y, Width, Height, Max_Value);
}

//void update_wave(uint16_t value, uint16_t N, uint16_t Start_x, uint16_t Start_y, uint16_t Width, uint16_t Height,
//                 uint16_t Max_Value)
//{
//    constexpr uint8_t Bezier_Times = 2;// 贝塞尔次数
//
//    if (N < 2) return; // 如果数据点少于2个，则无法画线
//
//    // 计算每个数据点之间的水平距离
//    uint16_t step =Width/ (N - 1);// 步长
//    uint16_t x[Bezier_Times + 1], y[Bezier_Times + 2];
//
//    for (int i = 0; i < N - Bezier_Times; ++i)
//    {
//        // 确认旧点
//        for (int j = 0; j < Bezier_Times + 1; ++j)
//        {
//            x[j] = (uint16_t )(Start_x + (i + j) * step);
//            y[j] = Start_y + Height - (Data[i + j] * (Height - 1) / Max_Value);
//        }
//
//        // 确认新点
//        if (i == N - Bezier_Times - 1)
//        {
//            y[Bezier_Times + 1] = Start_y + Height - (value * (Height - 1) / Max_Value);// 反转y轴以匹配屏幕坐标系
//        } else
//        {
//            y[Bezier_Times + 1] = Start_y + Height - (Data[i + Bezier_Times + 1] * (Height - 1) / Max_Value);
//        }
//
//        // 更新曲线
//        DrawBezierCurve(x, y, COLOR_WHITE, Bezier_Times);// 清除前面曲线
//        DrawBezierCurve(x, y + 1, COLOR_BLACK, Bezier_Times);// 绘制新曲线
//    }
//
//    /******更新数据******/
//    memmove(&Data[0], &Data[1], (N - 1) * sizeof(Data[0]));
//    Data[N - 1] = value;
//}

// 计算 Catmull-Rom 样条上的一点
static inline void
CatmullRomPoint(float t, uint16_t p0[2], uint16_t p1[2], uint16_t p2[2], uint16_t p3[2], float *x, float *y)
{
    float t2 = t * t;
    float t3 = t2 * t;

    *x = 0.5 * (2 * p1[0] +
                (-p0[0] + p2[0]) * t +
                (2 * p0[0] - 5 * p1[0] + 4 * p2[0] - p3[0]) * t2 +
                (-p0[0] + 3 * p1[0] - 3 * p2[0] + p3[0]) * t3);

    *y = 0.5 * (2 * p1[1] +
                (-p0[1] + p2[1]) * t +
                (2 * p0[1] - 5 * p1[1] + 4 * p2[1] - p3[1]) * t2 +
                (-p0[1] + 3 * p1[1] - 3 * p2[1] + p3[1]) * t3);
}

// 绘制 Catmull-Rom 样条曲线
static inline void
DrawCatmullRomSpline(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3,
                     uint16_t y3, uint32_t color)
{
    constexpr float step = 0.01;

    uint16_t p0[2] = {x0, y0};
    uint16_t p1[2] = {x1, y1};
    uint16_t p2[2] = {x2, y2};
    uint16_t p3[2] = {x3, y3};

    for (float t = 0.0; t <= 1.0; t += step)
    {
        float x, y;
        CatmullRomPoint(t, p0, p1, p2, p3, &x, &y);
        // 转换浮点数坐标到整数
        uint16_t px = (uint16_t) x;
        uint16_t py = (uint16_t) y;
        // 设置像素点
        LCD_Set_Pixel(px, py, color);
    }
}

//void update_wave(uint16_t value, uint16_t N, uint16_t Start_x, uint16_t Start_y, uint16_t Width, uint16_t Height,
//                 uint16_t Max_Value)
//{
//    /******清除前面数据******/
//    if (N < 4) return;   // 不足 4 个点，无法绘制 Catmull-Rom 样条
//
//    // 计算每个数据点之间的水平距离
//    uint16_t step = Width / (N - 1);
//    uint16_t x0, y0, x1, y1, x2, y2, x3, y3;
//
//    for (int i = 0; i < N - 3; ++i)
//    {
//        // 旧：当前点和下一个点的坐标
//        x0 = Start_x + i * step;
//        y0 = Start_y + Height - (Data[i] * (Height - 1) / Max_Value); // 反转y轴以匹配屏幕坐标系
//        x1 = Start_x + (i + 1) * step;
//        y1 = Start_y + Height - (Data[i + 1] * (Height - 1) / Max_Value);
//        x2 = Start_x + (i + 2) * step;
//        y2 = Start_y + Height - (Data[i + 2] * (Height - 1) / Max_Value);
//        x3 = Start_x + (i + 3) * step;
//        y3 = Start_y + Height - (Data[i + 3] * (Height - 1) / Max_Value);
//
//        /**********画线算法***********/
//        DrawCatmullRomSpline(x0, y0, x1, y1, x2, y2, x3, y3, COLOR_WHITE);
//
//        // 新：当前点和下一个点的坐标
//        if (i == N - 4)
//        {
//            y0 = Start_y + Height - (value * (Height - 1) / Max_Value);
//        } else
//        {
//            y0 = Start_y + Height - (Data[i + 3] * (Height - 1) / Max_Value);
//        }
//
//        /**********画线算法***********/
//        DrawCatmullRomSpline(x0, y1, x1, y2, x2, y3, x3, y0, COLOR_BLACK);
//    }
//    /******更新数据******/
//
//    memmove(&Data[0], &Data[1], (N - 1) * sizeof(Data[0]));
//    Data[N - 1] = value;
//}

// 用于在main中执行
void test_handler()
{
    static uint8_t cnt = 0;
//    update_wave(sine_wave[(cnt++) % 128]);
//    update_wave(sine_wave[(cnt++) % 128], Sample_Size, start_x, start_y, chart_width, chart_height, max_value);
WaveCurve::update_interpolated_line(sine_wave[(cnt++) % 128], Sample_Size, start_x, start_y, chart_width, chart_height, max_value);
}

