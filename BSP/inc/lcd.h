//
// Created by 34753 on 2024/9/22.
//

#ifndef FURINA_LCD_H
#define FURINA_LCD_H
#ifndef LCD_H_
#define LCD_H_

#include "JYZQ_Conf.h"

#ifdef USE_LCD

#include "stm32f4xx.h"

#ifdef __cplusplus
extern "C" {
#endif

/*预编译*/
#define LCD_SORTS 9481


/*LCD地址*/
#define TFT_CMD (*((volatile unsigned short *) 0x60060000)) // TFT命令寄存器片选地址
#define TFT_DATA (*((volatile unsigned short *) 0x60060002))// TFT数据寄存器片选地址
#define TFT_RST (*((volatile unsigned short *) 0x60060004)) // TFT复位寄存器地址
#define TFTLED (*((volatile unsigned short *) 0x60060008))  // TFT背光寄存器地址
#define TFT_DATA_ADDR 0x60060002
#define LCD_WRITE_CMD(Command) (TFT_CMD = Command)
#define LCD_WRITE_DATA(Data) (TFT_DATA = Data)

/**
 * 设置LCD显示窗口
 * 该函数用于配置LCD的显示区域，通过指定窗口的起始和结束坐标
 *
 * @param sx 窗口起始点的X坐标
 * @param sy 窗口起始点的Y坐标
 * @param ex 窗口结束点的X坐标
 * @param ey 窗口结束点的Y坐标
 */
static inline void LCD_Set_Window(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey)
{
    // 设置列地址范围
    LCD_WRITE_CMD(0x002A);
    LCD_WRITE_DATA(sx >> 8);
    LCD_WRITE_DATA(0x00FF & sx);
    LCD_WRITE_DATA((ex) >> 8);
    LCD_WRITE_DATA(0x00FF & (ex));

    // 设置行地址范围
    LCD_WRITE_CMD(0x002B);
    LCD_WRITE_DATA(sy >> 8);
    LCD_WRITE_DATA(0x00FF & sy);
    LCD_WRITE_DATA(ey >> 8);
    LCD_WRITE_DATA(0x00FF & ey);

    // 开始传输数据到LCD
    LCD_WRITE_CMD(0x002C);
}

/*清空画面*/
static inline void LCD_Clear(uint16_t color)
{
    LCD_Set_Window(0, 0, 479, 319);
    for (uint32_t i = 0; i < 0x25800; i++)
        LCD_WRITE_DATA(color);
}

/*填充一个区域*/
static inline void LCD_Color_Fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, const uint16_t *color)
{
    uint16_t length, width;
    length = ex - sx + 1;// 矩形区域的长度
    width = ey - sy + 1; // 矩形区域的宽度
    LCD_Set_Window(sx, sy, ex, ey);
    // 遍历矩形区域的每一行
    for (int j = 0; j < width; ++j)
    {
        for (int i = 0; i < length; ++i)
        {
            LCD_WRITE_DATA(color[i + length * j]);// 一次性写入整行的颜色数据
        }
    }
}

static inline void LCD_Color_Clean(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t color)
{
    uint16_t length, width;
    length = ex - sx + 1;// 矩形区域的长度
    width = ey - sy + 1; // 矩形区域的宽度
    LCD_Set_Window(sx, sy, ex, ey);
    // 遍历矩形区域的每一行
    int total = length * width;
    for (int j = 0; j < total; ++j)
    {
        LCD_WRITE_DATA(color);// 一次性写入整行的颜色数据
    }
}

/// 设置一个像素
static inline void LCD_Set_Pixel(uint16_t x, uint16_t y, uint16_t color)
{
    LCD_Set_Window(x, y, x, y);
    LCD_WRITE_DATA(color);
}


/*液晶初始化*/
void lcd_init(void);

/*液晶填充函数*/
void lcd_flush(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, const uint16_t * color_p);

#ifdef __cplusplus
}
#endif
#endif

#endif
#endif //FURINA_LCD_H
