//
// Created by DV on 2024/12/22.
//

#include "WaveDraw.hpp"
//
///***************辅助函数*****************/
//// 辅助函数：检查点是否已经在脏点列表中
//static bool inline is_point_in_list(int32_t x, int32_t y, const Point dirtyPoints[], size_t dirtyPointsCount)
//{
//    for (size_t i = 0; i < dirtyPointsCount; ++i) {
//        if (dirtyPoints[i].x == (uint16_t)x && dirtyPoints[i].y == (uint16_t)y) {
//            return true;
//        }
//    }
//    return false;
//}
///***************辅助算法*****************/
///*
// * 辅助算法：Bresenham算法绘制两点之间的线段
// *  x0 和 y0 是线段的起点坐标
// *  x1 和 y1 是线段的终点坐标
// *  color 是要绘制的线的颜色
// */
//static void inline draw_bresenham_segment(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint16_t color) {
//    int32_t dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
//    int32_t dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
//    int32_t err = dx + dy, e2; /* error value e_xy */
//
//    for(;;){
//        LCD_Set_Pixel((uint16_t)x0, (uint16_t)y0, color);
//
//        if (x0 == x1 && y0 == y1) break;
//        e2 = 2 * err;
//        if (e2 >= dy) { /* e_xy+e_x > 0 */
//            err += dy; x0 += sx;
//        }
//        if (e2 <= dx) { /* e_xy+e_y < 0 */
//            err += dx; y0 += sy;
//        }
//    }
//}
//
//// 辅助算法：Bresenham算法绘制两点之间的线段,记录脏点列表
//static void inline draw_bresenham_segment_with_dirty_point(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint16_t color,Point dirtyPoints[], size_t& dirtyPointsCount, size_t MAX_DIRTY_POINTS)
//{
//    int32_t dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
//    int32_t dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
//    int32_t err = dx + dy, e2; /* error value e_xy */
//
//    for(;;){
//        // 绘制当前点
//        LCD_Set_Pixel((uint16_t)x0, (uint16_t)y0, color);
//
//        // 将当前点添加到脏点列表（如果不在列表中）
//        if (dirtyPointsCount < MAX_DIRTY_POINTS && !is_point_in_list(x0, y0, dirtyPoints, dirtyPointsCount)) {
//            dirtyPoints[dirtyPointsCount].x = (uint16_t)x0;
//            dirtyPoints[dirtyPointsCount].y = (uint16_t)y0;
//            dirtyPointsCount++;
//        }
//
//        // 如果到达终点，退出循环
//        if (x0 == x1 && y0 == y1) break;
//
//        e2 = 2 * err;
//        if (e2 >= dy) { /* e_xy+e_x > 0 */
//            err += dy; x0 += sx;
//        }
//        if (e2 <= dx) { /* e_xy+e_y < 0 */
//            err += dx; y0 += sy;
//        }
//    }
//}
///******************绘制曲线函数******************/
///*
// * 绘制插值波形
// * @param start_x 起始点的 x 坐标,左下角原点
// * @param start_y 起始点的 y 坐标,左下角原点
// * @param p_DST_Buff    当前帧目标数据缓存区
// * @param p_SOC_Buff    源数据缓存区
// * @param length 显示长度（从start_index开始取length个数）
// * @param start_index 数组起始索引
// * @param array_length y数组的实际长度
// * @param color 颜色
// */
//void draw_interpolated_wave(uint16_t x, uint16_t y,
//                            size_t height,size_t width,size_t margin,
//                            const uint8_t *p_SOC_Buff,uint8_t *p_DST_Buff,
//                            size_t length,size_t start_index, size_t array_length,
//                            uint16_t color, uint16_t background_color)
//{
//    uint16_t start_x = x + 2 * margin;
//    uint16_t start_y = y + height - 2 * margin;
//    // 检查输入参数的有效性
//    if (length == 0 || start_index >= array_length) return;
//
//    // 确保不会访问越界的数组元素
//    size_t end_index = start_index + length;
//    if (end_index > array_length) {
//        length = array_length - start_index;
//        if (length == 0) return;
//    }
//
//    //初始化上一次数据的第一个点
//    float last_prev_x = start_x;
//    float last_prev_y = start_y - p_DST_Buff[0] * (height-4*margin+1) / 255;
//
//    // 初始化第一个点
//    float prev_x = start_x;
//    float prev_y = start_y - p_SOC_Buff[start_index] * (height-4*margin+1) / 255;
//
//    for (size_t i = 1; i < length; ++i) {
//        // 计算当前点的 x 和 y 坐标
//        float current_x = start_x + ((float)i / (length - 1)) * (width-4*margin+1);
//        float current_y = start_y - p_SOC_Buff[start_index + i] * (height-4*margin+1) / 255;
//
//        // 计算上一次数据的当前点的 x 和 y 坐标
//        float last_current_x = start_x + ((float) i / (length - 1)) * (width-4*margin+1);
//        float last_current_y = start_y - p_DST_Buff[i] * (height-4*margin+1) / 255;
//        // 清除旧线段
//        draw_bresenham_segment((int32_t) round(last_prev_x), (int32_t) round(last_prev_y),
//                               (int32_t) round(last_current_x), (int32_t) round(last_current_y), background_color);
//        // 绘制新线段
//        draw_bresenham_segment((int32_t)round(prev_x), (int32_t)round(prev_y),
//                               (int32_t)round(current_x), (int32_t)round(current_y), color);
//
//        // 更新上一个点为当前点
//        last_prev_x= last_current_x;
//        last_prev_y= last_current_y;
//        prev_x = current_x;
//        prev_y = current_y;
//    }
//
//    // 将当前帧的波形数据 p_DST_Buff 复制到 p_DST_Buff_B 中，准备用于下一帧的绘制。
//    for (size_t i = 0; i < length; i++)
//    {
//        p_DST_Buff[i] = p_SOC_Buff[start_index + i];
//    }
//}
//
///*
// * 清除波形
// * @param start_x 起始点的 x 坐标,左下角原点
// * @param start_y 起始点的 y 坐标,左下角原点
// * @param p_DST_Buff    当前帧目标数据缓存区
// * @param length 显示长度（从start_index开始取length个数）
// * @param start_index 数组起始索引
// * @param array_length y数组的实际长度
// * @param background_color 背景颜色
// */
//void clear_interpolated_wave(uint16_t x, uint16_t y,
//                             size_t height,size_t width,size_t margin,
//                             const uint8_t *p_DST_Buff,
//                             size_t length,size_t start_index, size_t array_length,
//                             uint16_t background_color)
//{
//    uint16_t start_x = x + 2 * margin;            //82
//    uint16_t start_y = y + height - 2 * margin;   //240
//    // 检查输入参数的有效性
//    if (length == 0 || start_index >= array_length) return;
//
//    // 确保不会访问越界的数组元素
//    size_t end_index = start_index + length;
//    if (end_index > array_length) {
//        length = array_length - start_index;
//        if (length == 0) return;
//    }
//
//    //初始化上一次数据的第一个点
//    float last_prev_x = start_x;
//    float last_prev_y = start_y - p_DST_Buff[0] * (height-4*margin+1) / 255;
//
//    for (size_t i = 1; i < length; ++i) {
//        // 计算上一次数据的当前点的 x 和 y 坐标
//        float last_current_x = start_x + ((float) i / (length - 1)) * (width-4*margin+1);
//        float last_current_y = start_y - p_DST_Buff[i] * (height-4*margin+1) / 255;
//        // 清除旧线段
//        draw_bresenham_segment((int32_t) round(last_prev_x), (int32_t) round(last_prev_y),
//                               (int32_t) round(last_current_x), (int32_t) round(last_current_y), background_color);
//        // 更新上一个点为当前点
//        last_prev_x= last_current_x;
//        last_prev_y= last_current_y;
//    }
//}
//
//// 清屏函数：清除所有曾经绘制过的点
//void clear_drawn_points(uint16_t backgroundColor,Point dirtyPoints[], size_t& dirtyPointsCount)
//{
//    // 遍历脏点列表，清除每个点
//    for (size_t i = 0; i < dirtyPointsCount; ++i)
//    {
//        Point point = dirtyPoints[i];
//        LCD_Set_Pixel(point.x, point.y, backgroundColor);
//    }
//
//    // 清空脏点列表
//    dirtyPointsCount = 0;
//}
//
//
