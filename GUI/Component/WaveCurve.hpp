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
    draw_curve(Data data[], Coord N, Data value, Coord Start_x,
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
    static constexpr float smoothness = 0.1f;// 定为0.1比较适合，不推荐改，再小会非常卡，太大曲线会不连续
    static constexpr uint16_t buffer_size = (uint16_t) (1.0f / smoothness) + 1;

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
    volatile static inline Coord buff_x[buffer_size];
    volatile static inline Coord clean_buff_y[buffer_size];
    volatile static inline Coord *pBuff_x = buff_x;
    volatile static inline Coord *pCleanBuff_y = clean_buff_y;

    volatile static inline Coord draw_buff_y[buffer_size];
    volatile static inline Coord *pDrawBuff_y = draw_buff_y;
};

template<typename T>
auto WaveCurve::switch_ptr(T *&p1, T *&p2) -> void
{
    T *tmp = p1;
    p1 = p2;
    p2 = tmp;
}


// 获取一次绘制所需的点数
template<WaveCurve::Type draw_type>
constexpr auto WaveCurve::getOncePoints() -> uint16_t
{
    if constexpr (draw_type == Type::BezierCurve2)
    {
        return 3;// 三个点
    } else if constexpr (draw_type == Type::BezierCurve3 || draw_type == Type::CatmullRomSp_line)
    {
        return 4; // 四个点
    } else // 默认为 Type::Interpolated_Line线性插值算法
    {
        return 2; // 两个点
    }
}


/*DrawFunction特化版本*/
// 插值画线
template<typename Coord, typename Color>
struct WaveCurve::DrawFunction<WaveCurve::Type::Interpolated_Line, Coord, Color>
{
    static auto inline
    draw(Coord &y_old, Coord &Start_x, Coord &Start_y, const Coord x[], const Coord y[], Color &bg_color,
         Color &color) -> void
    {
        // Bresenham's line algorithm
        Coord x0 = x[0], y0 = y[0], x1 = x[1], y1 = y[1];
        const int dx = (abs(x1 - x0));
        const int sx = x0 < x1 ? 1 : -1;// sx：决定递增方向
        int dy = (-abs(y1 - y0));
        int sy = y0 < y1 ? 1 : -1;
        int err = (dx + dy), e2;

        for (;;)
        {
            LCD_Set_Pixel(x0, y0, bg_color);
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
        // 重置
        x0 = x[0];
        y0 = y1;
        x1 = x[1];
        y1 = y_old;
        dy = (-abs(y1 - y0));
        sy = y0 < y1 ? 1 : -1;
        err = (dx + dy);

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
//            y_old = y0;//使用这个会出现非常诡异的图形，观赏性是有滴
        y_old = y[1];
    }
};

// 二次贝塞尔曲线
template<typename Coord, typename Color>
struct WaveCurve::DrawFunction<WaveCurve::Type::BezierCurve2, Coord, Color>
{
    static auto inline draw(int &i, Coord &N, const Coord x[], const Coord y[], Color &bg_color, Color &color) -> void
    {
        uint16_t end_index = N - getOncePoints<WaveCurve::Type::BezierCurve2>();
        float t = 0.0f;
        for (int j = 0; j <= 10; ++j)
        {
            float one_minus_t = 1.0f - t;
            float two_t = 2.0f * t;
            float t2 = t * t;

            // 计算px和py
            pCleanBuff_y[j] = (Coord) (one_minus_t * one_minus_t * y[0] + two_t * one_minus_t * y[1] + t2 * y[2]);
            if (i != end_index)
            {
                pBuff_x[j] = (Coord) (one_minus_t * one_minus_t * x[0] + two_t * one_minus_t * x[1] + t2 * x[2]);

                LCD_Set_Pixel(pBuff_x[j], pCleanBuff_y[j], bg_color);// 清除旧像素点
                LCD_Set_Pixel(pBuff_x[j], pDrawBuff_y[j], color);// 绘制新像素点
            }
            t += smoothness;
        }
        switch_ptr(pCleanBuff_y, pDrawBuff_y);// 交换指针，把pBuff_y当做下一轮pBuff_new_y
    }
};

// 三次贝塞尔曲线
template<typename Coord, typename Color>
struct WaveCurve::DrawFunction<WaveCurve::Type::BezierCurve3, Coord, Color>
{
    static auto inline draw(int &i, Coord &N, const Coord x[], const Coord y[], Color &bg_color, Color &color) -> void
    {
        uint16_t end_index = N - getOncePoints<WaveCurve::Type::BezierCurve3>();
        float t = 0.0f;
        for (int j = 0; j <= 10; ++j)
        {
            float one_minus_t = 1.0f - t;
            float one_minus_t_2 = one_minus_t * one_minus_t;
            float one_minus_t_3 = one_minus_t_2 * one_minus_t;
            float three_t = 3.0f * t;
            float t2 = t * t;
            float t3 = t2 * t;

            // 计算px和py

            pCleanBuff_y[j] =
                    (Coord) (one_minus_t_3 * y[0] + three_t * one_minus_t_2 * y[1] + 3 * one_minus_t * t2 * y[2]
                             + t3 * y[3]);
            if (i != end_index)
            {
                pBuff_x[j] = (Coord) (one_minus_t_3 * x[0] + three_t * one_minus_t_2 * x[1] +
                                      3 * one_minus_t * t2 * x[2]
                                      + t3 * x[3]);

                LCD_Set_Pixel(pBuff_x[j], pCleanBuff_y[j], bg_color);// 清除旧像素点
                LCD_Set_Pixel(pBuff_x[j], pDrawBuff_y[j], color);// 绘制新像素点
            }
            t += smoothness;
        }
        switch_ptr(pCleanBuff_y, pDrawBuff_y);// 交换指针，把pBuff_y当做下一轮pBuff_new_y

    }
};

// Catmull-Rom样条曲线
template<typename Coord, typename Color>
struct WaveCurve::DrawFunction<WaveCurve::Type::CatmullRomSp_line, Coord, Color>
{
    static auto inline draw(int &i, Coord &N, const Coord x[], const Coord y[], Color &bg_color, Color &color) -> void
    {
        uint16_t end_index = N - getOncePoints<WaveCurve::Type::CatmullRomSp_line>();
        float t = 0.0f;
        for (int j = 0; j <= 10; ++j)
        {
            float t2 = t * t;
            float t3 = t2 * t;

            // 计算px和py
            pCleanBuff_y[j] = (Coord) (0.5 * (2 * y[1] +
                                              (-y[0] + y[2]) * t +
                                              (2 * y[0] - 5 * y[1] + 4 * y[2] - y[3]) * t2 +
                                              (-y[0] + 3 * y[1] - 3 * y[2] + y[3]) * t3));

            if (i != end_index)
            {
                pBuff_x[j] = (Coord) (0.5 *
                                      (2 * x[1] + (-x[0] + x[1]) * t + (2 * x[0] - 5 * x[1] + 4 * x[2] - x[3]) * t2 +
                                       (-x[0] + 3 * x[1] - 3 * x[2] + x[3]) * t3));

                LCD_Set_Pixel(pBuff_x[j], pCleanBuff_y[j], bg_color);// 清除旧像素点
                LCD_Set_Pixel(pBuff_x[j], pDrawBuff_y[j], color);// 绘制新像素点
            }
            t += smoothness;
        }
        switch_ptr(pCleanBuff_y, pDrawBuff_y);// 交换指针，把pBuff_y当做下一轮pBuff_new_y
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
 * @param N  总显示点数，也是缓冲数组大小-1,为了可以动态调整绘制点数，不进行模板特化
 * @param Start_x 绘制区域的起点横坐标
 * @param Start_y 绘制区域的起点纵坐标
 * @param Width 绘制区域的宽度
 * @param Height 绘制区域的高度
 * @param Max_Value 样本数据的最大值
 */
template<WaveCurve::Type draw_type, typename Data, typename Color, typename Coord>
auto WaveCurve::draw_curve(Data data[], Coord N, Data value, Coord Start_x, Coord Start_y, Coord Width, Coord Height,
                           Data Max_Value, Color bg_color, Color color) -> void
{
    /*******************前置条件*********************/
    // 定义一个常量，使用lambda表达式这种解决方法我是真没想到，不过这里用静态内联函数+模板+constexpr的组合
    constexpr uint16_t once_points = getOncePoints<draw_type>();

    // 如果数据点过少，则无法画线
    if (N < once_points) return;

    Coord x[once_points], y[once_points];
    float ratio = (Height - 1.0f) / Max_Value;
    float step = Width / (N - 1.0f);// 步长

    Coord y_old;// 未使用会被优化掉
    if constexpr (draw_type == Type::Interpolated_Line)
        y_old = (Coord) (Start_y + Height - value * ratio);

//        data[N - 1] = value;// 一放在前面就会卡死，不知缘由

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
            DrawFunction<draw_type, Coord, Color>::draw(y_old, Start_x, Start_y, x, y, bg_color, color);
        } else
        {
            DrawFunction<draw_type, Coord, Color>::draw(i, N, x, y, bg_color, color);// 绘制新曲线
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

#endif //SIMULATOR_WAVECURVE_HPP