//
// Created by DV on 2024/12/22.
//
#ifndef SIMULATOR_WAVEDRAW_HPP
#define SIMULATOR_WAVEDRAW_HPP


/* 预编译命令 */
// 用于区分模拟器和真机
#ifdef ARM_MATH_CM4

#include "lcd.h"
#include "stdint.h"
#include <stddef.h>
#include <cmath>


#else

#include "simulator.hpp"

#endif
#include <cstring> // 引入string.h以使用memmove
#include <valarray>
/***************************结构体和枚举体****************************/
//结构体
typedef struct {
    uint16_t x;
    uint16_t y;
} Point;



// 枚举体
enum class WaveDrawType
{
    // 插值画线
    Interpolated_Line
};

/***************************绘制功能代码****************************/
//留得青山在
//void draw_interpolated_wave(uint16_t x, uint16_t y,
//                            size_t height,size_t width,size_t margin,
//                            const uint8_t *p_SOC_Buff,uint8_t *p_DST_Buff,
//                            size_t length,size_t start_index, size_t array_length,
//                            uint16_t color, uint16_t background_color);
//
//
//void clear_interpolated_wave(uint16_t x, uint16_t y,
//                             size_t height,size_t width,size_t margin,
//                             const uint8_t *p_DST_Buff,
//                             size_t length,size_t start_index, size_t array_length,
//                             uint16_t background_color);
//
//
//void clear_drawn_points(uint16_t backgroundColor,Point dirtyPoints[], size_t& dirtyPointsCount);

/***************************模版类代码****************************/

template<typename Coord=uint32_t>
class WaveDraw {
public:

    // 坐标点结构体模板化
    template<typename T = Coord>
    struct Point {
        T x;
        T y;
    };

    // 使用参数结构体封装
    struct DrawParams {
        Coord x;
        Coord y;
        Coord height;
        Coord width;
        Coord margin;
        Coord length;
        Coord start_index;
        Coord array_length;
    };

    template<WaveDrawType draw_type,
            typename Data=uint8_t,
            typename Color=uint16_t>
    static void draw_wave(const DrawParams& params,
                          const Data* src_buf,
                          Data* dst_buf,
                          Color fg_color,
                          Color bg_color) {
        // 通过特化分发到具体实现
        DrawFunction<draw_type, Color>::template draw<Data>(params, src_buf, dst_buf, fg_color, bg_color);
    }

    template<WaveDrawType draw_type,
            typename Data=uint8_t,
            typename Color=uint16_t>
    static void clear_wave(const DrawParams& params,
                          Data* dst_buf,
                          Color bg_color) {
        // 通过特化分发到具体实现
        ClearFunction<draw_type, Color>::template clear<Data>(params, dst_buf, bg_color);
    }

protected:
    // 声明模板特化的入口
    template<WaveDrawType T, typename Color>
    struct DrawFunction;  // 主模板只需声明

    template<WaveDrawType T, typename Color>
    struct ClearFunction;  // 主模板只需声明

};

/****************************特化实现****************************/

// 插值算法的特化实现（曲线）
template<typename Coord>
template<typename Color>
struct WaveDraw<Coord>::DrawFunction<WaveDrawType::Interpolated_Line, Color> {
    template<typename Data>
    static void draw(const DrawParams& params,
                     const Data* src_buf, Data* dst_buf,
                     Color color, Color bg_color)
    {
//        //完全复用原有算法实现
//        ::draw_interpolated_wave(params.x, params.y, params.height, params.width, params.margin,
//                                 src_buf, dst_buf,
//                                 params.length, params.start_index, params.array_length,
//                                 color, bg_color);
        // 参数有效性检查
        if (params.length  == 0 || params.start_index  >= params.array_length)  return;

        const Coord start_x = params.x + 2 * params.margin;
        const Coord start_y = params.y + params.height  - 2 * params.margin;
        const float y_scale = (params.height  - 4 * params.margin  + 1) / 255.0f;
        const float x_step = static_cast<float>(params.width  - 4 * params.margin  + 1) / (params.length  - 1);

        // 初始化坐标
        float last_prev_x = start_x;
        float last_prev_y = start_y - dst_buf[0] * y_scale;
        float prev_x = start_x;
        float prev_y = start_y - src_buf[params.start_index] * y_scale;

        for (size_t i = 1; i < params.length;  ++i) {
            // 当前帧坐标计算
            const float current_x = start_x + i * x_step;
            const float current_y = start_y - src_buf[params.start_index + i] * y_scale;

            // 上一帧坐标计算
            const float last_current_x = start_x + i * x_step;
            const float last_current_y = start_y - dst_buf[i] * y_scale;

            // Bresenham算法内联实现 - 擦除旧线段
            {
                int32_t x0 = round(last_prev_x);
                int32_t y0 = round(last_prev_y);
                int32_t x1 = round(last_current_x);
                int32_t y1 = round(last_current_y);

                int32_t dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
                int32_t dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
                int32_t err = dx + dy;

                while (true) {
                    LCD_Set_Pixel(x0, y0, bg_color);
                    if (x0 == x1 && y0 == y1) break;
                    int32_t e2 = 2 * err;
                    if (e2 >= dy) { err += dy; x0 += sx; }
                    if (e2 <= dx) { err += dx; y0 += sy; }
                }
            }

            // Bresenham算法内联实现 - 绘制新线段
            {
                int32_t x0 = round(prev_x);
                int32_t y0 = round(prev_y);
                int32_t x1 = round(current_x);
                int32_t y1 = round(current_y);

                int32_t dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
                int32_t dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
                int32_t err = dx + dy;

                while (true) {
                    LCD_Set_Pixel(x0, y0, color);
                    if (x0 == x1 && y0 == y1) break;
                    int32_t e2 = 2 * err;
                    if (e2 >= dy) { err += dy; x0 += sx; }
                    if (e2 <= dx) { err += dx; y0 += sy; }
                }
            }

            // 更新坐标
            last_prev_x = last_current_x;
            last_prev_y = last_current_y;
            prev_x = current_x;
            prev_y = current_y;
        }

        // 更新目标缓冲区
        memcpy(dst_buf, src_buf + params.start_index,  params.length  * sizeof(Data));
    }
};

// 插值算法的特化实现（擦曲线）
template<typename Coord>
template<typename Color>
struct WaveDraw<Coord>::ClearFunction<WaveDrawType::Interpolated_Line, Color> {
    template<typename Data>
    static void clear(const DrawParams& params,
                      Data* dst_buf,
                      Color bg_color)
    {
        // 完全复用原有算法实现
//        ::clear_interpolated_wave(params.x, params.y, params.height, params.width, params.margin,
//                                  dst_buf,
//                                  params.length,params.start_index, params.array_length,
//                                  bg_color);
        // 参数有效性检查
        if (params.length  == 0 || params.start_index  >= params.array_length)  return;

        const uint16_t start_x = params.x + 2 * params.margin;
        const uint16_t start_y = params.y + params.height  - 2 * params.margin;
        const float y_scale = (params.height  - 4 * params.margin  + 1) / 255.0f;
        const float x_step = static_cast<float>(params.width  - 4 * params.margin  + 1) / (params.length  - 1);

        // 初始化坐标
        float last_prev_x = start_x;
        float last_prev_y = start_y - dst_buf[0] * y_scale;

        for (size_t i = 1; i < params.length;  ++i) {
            // 上一帧坐标计算
            const float last_current_x = start_x + i * x_step;
            const float last_current_y = start_y - dst_buf[i] * y_scale;

            // Bresenham算法内联实现 - 擦除旧线段
            {
                int32_t x0 = static_cast<int32_t>(round(last_prev_x));
                int32_t y0 = static_cast<int32_t>(round(last_prev_y));
                int32_t x1 = static_cast<int32_t>(round(last_current_x));
                int32_t y1 = static_cast<int32_t>(round(last_current_y));

                int32_t dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
                int32_t dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
                int32_t err = dx + dy;

                while (true) {
                    LCD_Set_Pixel(x0, y0, bg_color);
                    if (x0 == x1 && y0 == y1) break;
                    int32_t e2 = 2 * err;
                    if (e2 >= dy) { err += dy; x0 += sx; }
                    if (e2 <= dx) { err += dx; y0 += sy; }
                }
            }

            // 更新坐标
            last_prev_x = last_current_x;
            last_prev_y = last_current_y;
        }
    }
};



#endif //SIMULATOR_WAVEDRAW_HPP