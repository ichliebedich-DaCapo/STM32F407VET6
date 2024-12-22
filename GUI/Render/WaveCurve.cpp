//
// Created by DV on 2024/12/22.
//

#include "WaveCurve.hpp"

#include <stdint.h>
#include <stddef.h>

// 辅助函数：Bresenham算法绘制两点之间的线段
static void draw_bresenham_segment(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint16_t color) {
    int32_t dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int32_t dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int32_t err = dx + dy, e2; /* error value e_xy */

    for(;;){
        LCD_Set_Pixel((uint16_t)x0, (uint16_t)y0, color);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { /* e_xy+e_x > 0 */
            err += dy; x0 += sx;
        }
        if (e2 <= dx) { /* e_xy+e_y < 0 */
            err += dx; y0 += sy;
        }
    }
}
/*
 * 绘制一个简单的插值线
 * @param start_x 起始点的 x 坐标,左下角原点
 * @param start_y 起始点的 y 坐标,左下角原点
 * @param y y轴坐标数组
 * @param length 显示长度（从start_index开始取length个数）
 * @param start_index 数组起始索引
 * @param array_length y数组的实际长度
 * @param color 颜色
 */
void draw_interpolated_line_simple(uint16_t start_x, uint16_t start_y, const uint8_t y[], size_t length, size_t start_index, size_t array_length, uint16_t color) {
    // 检查输入参数的有效性
    if (length == 0 || start_index >= array_length) return; // 如果没有数据点或起始索引超出范围，则直接返回

    // 确保不会访问越界的数组元素
    size_t end_index = start_index + length;
    if (end_index > array_length) {
        length = array_length - start_index;
        if (length == 0) return; // 如果调整后长度为0，则直接返回
    }

    // 计算实际需要绘制的长度
    size_t actual_length = length;
    if (actual_length == 0) return; // 如果实际长度为0，则直接返回

    // 初始化第一个点
    float prev_x = start_x;
    float prev_y = start_y - y[start_index] * 0.5f;

    for (size_t i = 1; i < actual_length; ++i) {
        // 计算当前点的 x 和 y 坐标
        float current_x = start_x + ((float)i / (actual_length - 1)) * 321.0f; // 使用浮点数进行插值，修改这行要同时修改右移函数和左移、start_x、start_y
        float current_y = start_y - y[start_index + i] *  0.7f;//205.0f/255

        // 使用 Bresenham 算法绘制上一个点和当前点之间的线段
        draw_bresenham_segment((int32_t)round(prev_x), (int32_t)round(prev_y), (int32_t)round(current_x), (int32_t)round(current_y), color);

        // 更新上一个点为当前点
        prev_x = current_x;
        prev_y = current_y;
    }
}