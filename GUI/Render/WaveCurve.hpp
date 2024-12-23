//
// Created by fairy on 2024/11/3 05:52.
//
#ifndef SIMULATOR_WAVECURVE_HPP
#define SIMULATOR_WAVECURVE_HPP


/* 预编译命令 */
// 用于区分模拟器和真机
#ifdef ARM_MATH_CM4

#include "lcd.h"

#else

#include "lv_drivers/display/monitor.h"

#endif

#include <cstring> // 引入string.h以使用memmove
#include <valarray>

// 枚举体
enum class WaveCurveType
{
    // 插值画线
    Interpolated_Line,
    // 二次贝塞尔曲线
    BezierCurve2,
    // 三次贝塞尔曲线
    BezierCurve3,
    // Catmull-Rom样条曲线
    // 开-Og优化可以使用，-O2以上会直接卡死，我也不明白
    CatmullRomSp_line,
};

//结构体
typedef struct {
    uint16_t x;
    uint16_t y;
} Point;

/**
 * @brief 绘制波形曲线
 * @note 没有显式实例化，所以，请注意不要在多个地方使用同一个模板函数，不然会导致代码膨胀。不过就这个函数的性能而言，不大可能在多个地方调用这个函数
 * \绘制情况
 *          以168MHz的stm32f407裸机运行来说，如果绘制区域为320*200那么大，且使用FSMC+DMA，8080并口屏，那么：
 *          如果绘制点数大于100，建议用线性插值；如果小于100，建议用其他两种曲线。一帧时间推荐在80ms左右
 *          如果波形数据变化剧烈，建议不要使用线性插值。样条曲线更适合各种波形数据，非常均匀，不过要注意的是下面的样条曲线算法在-Og优化下没有问题，不知道什么原因。
 */


template<typename Coord=uint16_t>
class WaveCurve
{
public:
    // 绘制曲线_插值画线
    template<WaveCurveType draw_type, typename Data=uint16_t, typename Color=uint16_t>
    [[maybe_unused]] static auto
    draw_curve(Data data[], Coord N, Data value, Coord Start_x,
               Coord Start_y,
               Coord Width,
               Coord Height, Data Max_Value, Color bg_color, Color color) -> void;

protected:
    // 模板特化，用于确定绘制函数
    template<WaveCurveType draw_type, typename Color>
    struct DrawFunction;

private:
    // 获取一次绘制所需的点数
    template<WaveCurveType draw_type>
    static inline constexpr auto getOncePoints() -> uint16_t;

    // 交换指针
    template<typename T>
    static inline auto switch_ptr(T *&p1, T *&p2) -> void;

    static inline auto get_index(const Coord &N, const auto &index) -> Coord
    {

        if (index + index_offset >= N)
        {
            return index + index_offset - N;
        } else
        {
            return index + index_offset;
        }

    }

    static inline auto add_index(Coord N) -> void
    {
        ++index_offset;
        if (index_offset == N)
            index_offset = 0;
    }




private:
    // 定义步长，决定曲线的平滑度
    static constexpr float smoothness = 0.1f;// 定为0.1比较适合，不推荐改，再小会非常卡，太大曲线会不连续
    static constexpr uint16_t buffer_size = (uint16_t) (1.0f / smoothness) + 1;

    static inline Coord index_offset = 0;//用于优化索引

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
    static inline Coord buff_x[buffer_size];
    static inline Coord clean_buff_y[buffer_size];
    static inline Coord *pBuff_x = buff_x;
    static inline Coord *pCleanBuff_y = clean_buff_y;

    static inline Coord draw_buff_y[buffer_size];
    static inline Coord *pDrawBuff_y = draw_buff_y;

};

template<typename Coord>
template<typename T>
auto WaveCurve<Coord>::switch_ptr(T *&p1, T *&p2) -> void
{
    T *tmp = p1;
    p1 = p2;
    p2 = tmp;
}


// 获取一次绘制所需的点数
template<typename Coord>
template<WaveCurveType draw_type>
constexpr auto WaveCurve<Coord>::getOncePoints() -> uint16_t
{
    if constexpr (draw_type == WaveCurveType::BezierCurve2)
    {
        return 3;// 三个点
    } else if constexpr (draw_type == WaveCurveType::BezierCurve3 || draw_type == WaveCurveType::CatmullRomSp_line)
    {
        return 4; // 四个点
    } else // 默认为 Type::Interpolated_Line线性插值算法
    {
        return 2; // 两个点
    }
}


/*DrawFunction特化版本*/
/**
 * @brief 插值画线
 * @tparam Data
 * @tparam Coord
 * @tparam Color
 * @note 插值画线算法，使用Bresenham算法，插值点数由模板参数决定，默认为2，即线性插值。
 *        为了与其他函数一致，所以使用了 draw_buff_y[0]用于存储临时的y坐标
 */
template<typename Coord>
template<typename Color>
struct WaveCurve<Coord>::DrawFunction<WaveCurveType::Interpolated_Line, Color>
{

    static auto inline
    draw(const Coord x[], const Coord y[], Color &bg_color,
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
        y1 = draw_buff_y[0];
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
//            draw_buff_y[0] = y0;//使用这个会出现非常诡异的图形，观赏性是有滴
        draw_buff_y[0] = y[1];
    }
};





// 二次贝塞尔曲线
template<typename Coord>
template<typename Color>
struct WaveCurve<Coord>::DrawFunction<WaveCurveType::BezierCurve2, Color>
{
    static auto inline draw(const Coord x[], const Coord y[], Color &bg_color, Color &color) -> void
    {
        float t = 0.0f;
        for (int j = 0; j <= 10; ++j)
        {
            float one_minus_t = 1.0f - t;
            float two_t = t + t;
            float t2 = t * t;

            // 计算px和py
            pCleanBuff_y[j] = (Coord) (one_minus_t * one_minus_t * y[0] + two_t * one_minus_t * y[1] + t2 * y[2]);
            pBuff_x[j] = (Coord) (one_minus_t * one_minus_t * x[0] + two_t * one_minus_t * x[1] + t2 * x[2]);

            LCD_Set_Pixel(pBuff_x[j], pCleanBuff_y[j], bg_color);// 清除旧像素点
            LCD_Set_Pixel(pBuff_x[j], pDrawBuff_y[j], color);// 绘制新像素点
            t += smoothness;
        }
        switch_ptr(pCleanBuff_y, pDrawBuff_y);// 交换指针，把pBuff_y当做下一轮pBuff_new_y
    }
};

// 三次贝塞尔曲线
template<typename Coord>
template<typename Color>
struct WaveCurve<Coord>::DrawFunction<WaveCurveType::BezierCurve3, Color>
{
    static auto inline draw(const Coord x[], const Coord y[], Color &bg_color, Color &color) -> void
    {
        float t = 0.0f;
        for (int j = 0; j <= 10; ++j)
        {
            float one_minus_t = 1.0f - t;
            float one_minus_t_2 = one_minus_t * one_minus_t;
            float one_minus_t_3 = one_minus_t_2 * one_minus_t;
            float three_t = t + t + t;// 我不敢假定编译器的优化行为
            float t2 = t * t;
            float t3 = t2 * t;

            // 计算px和py
            pCleanBuff_y[j] = (Coord) (one_minus_t_3 * y[0] + three_t * one_minus_t_2 * y[1] +
                                       3 * one_minus_t * t2 * y[2] + t3 * y[3]);
            pBuff_x[j] = (Coord) (one_minus_t_3 * x[0] + three_t * one_minus_t_2 * x[1] + 3 * one_minus_t * t2 * x[2] +
                                  t3 * x[3]);

            LCD_Set_Pixel(pBuff_x[j], pCleanBuff_y[j], bg_color);// 清除旧像素点
            LCD_Set_Pixel(pBuff_x[j], pDrawBuff_y[j], color);// 绘制新像素点
            t += smoothness;
        }
        switch_ptr(pCleanBuff_y, pDrawBuff_y);// 交换指针，把pBuff_y当做下一轮pBuff_new_y
    }
};

// Catmull-Rom样条曲线
template<typename Coord>
template<typename Color>
struct WaveCurve<Coord>::DrawFunction<WaveCurveType::CatmullRomSp_line, Color>
{
    static auto inline draw(const Coord x[], const Coord y[], Color &bg_color, Color &color) -> void
    {
        float t = 0.0f;
        // 预计算一些常量表达式
        const float y_c0 = 2 * y[1];
        const float y_c1 = (-y[0] + y[2]);
        const float y_c2 = (2 * y[0] - 5 * y[1] + 4 * y[2] - y[3]);
        const float y_c3 = (-y[0] + 3 * y[1] - 3 * y[2] + y[3]);

        const float x_c0 = 2 * x[1];
        const float x_c1 = (-x[0] + x[1]);
        const float x_c2 = (2 * x[0] - 5 * x[1] + 4 * x[2] - x[3]);
        const float x_c3 = (-x[0] + 3 * x[1] - 3 * x[2] + x[3]);

        for (int j = 0; j <= 10; ++j)
        {
            // 我大胆猜想编译器应该会把它优化为成一张表
            float t2 = t * t;
            float t3 = t2 * t;

            // 计算px和py
            pCleanBuff_y[j] = (Coord) (0.5 * (y_c0 + y_c1 * t + y_c2 * t2 + y_c3 * t3));
            pBuff_x[j] = (Coord) (0.5 * (x_c0 + x_c1 * t + x_c2 * t2 + x_c3 * t3));

            LCD_Set_Pixel(pBuff_x[j], pCleanBuff_y[j], bg_color);// 清除旧像素点
            LCD_Set_Pixel(pBuff_x[j], pDrawBuff_y[j], color);// 绘制新像素点
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
template<typename Coord>
template<WaveCurveType draw_type, typename Data, typename Color>
[[maybe_unused]]auto
WaveCurve<Coord>::draw_curve(Data data[], Coord N, Data value, Coord Start_x, Coord Start_y, Coord Width, Coord Height,
                             Data Max_Value, Color bg_color, Color color) -> void
{
    /*******************前置条件*********************/
    // 定义一个常量，使用lambda表达式这种解决方法我是真没想到，不过这里用静态内联函数+模板+constexpr的组合
    constexpr uint16_t once_points = getOncePoints<draw_type>();

    // 如果数据点过少，则无法画线
    // 数据点一般是固定的，比较多。当然也不能排除这种情况
//    if (N < once_points) return;

    const float ratio = (Height - 1.0f) / Max_Value;
    const float step = Width / (N - 1.0f);// 步长

    Coord x[once_points], y[once_points];
    Coord y_temp;// 不使用会被自动优化掉，不用担心

    // 进行前置计算
    if constexpr (draw_type == WaveCurveType::Interpolated_Line)
        draw_buff_y[0] = (Coord) (Start_y + Height - value * ratio);
    else
    {
        // 如果你问我为什么不是计算最新的点，我只能说我也不知道。试了很久，无意中发现置零就不会漏墨，于是就置零了
        // 置零应该是不画最右段的曲线
        // 别细问，问就是理论对不上实践，fxxk玄学
        memset((void *) pDrawBuff_y, 0, buffer_size * sizeof(Coord));
    }

    /*******************绘制曲线*********************/

    // 从右往左刷新，更符合直观上的感受
    for (int i = N - once_points; i >= 0; --i)
    {
        // 确认控制点
        for (int j = 0; j < once_points; ++j)
        {
            x[j] = (Coord) (Start_x + (float) (i + j) * step);
            y[j] = (Coord) (Start_y + Height - (data[get_index(N,i + j)] * ratio));
        }

        // 绘制曲线
        DrawFunction<draw_type, Color>::draw(x, y, bg_color, color);
    }

    /********************更新数据*******************/
    add_index(N);
    data[index_offset?index_offset-1: N - 1] = value;
//    data[N - 1] = value;// 这个错误也挺具备观赏性的
//    memmove(data, data + 1, (N - 1) * sizeof(data[0]));// 将数据左移一位
//    data[N - 1] = value;// 一放在前面就会卡死，不知缘由 总不能是该死的优化导致的吧
}


// ***************************绘制功能代码****************************

/**
 * @brief 线性插值算法
 * @param start_x
 * @param start_y
 * @param y
 * @param length
 * @param bg_color
 * @param color
 */
void draw_interpolated_line_simple(uint16_t start_x, uint16_t start_y, const uint8_t y[], size_t length,size_t start_index, size_t array_length,uint16_t color);
void draw_interpolated_line_with_dirty_point(uint16_t start_x, uint16_t start_y, const uint8_t y[], size_t length, size_t start_index, size_t array_length,
                                             uint16_t color, Point dirtyPoints[], size_t& dirtyPointsCount, size_t MAX_DIRTY_POINTS);
static void draw_bresenham_segment(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint16_t color);
static void draw_bresenham_segment_with_dirty_point(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint16_t color,Point dirtyPoints[], size_t& dirtyPointsCount,
                                                    size_t MAX_DIRTY_POINTS);
static bool is_point_in_list(int32_t x, int32_t y, const Point dirtyPoints[], size_t dirtyPointsCount);
void clear_drawn_points(uint16_t backgroundColor,Point dirtyPoints[], size_t& dirtyPointsCount);
static void draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
void draw_dividers(uint16_t x, uint16_t y, uint16_t width, uint16_t height, size_t h_divs, size_t v_divs, uint16_t color, uint16_t margin);
void draw_dividers_with_dirty_points(uint16_t x, uint16_t y, uint16_t width, uint16_t height, size_t h_divs, size_t v_divs, uint16_t color,
                                     uint16_t margin, uint16_t* dirtyPoints, size_t& dirtyPointsCount, size_t maxDirtyPoints);

#endif //SIMULATOR_WAVECURVE_HPP