//
// Created by DV on 2025/2/12.
//

#ifndef SIMULATOR_BASICDRAW_HPP
#define SIMULATOR_BASICDRAW_HPP
#ifdef ARM_MATH_CM4

#include "lcd.h"
#include "stdint.h"
#else
#include "simulator.hpp"
#endif
#include <stddef.h>
#include <cmath>

static void draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
void draw_dashed_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color, uint16_t dash_length, uint16_t gap_length);
void draw_dividers(uint16_t x, uint16_t y, uint16_t width, uint16_t height, size_t h_divs, size_t v_divs, uint16_t color, uint16_t margin);
void draw_dashed_dividers(uint16_t x, uint16_t y,  uint16_t height,uint16_t width, size_t h_divs, size_t v_divs,
                          uint16_t color, uint16_t margin, uint16_t dash_length, uint16_t gap_length);
#endif //SIMULATOR_BASICDRAW_HPP