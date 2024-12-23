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

// 辅助函数：Bresenham算法绘制两点之间的线段,记录脏点列表
static void draw_bresenham_segment_with_dirty_point(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint16_t color,Point dirtyPoints[], size_t& dirtyPointsCount, size_t MAX_DIRTY_POINTS)
{
    int32_t dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int32_t dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int32_t err = dx + dy, e2; /* error value e_xy */

    for(;;){
        // 绘制当前点
        LCD_Set_Pixel((uint16_t)x0, (uint16_t)y0, color);

        // 将当前点添加到脏点列表（如果不在列表中）
        if (dirtyPointsCount < MAX_DIRTY_POINTS && !is_point_in_list(x0, y0, dirtyPoints, dirtyPointsCount)) {
            dirtyPoints[dirtyPointsCount].x = (uint16_t)x0;
            dirtyPoints[dirtyPointsCount].y = (uint16_t)y0;
            dirtyPointsCount++;
        }

        // 如果到达终点，退出循环
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
    float prev_y = start_y - y[start_index] *  201.0f/255;

    for (size_t i = 1; i < actual_length; ++i) {
        // 计算当前点的 x 和 y 坐标
        float current_x = start_x + ((float)i / (actual_length - 1)) * 317.0f; // 使用浮点数进行插值，修改这行要同时修改右移函数和左移、start_x、start_y
        float current_y = start_y - y[start_index + i] * 201.0f/255;

        // 使用 Bresenham 算法绘制上一个点和当前点之间的线段
        draw_bresenham_segment((int32_t)round(prev_x), (int32_t)round(prev_y), (int32_t)round(current_x), (int32_t)round(current_y), color);

        // 更新上一个点为当前点
        prev_x = current_x;
        prev_y = current_y;
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
void draw_interpolated_line_with_dirty_point(uint16_t start_x, uint16_t start_y, const uint8_t y[], size_t length, size_t start_index,
                                             size_t array_length, uint16_t color, Point dirtyPoints[], size_t& dirtyPointsCount, size_t MAX_DIRTY_POINTS) {
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
    float prev_y = start_y - y[start_index] * 201.0f / 255;

    for (size_t i = 1; i < actual_length; ++i) {
        // 计算当前点的 x 和 y 坐标
        float current_x = start_x + ((float)i / (actual_length - 1)) * 317.0f;
        float current_y = start_y - y[start_index + i] * 201.0f / 255;

        // 使用 Bresenham 算法绘制上一个点和当前点之间的线段
        draw_bresenham_segment_with_dirty_point((int32_t)round(prev_x), (int32_t)round(prev_y), (int32_t)round(current_x), (int32_t)round(current_y), color, dirtyPoints, dirtyPointsCount, MAX_DIRTY_POINTS);

        // 更新上一个点为当前点
        prev_x = current_x;
        prev_y = current_y;
    }
}
// 辅助函数：检查点是否已经在脏点列表中
static bool is_point_in_list(int32_t x, int32_t y, const Point dirtyPoints[], size_t dirtyPointsCount)
{
    for (size_t i = 0; i < dirtyPointsCount; ++i)
    {
        if (dirtyPoints[i].x == (uint16_t)x && dirtyPoints[i].y == (uint16_t)y) {
            return true;
        }
    }
    return false;
}
// 清屏函数：清除所有曾经绘制过的点
void clear_drawn_points(uint16_t backgroundColor,Point dirtyPoints[], size_t& dirtyPointsCount)
{
    // 遍历脏点列表，清除每个点
    for (size_t i = 0; i < dirtyPointsCount; ++i)
    {
        Point point = dirtyPoints[i];
        LCD_Set_Pixel(point.x, point.y, backgroundColor);
    }

    // 清空脏点列表
    dirtyPointsCount = 0;
}
// 辅助函数：绘制一条线段
static void draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) {
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

// 绘制分割线
void draw_dividers(uint16_t x, uint16_t y, uint16_t width, uint16_t height, size_t h_divs, size_t v_divs, uint16_t color, uint16_t margin) {
    // 计算每个分割线的间隔
    uint16_t h_interval = width / h_divs;
    uint16_t v_interval = height / v_divs;

    // 绘制水平分割线
    for (size_t i = 1; i < h_divs; ++i) {  // 从1开始，避免绘制最左边和最右边的线
        uint16_t current_x = x + i * h_interval;
        draw_line(current_x, y + margin, current_x, y + height - margin, color); // 水平分割线
    }

    // 绘制垂直分割线
    for (size_t i = 1; i < v_divs; ++i) {  // 从1开始，避免绘制最顶部和最底部的线
        uint16_t current_y = y + i * v_interval;
        draw_line(x + margin, current_y, x + width - margin, current_y, color); // 垂直分割线
    }
}

// 绘制分割线
void draw_dividers_with_dirty_points(uint16_t x, uint16_t y, uint16_t width, uint16_t height, size_t h_divs, size_t v_divs, uint16_t color, uint16_t margin,
                                     uint16_t* dirtyPoints, size_t& dirtyPointsCount, size_t maxDirtyPoints) {
    // 计算每个分割线的间隔
    uint16_t h_interval = width / h_divs;
    uint16_t v_interval = height / v_divs;

    // 绘制水平分割线
    for (size_t i = 1; i < h_divs; ++i) {  // 从1开始，避免绘制最左边和最右边的线
        uint16_t current_x = x + i * h_interval;
        draw_line(current_x, y + margin, current_x, y + height - margin, color); // 水平分割线
        if (dirtyPointsCount < maxDirtyPoints) {
            dirtyPoints[dirtyPointsCount++] = current_x;
            dirtyPoints[dirtyPointsCount++] = y + margin;
            dirtyPoints[dirtyPointsCount++] = current_x;
            dirtyPoints[dirtyPointsCount++] = y + height - margin;
        }
    }

    // 绘制垂直分割线
    for (size_t i = 1; i < v_divs; ++i) {  // 从1开始，避免绘制最顶部和最底部的线
        uint16_t current_y = y + i * v_interval;
        draw_line(x + margin, current_y, x + width - margin, current_y, color); // 垂直分割线
        if (dirtyPointsCount < maxDirtyPoints) {
            dirtyPoints[dirtyPointsCount++] = x + margin;
            dirtyPoints[dirtyPointsCount++] = current_y;
            dirtyPoints[dirtyPointsCount++] = x + width - margin;
            dirtyPoints[dirtyPointsCount++] = current_y;
        }
    }
}

