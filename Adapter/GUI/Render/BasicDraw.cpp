//
// Created by DV on 2025/2/12.
//

#include "BasicDraw.hpp"


/***********************绘制分割线*********************/
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
void draw_dashed_dividers(uint16_t x, uint16_t y,  uint16_t height,uint16_t width, size_t h_divs, size_t v_divs, uint16_t color, uint16_t margin, uint16_t dash_length, uint16_t gap_length) {
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