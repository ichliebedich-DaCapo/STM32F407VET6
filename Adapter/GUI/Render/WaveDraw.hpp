#ifndef SIMULATOR_DRAW_WAVE_HPP
#define SIMULATOR_DRAW_WAVE_HPP
/* 预编译命令 */
// 用于区分模拟器和真机
#ifdef ARM_MATH_CM4

#include "lcd.h"

#else

#include "simulator.hpp"

#endif

#include "ui.hpp"
//结构体
typedef struct {
    uint16_t x;
    uint16_t y;
//    bool latch_state;
} Point;


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
                                     uint16_t margin, Point* dirtyPoints, size_t& dirtyPointsCount, size_t maxDirtyPoints);
void draw_interpolated_wave(uint16_t start_x, uint16_t start_y,
                            const uint8_t *p_SOC_Buff,uint8_t *p_DST_Buff,
                            size_t length,size_t start_index, size_t array_length,
                            uint16_t color, uint16_t background_color);
void clear_interpolated_wave(uint16_t start_x, uint16_t start_y,const uint8_t *p_DST_Buff,
                             size_t length,size_t start_index, size_t array_length,
                             uint16_t background_color);
void draw_dashed_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color, uint16_t dash_length, uint16_t gap_length);
void draw_dashed_dividers(uint16_t x, uint16_t y, uint16_t width, uint16_t height, size_t h_divs, size_t v_divs, uint16_t color, uint16_t margin, uint16_t dash_length, uint16_t gap_length);
#endif //SIMULATOR_WAVECURVE_HPP