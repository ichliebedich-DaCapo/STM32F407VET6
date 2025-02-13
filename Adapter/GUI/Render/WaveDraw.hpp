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

void draw_interpolated_wave(uint16_t x, uint16_t y,
                            size_t height,size_t width,size_t margin,
                            const uint8_t *p_SOC_Buff,uint8_t *p_DST_Buff,
                            size_t length,size_t start_index, size_t array_length,
                            uint16_t color, uint16_t background_color);


void clear_interpolated_wave(uint16_t x, uint16_t y,
                             size_t height,size_t width,size_t margin,
                             const uint8_t *p_DST_Buff,
                             size_t length,size_t start_index, size_t array_length,
                             uint16_t background_color);


void clear_drawn_points(uint16_t backgroundColor,Point dirtyPoints[], size_t& dirtyPointsCount);

/***************************模版类代码****************************/

template<typename Coord=uint32_t>
class WaveDraw {
public:


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
// 插值算法的特化实现(画曲线)
template<typename Coord>
template<typename Color>
struct WaveDraw<Coord>::DrawFunction<WaveDrawType::Interpolated_Line, Color> {
    template<typename Data>
    static void draw(const DrawParams& params,
                     const Data* src_buf, Data* dst_buf,
                     Color color, Color bg_color)
    {
        // 完全复用原有算法实现
        ::draw_interpolated_wave(params.x, params.y, params.height, params.width, params.margin,
                                 src_buf, dst_buf,
                                 params.length,params.start_index, params.array_length,
                                 color, bg_color);
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
        ::clear_interpolated_wave(params.x, params.y, params.height, params.width, params.margin,
                                 dst_buf,
                                 params.length,params.start_index, params.array_length,
                                  bg_color);
    }
};



#endif //SIMULATOR_WAVEDRAW_HPP