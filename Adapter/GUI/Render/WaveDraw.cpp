//
// Created by DV on 2024/12/22.
//

#include "WaveDraw.hpp"

#include <stdint.h>
#include <stddef.h>
#include <cmath>

/*
 * 辅助函数：Bresenham算法绘制两点之间的线段
 *  x0 和 y0 是线段的起点坐标
 *  x1 和 y1 是线段的终点坐标
 *  color 是要绘制的线的颜色
 */
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
 * 绘制插值曲线
 * @param start_x 起始点的 x 坐标,左下角原点
 * @param start_y 起始点的 y 坐标,左下角原点
 * @param p_DST_Buff    当前帧目标数据缓存区
 * @param p_SOC_Buff    源数据缓存区
 * @param length 显示长度（从start_index开始取length个数）
 * @param start_index 数组起始索引
 * @param array_length y数组的实际长度
 * @param color 颜色
 */
void draw_interpolated_wave(uint16_t start_x, uint16_t start_y,
                            const uint8_t *p_SOC_Buff,uint8_t *p_DST_Buff,
                            size_t length,size_t start_index, size_t array_length,
                            uint16_t color, uint16_t background_color)
{
    // 检查输入参数的有效性
    if (length == 0 || start_index >= array_length) return;

    // 确保不会访问越界的数组元素
    size_t end_index = start_index + length;
    if (end_index > array_length) {
        length = array_length - start_index;
        if (length == 0) return;
    }

    //初始化上一次数据的第一个点
    float last_prev_x = start_x;
    float last_prev_y = start_y - p_DST_Buff[0] * (border_info::height-4*border_info::margin+1) / 255;

    // 初始化第一个点
    float prev_x = start_x;
    float prev_y = start_y - p_SOC_Buff[start_index] * (border_info::height-4*border_info::margin+1) / 255;

    for (size_t i = 1; i < length; ++i) {
        // 计算当前点的 x 和 y 坐标
        float current_x = start_x + ((float)i / (length - 1)) * (border_info::width-4*border_info::margin+1);
        float current_y = start_y - p_SOC_Buff[start_index + i] * (border_info::height-4*border_info::margin+1) / 255;

        // 计算上一次数据的当前点的 x 和 y 坐标
        float last_current_x = start_x + ((float) i / (length - 1)) * (border_info::width-4*border_info::margin+1);
        float last_current_y = start_y - p_DST_Buff[i] * (border_info::height-4*border_info::margin+1) / 255;
        // 清除旧线段
        draw_bresenham_segment((int32_t) round(last_prev_x), (int32_t) round(last_prev_y),
                               (int32_t) round(last_current_x), (int32_t) round(last_current_y), background_color);
        // 绘制新线段
        draw_bresenham_segment((int32_t)round(prev_x), (int32_t)round(prev_y),
                               (int32_t)round(current_x), (int32_t)round(current_y), color);

        // 更新上一个点为当前点
        last_prev_x= last_current_x;
        last_prev_y= last_current_y;
        prev_x = current_x;
        prev_y = current_y;
    }

    // 将当前帧的波形数据 p_DST_Buff 复制到 p_DST_Buff_B 中，准备用于下一帧的绘制。
    for (size_t i = 0; i < length; i++)
    {
        p_DST_Buff[i] = p_SOC_Buff[start_index + i];
    }
}

/*
 * 清除波形
 * @param start_x 起始点的 x 坐标,左下角原点
 * @param start_y 起始点的 y 坐标,左下角原点
 * @param p_DST_Buff    当前帧目标数据缓存区
 * @param length 显示长度（从start_index开始取length个数）
 * @param start_index 数组起始索引
 * @param array_length y数组的实际长度
 * @param background_color 背景颜色
 */
void clear_interpolated_wave(uint16_t start_x, uint16_t start_y,const uint8_t *p_DST_Buff,
                             size_t length,size_t start_index, size_t array_length,
                             uint16_t background_color)
{
    // 检查输入参数的有效性
    if (length == 0 || start_index >= array_length) return;

    // 确保不会访问越界的数组元素
    size_t end_index = start_index + length;
    if (end_index > array_length) {
        length = array_length - start_index;
        if (length == 0) return;
    }

    //初始化上一次数据的第一个点
    float last_prev_x = start_x;
    float last_prev_y = start_y - p_DST_Buff[0] * (border_info::height-4*border_info::margin+1) / 255;

    for (size_t i = 1; i < length; ++i) {
        // 计算上一次数据的当前点的 x 和 y 坐标
        float last_current_x = start_x + ((float) i / (length - 1)) * (border_info::width-4*border_info::margin+1);
        float last_current_y = start_y - p_DST_Buff[i] * (border_info::height-4*border_info::margin+1) / 255;
        // 清除旧线段
        draw_bresenham_segment((int32_t) round(last_prev_x), (int32_t) round(last_prev_y),
                               (int32_t) round(last_current_x), (int32_t) round(last_current_y), background_color);
        // 更新上一个点为当前点
        last_prev_x= last_current_x;
        last_prev_y= last_current_y;
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
    for (size_t i = 0; i < dirtyPointsCount; ++i) {
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
                                     Point * dirtyPoints, size_t& dirtyPointsCount, size_t maxDirtyPoints) {
    // 计算每个分割线的间隔
    uint16_t h_interval = width / h_divs;
    uint16_t v_interval = height / v_divs;

    // 绘制水平分割线
    for (size_t i = 1; i < h_divs; ++i) {  // 从1开始，避免绘制最左边和最右边的线
        uint16_t current_x = x + i * h_interval;
        draw_bresenham_segment_with_dirty_point(current_x, y + margin, current_x, y + height - margin, color, dirtyPoints, dirtyPointsCount, maxDirtyPoints); // 水平分割线
    }

    // 绘制垂直分割线
    for (size_t i = 1; i < v_divs; ++i) {  // 从1开始，避免绘制最顶部和最底部的线
        uint16_t current_y = y + i * v_interval;
        draw_bresenham_segment_with_dirty_point(x + margin, current_y, x + width - margin, current_y, color, dirtyPoints, dirtyPointsCount, maxDirtyPoints); // 垂直分割线
    }
}

// 绘制虚线
void draw_dashed_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color, uint16_t dash_length, uint16_t gap_length) {
    int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
    int dy = -abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
    int err = dx + dy, e2;  // error value e_xy

    bool is_drawing = true; // 控制是否绘制当前段
    int current_length = 0; // 当前段的长度

    while (true) {
        LCD_Set_Pixel(x1, y1, is_drawing ? color : 0); // 0 表示不绘制（即背景色或透明）
        if (x1 == x2 && y1 == y2) break;
        e2 = 2 * err;
        if (e2 >= dy) { // e_xy+e_x > 0
            err += dy; x1 += sx;
        }
        if (e2 <= dx) { // e_xy+e_y < 0
            err += dx; y1 += sy;
        }

        // 更新当前段的长度，并检查是否需要切换绘制状态
        current_length++;
        if ((is_drawing && current_length == dash_length) || (!is_drawing && current_length == gap_length)) {
            is_drawing = !is_drawing; // 切换绘制状态
            current_length = 0;       // 重置当前段长度
        }
    }
}

// 使用虚线绘制分割线
/*
 * @param x: 分割线起始点的 x 坐标
 * @param y: 分割线起始点的 y 坐标
 * @param width: 分割线的宽度
 * @param height: 分割线的高度
 * @param h_divs: 水平方向上的分割线数量
 * @param v_divs: 垂直方向上的分割线数量
 * @param color: 分割线的颜色
 * @param margin: 分割线与边界的距离
 * @param dash_length: 虚线段的长度
 * @param gap_length: 虚线段之间的间隔长度
 */
void draw_dashed_dividers(uint16_t x, uint16_t y, uint16_t width, uint16_t height, size_t h_divs, size_t v_divs, uint16_t color, uint16_t margin, uint16_t dash_length, uint16_t gap_length) {
    uint16_t h_interval = width / h_divs;
    uint16_t v_interval = height / v_divs;

    for (size_t i = 1; i < h_divs; ++i) {
        uint16_t current_x = x + i * h_interval;
        draw_dashed_line(current_x, y + margin, current_x, y + height - margin, color, dash_length, gap_length);
    }

    for (size_t i = 1; i < v_divs; ++i) {
        uint16_t current_y = y + i * v_interval;
        draw_dashed_line(x + margin, current_y, x + width - margin, current_y, color, dash_length, gap_length);
    }
}