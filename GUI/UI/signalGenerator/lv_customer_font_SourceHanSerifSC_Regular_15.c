/*
 * Copyright 2024 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/
/*******************************************************************************
 * Size: 15 px
 * Bpp: 4
 * Opts: undefined
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl.h"
#endif

#ifndef LV_CUSTOMER_FONT_SOURCEHANSERIFSC_REGULAR_15
#define LV_CUSTOMER_FONT_SOURCEHANSERIFSC_REGULAR_15 1
#endif

#if LV_CUSTOMER_FONT_SOURCEHANSERIFSC_REGULAR_15

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0025 "%" */
    0x5, 0x78, 0x20, 0x0, 0x0, 0x60, 0x0, 0xd,
    0x1, 0xb0, 0x0, 0x6, 0x30, 0x0, 0x3b, 0x0,
    0xe0, 0x0, 0x27, 0x0, 0x0, 0x4a, 0x0, 0xe0,
    0x0, 0x80, 0x0, 0x0, 0x2b, 0x0, 0xe0, 0x6,
    0x21, 0x66, 0x0, 0xc, 0x3, 0x90, 0x26, 0xb,
    0x12, 0xa0, 0x1, 0x66, 0x0, 0x80, 0x1c, 0x0,
    0xd0, 0x0, 0x0, 0x7, 0x20, 0x3b, 0x0, 0xc2,
    0x0, 0x0, 0x35, 0x0, 0x2c, 0x0, 0xd1, 0x0,
    0x0, 0x80, 0x0, 0xd, 0x0, 0xc0, 0x0, 0x7,
    0x10, 0x0, 0x4, 0x88, 0x30, 0x0, 0x3, 0x0,
    0x0, 0x0, 0x0, 0x0,

    /* U+002B "+" */
    0x0, 0x0, 0x20, 0x0, 0x0, 0x0, 0xa, 0x0,
    0x0, 0x0, 0x0, 0xa0, 0x0, 0x0, 0x0, 0xa,
    0x0, 0x0, 0x49, 0x99, 0xe9, 0x99, 0x10, 0x0,
    0xa, 0x0, 0x0, 0x0, 0x0, 0xa0, 0x0, 0x0,
    0x0, 0xa, 0x0, 0x0, 0x0, 0x0, 0x30, 0x0,
    0x0,

    /* U+002D "-" */
    0x4c, 0xcc, 0x60,

    /* U+0031 "1" */
    0x0, 0x0, 0x0, 0x69, 0xe7, 0x0, 0x10, 0xc7,
    0x0, 0x0, 0xc7, 0x0, 0x0, 0xc6, 0x0, 0x0,
    0xc6, 0x0, 0x0, 0xc6, 0x0, 0x0, 0xc6, 0x0,
    0x0, 0xc7, 0x0, 0x0, 0xc7, 0x0, 0x0, 0xc7,
    0x0, 0x69, 0xec, 0x82,

    /* U+0036 "6" */
    0x0, 0x0, 0x0, 0x21, 0x0, 0x0, 0x77, 0x40,
    0x0, 0x1a, 0x50, 0x0, 0x0, 0xb6, 0x0, 0x0,
    0x5, 0xc0, 0x0, 0x0, 0xc, 0x77, 0x9a, 0x30,
    0xf, 0x80, 0x4, 0xf1, 0x1f, 0x20, 0x0, 0xc7,
    0xf, 0x30, 0x0, 0xa9, 0xd, 0x60, 0x0, 0xb7,
    0x6, 0xc0, 0x1, 0xe1, 0x0, 0x69, 0x79, 0x20,

    /* U+0038 "8" */
    0x0, 0x88, 0x8a, 0x20, 0x8, 0x80, 0x4, 0xd0,
    0xc, 0x50, 0x0, 0xf0, 0xa, 0x90, 0x3, 0xc0,
    0x2, 0xe7, 0x18, 0x10, 0x0, 0x2b, 0xe8, 0x0,
    0x3, 0x80, 0xa, 0xd0, 0xe, 0x20, 0x0, 0xd6,
    0x1f, 0x0, 0x0, 0xa7, 0xd, 0x40, 0x0, 0xd2,
    0x2, 0xa8, 0x79, 0x40,

    /* U+004B "K" */
    0x18, 0xdd, 0x81, 0x18, 0xd9, 0x40, 0xa, 0xa0,
    0x0, 0x46, 0x0, 0x0, 0xaa, 0x0, 0x28, 0x0,
    0x0, 0xa, 0xa0, 0x9, 0x0, 0x0, 0x0, 0xaa,
    0xb, 0x60, 0x0, 0x0, 0xa, 0xa7, 0x9e, 0x0,
    0x0, 0x0, 0xae, 0x40, 0xd7, 0x0, 0x0, 0xa,
    0xa0, 0x5, 0xf1, 0x0, 0x0, 0xaa, 0x0, 0xc,
    0x90, 0x0, 0xa, 0xa0, 0x0, 0x3f, 0x20, 0x18,
    0xdd, 0x81, 0x8, 0xed, 0x70,

    /* U+4FE1 "信" */
    0x0, 0x0, 0x61, 0x0, 0x52, 0x0, 0x0, 0x0,
    0x0, 0x5e, 0x0, 0x1, 0xe2, 0x0, 0x0, 0x0,
    0xa, 0x70, 0x0, 0x7, 0x40, 0x18, 0x0, 0x0,
    0xe1, 0x77, 0x77, 0x77, 0x77, 0x83, 0x0, 0x6d,
    0x0, 0x0, 0x0, 0x0, 0x60, 0x0, 0xd, 0xc0,
    0x7, 0x77, 0x77, 0x7a, 0x40, 0x5, 0x7c, 0x0,
    0x0, 0x0, 0x0, 0x50, 0x0, 0x62, 0xc0, 0x7,
    0x77, 0x77, 0x7a, 0x40, 0x0, 0x2c, 0x0, 0x10,
    0x0, 0x0, 0x20, 0x0, 0x2, 0xc0, 0xd, 0x77,
    0x77, 0x7f, 0x50, 0x0, 0x2c, 0x0, 0xe1, 0x0,
    0x0, 0xd1, 0x0, 0x2, 0xc0, 0xe, 0x10, 0x0,
    0xd, 0x10, 0x0, 0x2c, 0x0, 0xe7, 0x77, 0x77,
    0xe1, 0x0, 0x2, 0xb0, 0xd, 0x0, 0x0, 0xc,
    0x10, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0,

    /* U+5355 "单" */
    0x0, 0x1, 0x10, 0x0, 0x0, 0x71, 0x0, 0x0,
    0x0, 0xa, 0x70, 0x0, 0x6c, 0x10, 0x0, 0x0,
    0x0, 0x1f, 0x20, 0xb, 0x10, 0x0, 0x0, 0x2,
    0xa7, 0xc7, 0x7a, 0x87, 0xc6, 0x0, 0x0, 0x2c,
    0x0, 0xe, 0x10, 0xa, 0x50, 0x0, 0x2, 0xc0,
    0x0, 0xe1, 0x0, 0xa4, 0x0, 0x0, 0x2e, 0x66,
    0x6f, 0x76, 0x6d, 0x40, 0x0, 0x2, 0xc0, 0x0,
    0xe1, 0x0, 0xa4, 0x0, 0x0, 0x2e, 0x66, 0x6f,
    0x76, 0x6d, 0x40, 0x0, 0x1, 0x60, 0x0, 0xe1,
    0x0, 0x42, 0x10, 0x16, 0x66, 0x66, 0x6f, 0x76,
    0x66, 0xbe, 0x20, 0x0, 0x0, 0x0, 0xe1, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0xe, 0x10, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0xd0, 0x0, 0x0,
    0x0,

    /* U+53CC "双" */
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1,
    0x77, 0x77, 0x9a, 0x57, 0x77, 0x79, 0xb0, 0x0,
    0x0, 0x8, 0x90, 0x60, 0x0, 0x8a, 0x0, 0x0,
    0x0, 0xb6, 0x6, 0x0, 0xb, 0x50, 0x1, 0x60,
    0xe, 0x30, 0x42, 0x0, 0xf1, 0x0, 0x2, 0x81,
    0xf0, 0x2, 0x50, 0x3d, 0x0, 0x0, 0x4, 0xca,
    0x0, 0x9, 0xa, 0x70, 0x0, 0x0, 0xc, 0x80,
    0x0, 0x83, 0xe1, 0x0, 0x0, 0x1, 0xee, 0x10,
    0x3, 0xe8, 0x0, 0x0, 0x0, 0x87, 0x98, 0x0,
    0x1f, 0x30, 0x0, 0x0, 0xc, 0x3, 0xe0, 0xc,
    0x9c, 0x0, 0x0, 0x9, 0x40, 0xb, 0x9, 0x50,
    0xaa, 0x0, 0x6, 0x50, 0x0, 0x8, 0x40, 0x1,
    0xdc, 0x12, 0x40, 0x0, 0x16, 0x0, 0x0, 0x1,
    0xb3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0,

    /* U+53D1 "发" */
    0x0, 0x0, 0x0, 0x74, 0x0, 0x0, 0x0, 0x0,
    0x2, 0x81, 0xe, 0x30, 0x5a, 0x10, 0x0, 0x0,
    0x7a, 0x1, 0xf0, 0x0, 0x8b, 0x0, 0x0, 0xc,
    0x40, 0x3c, 0x0, 0x0, 0x61, 0x10, 0x5, 0xf7,
    0x7a, 0xc7, 0x77, 0x77, 0xbc, 0x0, 0x2, 0x0,
    0xb4, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1f,
    0x77, 0x77, 0x7c, 0x10, 0x0, 0x0, 0x6, 0xc0,
    0x0, 0x8, 0x90, 0x0, 0x0, 0x0, 0xc3, 0x50,
    0x1, 0xe1, 0x0, 0x0, 0x0, 0x64, 0x8, 0x20,
    0xb7, 0x0, 0x0, 0x0, 0x2a, 0x0, 0xb, 0xa9,
    0x0, 0x0, 0x0, 0x9, 0x0, 0x1, 0xac, 0xc4,
    0x0, 0x0, 0x17, 0x0, 0x6, 0xa2, 0x4, 0xdd,
    0x83, 0x1, 0x3, 0x66, 0x20, 0x0, 0x0, 0x3a,
    0xa1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0,

    /* U+53F7 "号" */
    0x0, 0x1, 0x0, 0x0, 0x0, 0x3, 0x0, 0x0,
    0x0, 0xe7, 0x77, 0x77, 0x78, 0xf2, 0x0, 0x0,
    0xe, 0x10, 0x0, 0x0, 0x2d, 0x0, 0x0, 0x0,
    0xe1, 0x0, 0x0, 0x2, 0xd0, 0x0, 0x0, 0xe,
    0x77, 0x77, 0x77, 0x8d, 0x0, 0x0, 0x0, 0xb0,
    0x0, 0x0, 0x1, 0x70, 0x10, 0x26, 0x66, 0x66,
    0x66, 0x66, 0x66, 0xae, 0x20, 0x0, 0xe, 0x20,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x5, 0xb0, 0x0,
    0x0, 0x2, 0x0, 0x0, 0x0, 0xc9, 0x66, 0x66,
    0x66, 0xd6, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0xe, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4,
    0xc0, 0x0, 0x0, 0x0, 0x0, 0x14, 0x33, 0xc6,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x7, 0xd8, 0x0,
    0x0,

    /* U+5668 "器" */
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x10, 0x0,
    0xd, 0x77, 0x8d, 0xd, 0x77, 0x7e, 0x50, 0x0,
    0xe0, 0x2, 0xc0, 0xe0, 0x0, 0xd1, 0x0, 0xe,
    0x0, 0x2c, 0xe, 0x0, 0xd, 0x10, 0x0, 0xe6,
    0x68, 0xc0, 0xe7, 0x66, 0xe1, 0x0, 0x8, 0x0,
    0xb, 0x62, 0xb5, 0x3, 0x20, 0x26, 0x66, 0x69,
    0xf7, 0x68, 0xc6, 0x9f, 0x30, 0x0, 0x7, 0xc1,
    0x1, 0x82, 0x0, 0x0, 0x0, 0x9, 0x80, 0x0,
    0x1, 0x9b, 0x62, 0x0, 0x6d, 0xa7, 0x7c, 0x2a,
    0x77, 0xaf, 0xf7, 0x20, 0xb2, 0x1, 0xd1, 0xd0,
    0x2, 0xd0, 0x0, 0xb, 0x20, 0x1c, 0x1d, 0x0,
    0x2c, 0x0, 0x0, 0xb8, 0x78, 0xc1, 0xe7, 0x78,
    0xc0, 0x0, 0xb, 0x20, 0x1b, 0x1c, 0x0, 0x2b,
    0x0,

    /* U+751F "生" */
    0x0, 0x0, 0x0, 0x7, 0x20, 0x0, 0x0, 0x0,
    0x0, 0x5c, 0x10, 0xf0, 0x0, 0x0, 0x0, 0x0,
    0xb, 0x80, 0xf, 0x0, 0x0, 0x0, 0x0, 0x1,
    0xf1, 0x0, 0xf0, 0x0, 0x39, 0x0, 0x0, 0x7a,
    0x66, 0x7f, 0x66, 0x67, 0x72, 0x0, 0xc, 0x0,
    0x0, 0xf0, 0x0, 0x0, 0x0, 0x7, 0x30, 0x0,
    0xf, 0x0, 0x0, 0x0, 0x2, 0x30, 0x0, 0x0,
    0xf0, 0x0, 0x61, 0x0, 0x0, 0x46, 0x66, 0x7f,
    0x66, 0x7b, 0x80, 0x0, 0x0, 0x0, 0x0, 0xf0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xf, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xf0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0xf, 0x0, 0x0,
    0x57, 0x2, 0x66, 0x66, 0x66, 0x86, 0x66, 0x67,
    0x81,

    /* U+97F3 "音" */
    0x0, 0x0, 0x0, 0x33, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0xd, 0x30, 0x2, 0x0, 0x4, 0x77,
    0x77, 0x79, 0x88, 0x7c, 0xa0, 0x0, 0x0, 0x74,
    0x0, 0xd, 0x60, 0x0, 0x0, 0x0, 0x1e, 0x0,
    0x3c, 0x0, 0x0, 0x0, 0x0, 0x8, 0x0, 0x81,
    0x0, 0xb2, 0x26, 0x66, 0x66, 0x66, 0x66, 0x66,
    0x64, 0x0, 0x4, 0x0, 0x0, 0x0, 0x51, 0x0,
    0x0, 0xd, 0x86, 0x66, 0x66, 0xd7, 0x0, 0x0,
    0xd, 0x10, 0x0, 0x0, 0xb4, 0x0, 0x0, 0xd,
    0x86, 0x66, 0x66, 0xd4, 0x0, 0x0, 0xd, 0x10,
    0x0, 0x0, 0xb4, 0x0, 0x0, 0xd, 0x86, 0x66,
    0x66, 0xd4, 0x0, 0x0, 0xc, 0x10, 0x0, 0x0,
    0xb3, 0x0,

    /* U+9891 "频" */
    0x0, 0x0, 0x20, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0xe1, 0x2, 0x33, 0x33, 0x3c, 0x40,
    0x0, 0xe1, 0xe0, 0x83, 0x33, 0xf3, 0x33, 0x20,
    0x0, 0xd0, 0xe7, 0x73, 0x2, 0x80, 0x0, 0x0,
    0x0, 0xd0, 0xe0, 0x20, 0xd7, 0x76, 0x7e, 0x10,
    0x27, 0xb7, 0xc7, 0xb8, 0xe0, 0x74, 0x1d, 0x0,
    0x0, 0x0, 0xa1, 0x0, 0xe0, 0xc2, 0x1d, 0x0,
    0x0, 0xa1, 0xd0, 0x55, 0xe0, 0xc2, 0x1d, 0x0,
    0x4, 0xa0, 0xd0, 0xe3, 0xe0, 0xd1, 0x1d, 0x0,
    0xa, 0x20, 0xd4, 0xb0, 0xe0, 0xe0, 0x1d, 0x0,
    0x34, 0x0, 0x9d, 0x30, 0xe3, 0xa0, 0x19, 0x0,
    0x0, 0x0, 0x87, 0x0, 0x1a, 0x47, 0x70, 0x0,
    0x0, 0x9, 0x70, 0x0, 0x77, 0x0, 0xab, 0x0,
    0x4, 0x83, 0x0, 0x37, 0x30, 0x0, 0xe, 0x20,
    0x1, 0x0, 0x0, 0x10, 0x0, 0x0, 0x0, 0x0,

    /* U+FF1A "：" */
    0x0, 0x7, 0xf1, 0x38, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x7, 0xf1, 0x38, 0x0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 222, .box_w = 14, .box_h = 12, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 84, .adv_w = 139, .box_w = 9, .box_h = 9, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 125, .adv_w = 83, .box_w = 5, .box_h = 1, .ofs_x = 0, .ofs_y = 4},
    {.bitmap_index = 128, .adv_w = 113, .box_w = 6, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 164, .adv_w = 134, .box_w = 8, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 212, .adv_w = 134, .box_w = 8, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 256, .adv_w = 175, .box_w = 11, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 317, .adv_w = 240, .box_w = 15, .box_h = 15, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 430, .adv_w = 240, .box_w = 15, .box_h = 14, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 535, .adv_w = 240, .box_w = 15, .box_h = 15, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 648, .adv_w = 240, .box_w = 15, .box_h = 15, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 761, .adv_w = 240, .box_w = 15, .box_h = 14, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 866, .adv_w = 240, .box_w = 15, .box_h = 14, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 971, .adv_w = 240, .box_w = 15, .box_h = 14, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1076, .adv_w = 240, .box_w = 14, .box_h = 14, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1174, .adv_w = 240, .box_w = 16, .box_h = 15, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1294, .adv_w = 240, .box_w = 3, .box_h = 9, .ofs_x = 2, .ofs_y = 0}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint16_t unicode_list_0[] = {
    0x0, 0x6, 0x8, 0xc, 0x11, 0x13, 0x26, 0x4fbc,
    0x5330, 0x53a7, 0x53ac, 0x53d2, 0x5643, 0x74fa, 0x97ce, 0x986c,
    0xfef5
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 37, .range_length = 65270, .glyph_id_start = 1,
        .unicode_list = unicode_list_0, .glyph_id_ofs_list = NULL, .list_length = 17, .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY
    }
};

/*-----------------
 *    KERNING
 *----------------*/


/*Map glyph_ids to kern left classes*/
static const uint8_t kern_left_class_mapping[] =
{
    0, 0, 0, 0, 0, 0, 0, 1,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0
};

/*Map glyph_ids to kern right classes*/
static const uint8_t kern_right_class_mapping[] =
{
    0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0
};

/*Kern values between classes*/
static const int8_t kern_class_values[] =
{
    -8
};


/*Collect the kern class' data in one place*/
static const lv_font_fmt_txt_kern_classes_t kern_classes =
{
    .class_pair_values   = kern_class_values,
    .left_class_mapping  = kern_left_class_mapping,
    .right_class_mapping = kern_right_class_mapping,
    .left_class_cnt      = 1,
    .right_class_cnt     = 1,
};

/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LVGL_VERSION_MAJOR == 8
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
#endif

#if LVGL_VERSION_MAJOR >= 8
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = &kern_classes,
    .kern_scale = 16,
    .cmap_num = 1,
    .bpp = 4,
    .kern_classes = 1,
    .bitmap_format = 0,
#if LVGL_VERSION_MAJOR == 8
    .cache = &cache
#endif
};



/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t lv_customer_font_SourceHanSerifSC_Regular_15 = {
#else
lv_font_t lv_customer_font_SourceHanSerifSC_Regular_15 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 15,          /*The maximum line height required by the font*/
    .base_line = 2.25,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -1,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = NULL,
#endif
};



#endif /*#if LV_CUSTOMER_FONT_SOURCEHANSERIFSC_REGULAR_15*/
