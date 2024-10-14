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
#define LCD_SORTS 9488


/*LCD地址*/
#define TFT_CMD (*((volatile unsigned short *) 0x60060000)) // TFT命令寄存器片选地址
#define TFT_DATA (*((volatile unsigned short *) 0x60060002))// TFT数据寄存器片选地址
#define TFT_RST (*((volatile unsigned short *) 0x60060004)) // TFT复位寄存器地址
#define TFTLED (*((volatile unsigned short *) 0x60060008))  // TFT背光寄存器地址
#define TFT_DATA_ADDR 0x60060002
#define LCD_WRITE_CMD(Command) (TFT_CMD = Command)
#define LCD_WRITE_DATA(Data) (TFT_DATA = Data)


/*设置窗口*/
#define LCD_Set_Window(sx,sy,ex,ey) \
{\
    LCD_WRITE_CMD(0x002A);\
    LCD_WRITE_DATA(sx >> 8);\
    LCD_WRITE_DATA(0x00FF & sx);\
    LCD_WRITE_DATA((ex) >> 8);\
    LCD_WRITE_DATA(0x00FF & (ex));\
    LCD_WRITE_CMD(0x002B);\
    LCD_WRITE_DATA(sy >> 8);\
    LCD_WRITE_DATA(0x00FF & sy);\
    LCD_WRITE_DATA(ey >> 8);\
    LCD_WRITE_DATA(0x00FF & ey);\
    LCD_WRITE_CMD(0x002C);\
}


/*液晶初始化*/
void lcd_init(void);


/*旧函数库*/
/*清空画面*/
void LCD_Clear(uint16_t color);
/*填充一个区域*/
void LCD_Color_Fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t *color);



#ifdef __cplusplus
}
#endif
#endif

#endif
#endif //FURINA_LCD_H
