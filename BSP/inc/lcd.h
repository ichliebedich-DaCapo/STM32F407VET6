//
// Created by 34753 on 2024/9/22.
//

#ifndef FURINA_LCD_H
#define FURINA_LCD_H


#include "App_Conf.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


void LCD_Set_Window(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey);// 设置显示窗口

void LCD_Clear(uint16_t color);// 清屏

void LCD_Color_Fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, const uint16_t *color);// 填充矩形区域

void LCD_Color_Clean(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t color);// 清除矩形区域

void LCD_Set_Pixel(uint16_t x, uint16_t y, uint16_t color);// 设置一个像素

void lcd_init(void);// 液晶初始化

void LCD_direction(uint8_t direction);// 设置显示方向

void lcd_flush(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, const uint16_t *color_p);// 填充矩形区域

#ifdef __cplusplus
}
#endif


#endif //FURINA_LCD_H
