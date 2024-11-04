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
 * @note 没有显式实例化，所以，请注意不要在多个地方使用同一个模板函数，不然会导致代码膨胀。不过就这个函数的性能而言，不大可能在多个地方调用这个函数
 */

class WaveCurve
{
public:
    // 这个也是够蠢的，必须把声明放在前面，不然找不到
    enum class Type
    {
        // 插值画线
        Interpolated_Line,
        // 二次贝塞尔曲线
        BezierCurve2,
        // 三次贝塞尔曲线
        BezierCurve3,
        // Catmull-Rom样条曲线
        CatmullRomSp_line,
    };
public:
    // 绘制曲线_插值画线
    template<WaveCurve::Type draw_type, typename Data=uint16_t, typename Color=uint16_t, typename Coord=uint16_t>
    static auto
    draw_curve(Data data[], Data value, Coord N, Coord Start_x,
               Coord Start_y,
               Coord Width,
               Coord Height, Data Max_Value, Color bg_color, Color color) -> void;

protected:
    // 模板类，用于确定绘制函数
    template<WaveCurve::Type draw_type, typename Coord, typename Color>
    struct DrawFunction;

    // 绘制函数
    template<typename Coord, typename Color>
    static inline auto draw_interpolated_line(const Coord *x, const Coord *y, Color color) -> void;

    template<typename Coord, typename Color>
    static inline auto draw_BezierCurve2(const Coord *x, const Coord *y, Color color) -> void;

    template<typename Coord, typename Color>
    static inline auto draw_BezierCurve3(const Coord *x, const Coord *y, Color color) -> void;

    template<typename Coord, typename Color>
    static inline auto draw_CatmullRomSp_line(const Coord *x, const Coord *y, Color color) -> void;

    // 清除函数
    template<WaveCurve::Type draw_type, typename Coord, typename Color>
    static inline auto clean_general(const Coord *x, const Coord *y, Color color) -> void;


private:
    // 获取一次绘制所需的点数
    template<WaveCurve::Type draw_type>
    static inline constexpr auto getOncePoints() -> uint16_t;

    // 获取最大最小值
    template<typename Coord>
    static inline auto get_max_min(Coord value, Coord &max, Coord &min) -> void;

    // 交换指针
    template<typename T>
    static inline auto switch_ptr(T *&p1, T *&p2) -> void;


    // 定义步长，决定曲线的平滑度
    static constexpr float smoothness = 0.1f;// 定为0.1比较适合，不推荐改，如果改的话，把下面的缓冲数组尺寸也改了1/smoothness+1，得是整数
    static constexpr uint16_t buffer_size = 11;

    // 定义大量静态数组时千万不要把volatile去掉，程序对数组的优化，尤其是静态数组，总是容易优化死
    // 根据我的远古回忆，上次优化导致卡死还是在使用TI(默认-O2还是-O3来着)时，定义了一个很小的静态数组，结果排查了半天
    // 最后根据汇编查看才发现是静态数组被优化掉了，变成寄存器了还是什么的，总之之后就卡死了
//    volatile inline static Coord x_buff1[buffer_size]{};
//    volatile inline static Coord y_buff1[buffer_size]{};
//    volatile inline static Coord x_buff2[buffer_size]{};
//    volatile inline static Coord y_buff2[buffer_size]{};
//    volatile inline static Coord *pBuff_draw_x = x_buff1;
//    volatile inline static Coord *pBuff_draw_y = y_buff1;
//    volatile inline static Coord *pBuff_clean_x = x_buff2;
//    volatile inline static Coord *pBuff_clean_y = y_buff2;
};

template<typename T>
auto WaveCurve::switch_ptr(T *&p1, T *&p2) -> void
{
    T *tmp = p1;
    p1 = p2;
    p2 = tmp;
}


template<typename Coord>
auto WaveCurve::get_max_min(Coord value, Coord &max, Coord &min) -> void
{
    if (value < min)
        min = value;
    else if (value > max)
        max = value;
}

// 显式实例化
template auto WaveCurve::get_max_min(uint16_t value, uint16_t &max, uint16_t &min) -> void;


// 获取一次绘制所需的点数
template<WaveCurve::Type draw_type>
constexpr auto WaveCurve::getOncePoints() -> uint16_t
{
    if constexpr (draw_type == Type::Interpolated_Line)
    {
        return 2; // 两个点
    } else if constexpr (draw_type == Type::BezierCurve2)
    {
        return 3;// 三个点
    } else if constexpr (draw_type == Type::BezierCurve3)
    {
        return 4; // 四个点
    } else if constexpr (draw_type == Type::CatmullRomSp_line)
    {
        return 4; // 四个点
    } else
    {
        return 3;// 三个点
    }
}

/**
 * @brief 通用清除函数
 * @tparam Coord
 * @tparam Color
 * @param x
 * @param y
 * @param color
 */
template<WaveCurve::Type draw_type, typename Coord, typename Color>
auto WaveCurve::clean_general(const Coord *x, const Coord *y, Color color) -> void
{
    constexpr auto once_points = getOncePoints<draw_type>();
    Coord min_y = y[0], max_y = y[0];
    for (int i = 1; i < once_points; ++i)
    {
        get_max_min<Coord>(y[i], max_y, min_y);
    }
    LCD_Color_Clean(x[0], min_y, x[1], max_y, color);
}

/*DrawFunction特化版本*/
// 插值画线
template<typename Coord, typename Color>
struct WaveCurve::DrawFunction<WaveCurve::Type::Interpolated_Line, Coord, Color>
{
    static auto inline draw(const Coord *x, const Coord *y, Color color) -> void
    {
        draw_interpolated_line(x, y, color);
    }

    static auto inline clean(const Coord *x, const Coord *y, Color color) -> void
    {
        draw_interpolated_line(x, y, color);// 这样反而更快
    }
};

// 二次贝塞尔曲线
template<typename Coord, typename Color>
struct WaveCurve::DrawFunction<WaveCurve::Type::BezierCurve2, Coord, Color>
{
    static inline Coord buff_x[buffer_size];
    static inline Coord clean_buff_y[buffer_size];
    static inline Coord *pBuff_x = buff_x;
    static inline Coord *pCleanBuff_y = clean_buff_y;

    static inline Coord draw_buff_y[buffer_size];
    static inline Coord *pDrawBuff_y = draw_buff_y;

    static auto inline draw(int &i,Coord& N,const Coord x[], const Coord y[], Color &bg_color, Color &color) -> void
    {
        uint16_t end_index=N- getOncePoints<WaveCurve::Type::BezierCurve2>();
        for (int j = 0; j <= 10; ++j)
        {
            float t = j * smoothness;
            float one_minus_t = 1.0f - t;
            float two_t = 2.0f * t;
            float t2 = t * t;

            // 计算px和py
            pCleanBuff_y[j] = (Coord) (one_minus_t * one_minus_t * y[0] + two_t * one_minus_t * y[1] + t2 * y[2]);
            if(i!=end_index)
            {
                pBuff_x[j] = (Coord) (one_minus_t * one_minus_t * x[0] + two_t * one_minus_t * x[1] + t2 * x[2]);

                LCD_Set_Pixel(pBuff_x[j], pCleanBuff_y[j], bg_color);// 清除旧像素点
                LCD_Set_Pixel(pBuff_x[j], pDrawBuff_y[j], color);// 绘制新像素点
            }
        }
        switch_ptr(pCleanBuff_y, pDrawBuff_y);// 交换指针，把pBuff_y当做下一轮pBuff_new_y
    }
};

// 三次贝塞尔曲线
template<typename Coord, typename Color>
struct WaveCurve::DrawFunction<WaveCurve::Type::BezierCurve3, Coord, Color>
{
    static auto inline draw(const Coord *x, const Coord *y, Color color) -> void
    {
        draw_BezierCurve3(x, y, color);
    }

    static auto inline clean(const Coord *x, const Coord *y, Color color) -> void
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
};

// Catmull-Rom样条曲线
template<typename Coord, typename Color>
struct WaveCurve::DrawFunction<WaveCurve::Type::CatmullRomSp_line, Coord, Color>
{
    static auto inline draw(const Coord *x, const Coord *y, Color color) -> void
    {
        draw_CatmullRomSp_line(x, y, color);
    }

    static auto inline clean(const Coord *x, const Coord *y, Color color) -> void
    {
        draw_CatmullRomSp_line(x, y, color);
    }
};

/**
 * @brief 绘制曲线
 * @tparam draw_type 绘制曲线类型
 * @tparam Data 数据类型，默认为uint8_t
 * @tparam Color 颜色类型，默认为uint16_t
 * @tparam Coord 坐标类型，默认为uint16_t
 * @param data  缓冲数组，其大小为总显示点数+1，因为要预留1个元素用于提速（去除分支判断）
 * @param value 待绘制的值
 * @param N  总显示点数，也是缓冲数组大小-1
 * @param Start_x 绘制区域的起点横坐标
 * @param Start_y 绘制区域的起点纵坐标
 * @param Width 绘制区域的宽度
 * @param Height 绘制区域的高度
 * @param Max_Value 样本数据的最大值
 */
template<WaveCurve::Type draw_type, typename Data, typename Color, typename Coord>
auto WaveCurve::draw_curve(Data data[], Data value, Coord N, Coord Start_x, Coord Start_y, Coord Width, Coord Height,
                           Data Max_Value, Color bg_color, Color color) -> void
{
    /*******************前置条件*********************/
    // 定义一个常量，使用lambda表达式这种解决方法我是真没想到，不过这里用静态内联函数+模板+constexpr的组合
    constexpr uint16_t once_points = getOncePoints<draw_type>();

    // 如果数据点过少，则无法画线
    if (N < once_points) return;

    Coord x[once_points], y[once_points + 1];
    float ratio = (Height - 1.0f) / Max_Value;
    float step = Width / (N - 1.0f);// 步长

    // 一放在前面就会卡死，不知缘由
//        data[N - 1] = value;

    /*******************绘制曲线*********************/
    // 从右往左刷新，更符合直观上的感受
    for (int i = N - once_points; i >= 0; --i)
    {
        // 确认旧点
        for (int j = 0; j < once_points; ++j)
        {
            x[j] = (Coord) (Start_x + (float) (i + j) * step);
            y[j] = (Coord) (Start_y + Height - (data[i + j] * ratio));
        }

        // 更新曲线
        if constexpr (draw_type == Type::Interpolated_Line)
        {
            // 确认新点
            y[once_points] = (Coord) (Start_y + Height - data[i + once_points] * ratio);
            DrawFunction<draw_type, Coord, Color>::clean(x, y, bg_color);// 清除旧曲线
            DrawFunction<draw_type, Coord, Color>::draw(x, y + 1, color);// 绘制新曲线
        } else
        {
            DrawFunction<draw_type, Coord, Color>::draw(i,N,x, y, bg_color, color);// 绘制新曲线
        }
    }

/******更新数据******/
    memmove(data, data + 1, (N - 1) * sizeof(data[0]));// 将数据左移一位
    data[N - 1] = value;
}

/*************************画线函数算法***************************/
// 插值画线函数
template<typename Coord, typename Color>
auto WaveCurve::draw_interpolated_line(const Coord *x, const Coord *y, Color color) -> void
{
    // Bresenham's line algorithm
    Coord x0 = x[0], y0 = y[0], x1 = x[1], y1 = y[1];
    int dx = (abs(x1 - x0)), sx = x0 < x1 ? 1 : -1;// sx：决定递增方向
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