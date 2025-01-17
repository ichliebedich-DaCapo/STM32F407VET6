//
// Created by fairy on 2024/9/22.
//

#include <string.h>
#include "lcd.h"


#include "fsmc.h"
#include "stm32f4xx_hal.h"

extern DMA_HandleTypeDef hdma_memtomem_dma2_stream6;


/*LCD地址*/
#define TFT_CMD (*((volatile unsigned short *) 0x60060000)) // TFT命令寄存器片选地址
#define TFT_DATA (*((volatile unsigned short *) 0x60060002))// TFT数据寄存器片选地址
#define TFT_RST (*((volatile unsigned short *) 0x60060004)) // TFT复位寄存器地址
#define TFTLED (*((volatile unsigned short *) 0x60060008))  // TFT背光寄存器地址
#define TFT_DATA_ADDR 0x60060002U
#define LCD_WRITE_CMD(Command) (TFT_CMD = Command)
#define LCD_WRITE_DATA(Data) (TFT_DATA = Data)



/*预编译*/
#define LCD_SORTS 9488

/********************************************************************
 * 名称 : LCD_Init9481
 * 功能 : 液晶初始化
 * 输入 : 无
 * 输出 : 无
 ***********************************************************************/
void lcd_init(void)
{
#if LCD_SORTS == 9481
    TFT_RST = 0;
    HAL_Delay(5);
    TFT_RST = 1;

    LCD_WRITE_CMD(0x0011);
    HAL_Delay(5);
    LCD_WRITE_CMD(0x00D0);
    LCD_WRITE_DATA(0x0007);
    LCD_WRITE_DATA(0x0041);
    LCD_WRITE_DATA(0x0018);

    LCD_WRITE_CMD(0x00D1);

    LCD_WRITE_DATA(0x0000);
    LCD_WRITE_DATA(0x000a);
    LCD_WRITE_DATA(0x0010);

    LCD_WRITE_CMD(0x00D2);
    LCD_WRITE_DATA(0x0001);
    LCD_WRITE_DATA(0x0011);

    LCD_WRITE_CMD(0x00C0);
    LCD_WRITE_DATA(0x0010);
    LCD_WRITE_DATA(0x003B);
    LCD_WRITE_DATA(0x0000);
    LCD_WRITE_DATA(0x0002);
    LCD_WRITE_DATA(0x0011);

    LCD_WRITE_CMD(0x00C1);
    LCD_WRITE_DATA(0x0010);
    LCD_WRITE_DATA(0x0013);
    LCD_WRITE_DATA(0x0088);

    LCD_WRITE_CMD(0x00C5);
    LCD_WRITE_DATA(0x0002);

    LCD_WRITE_CMD(0x00C8);
    LCD_WRITE_DATA(0x0000);
    LCD_WRITE_DATA(0x0037);
    LCD_WRITE_DATA(0x0025);
    LCD_WRITE_DATA(0x0006);
    LCD_WRITE_DATA(0x0004);
    LCD_WRITE_DATA(0x001e);
    LCD_WRITE_DATA(0x0026);
    LCD_WRITE_DATA(0x0042);
    LCD_WRITE_DATA(0x0077);
    LCD_WRITE_DATA(0x0044);
    LCD_WRITE_DATA(0x000f);
    LCD_WRITE_DATA(0x0012);

    LCD_WRITE_CMD(0x00F3);
    LCD_WRITE_DATA(0x0040);
    LCD_WRITE_DATA(0x000A);

    LCD_WRITE_CMD(0x00F6);
    LCD_WRITE_DATA(0x0080);

    LCD_WRITE_CMD(0x00F7);
    LCD_WRITE_DATA(0x0080);

    LCD_WRITE_CMD(0x0036); //显示行列设置
    LCD_WRITE_DATA(0x002f);//屏幕需要旋转180度 A9是倒过来的

    LCD_WRITE_CMD(0x003a);
    LCD_WRITE_DATA(0x0055);

    LCD_WRITE_CMD(0x0020);
    //	 LCD_WRITE_CMD(0x0021);

    LCD_WRITE_CMD(0x002A);
    LCD_WRITE_DATA(0x0000);
    LCD_WRITE_DATA(0x0000);
    LCD_WRITE_DATA(0x0001);
    LCD_WRITE_DATA(0x003F);

    LCD_WRITE_CMD(0x002B);
    LCD_WRITE_DATA(0x0000);
    LCD_WRITE_DATA(0x0000);
    LCD_WRITE_DATA(0x0001);
    LCD_WRITE_DATA(0x00df);

    LCD_WRITE_CMD(0x00C1);
    LCD_WRITE_DATA(0x0000);
    LCD_WRITE_DATA(0x0010);
    LCD_WRITE_DATA(0x0022);

    HAL_Delay(5);
    LCD_WRITE_CMD(0x0029);
    HAL_Delay(5);
    LCD_WRITE_CMD(0x002C);
    TFTLED = 0x01;
    HAL_Delay(20);
//    LCD_Clear(BLACK);
#endif

#if LCD_SORTS == 9488
    // 复位TFT显示屏
    TFT_RST = 1;  // 将TFT复位引脚设为高电平
    HAL_Delay(5);// 等待100毫秒
    TFT_RST = 0;  // 将TFT复位引脚设为低电平
    HAL_Delay(5);// 等待100毫秒
    TFT_RST = 1;  // 将TFT复位引脚设为高电平，完成复位操作
    HAL_Delay(5);// 等待100毫秒

    // 向LCD发送命令退出睡眠模式
    LCD_WRITE_CMD(0x11);// 退出睡眠模式
    HAL_Delay(5);      // 等待100毫秒，确保命令执行完毕

    // 设置LCD电源控制
    LCD_WRITE_CMD(0xd0); // 电源控制命令
    LCD_WRITE_DATA(0x07);// 设置参数
    LCD_WRITE_DATA(0x47);// 设置参数
    LCD_WRITE_DATA(0x19);// 设置参数

    LCD_WRITE_CMD(0xd1); // 电源控制命令
    LCD_WRITE_DATA(0x00);// 设置参数
    LCD_WRITE_DATA(0x36);// 设置参数
    LCD_WRITE_DATA(0x1f);// 设置参数

    LCD_WRITE_CMD(0xd2); // 电源控制命令
    LCD_WRITE_DATA(0x01);// 设置参数
    LCD_WRITE_DATA(0x11);// 设置参数

    // 其他LCD驱动相关设置
    LCD_WRITE_CMD(0xE4); // 驱动模式设置
    LCD_WRITE_DATA(0xa0);// 设置参数

    LCD_WRITE_CMD(0xf3); // 帧速率控制
    LCD_WRITE_DATA(0x00);// 设置参数
    LCD_WRITE_DATA(0x2a);// 设置参数

    LCD_WRITE_CMD(0xc0); // MV偏压控制
    LCD_WRITE_DATA(0x10);// 设置参数
    LCD_WRITE_DATA(0x3b);// 设置参数
    LCD_WRITE_DATA(0x00);// 设置参数
    LCD_WRITE_DATA(0x02);// 设置参数
    LCD_WRITE_DATA(0x11);// 设置参数

    LCD_WRITE_CMD(0xc5); // VCOM控制
    LCD_WRITE_DATA(0x03);// 设置参数

    LCD_WRITE_CMD(0xc8); // Gamma设置
    LCD_WRITE_DATA(0x00);// 设置Gamma参数
    LCD_WRITE_DATA(0x35);// 设置Gamma参数
    LCD_WRITE_DATA(0x23);// 设置Gamma参数
    LCD_WRITE_DATA(0x07);// 设置Gamma参数
    LCD_WRITE_DATA(0x00);// 设置Gamma参数
    LCD_WRITE_DATA(0x04);// 设置Gamma参数
    LCD_WRITE_DATA(0x45);// 设置Gamma参数
    LCD_WRITE_DATA(0x53);// 设置Gamma参数
    LCD_WRITE_DATA(0x77);// 设置Gamma参数
    LCD_WRITE_DATA(0x70);// 设置Gamma参数
    LCD_WRITE_DATA(0x00);// 设置Gamma参数
    LCD_WRITE_DATA(0x04);// 设置Gamma参数

    /**************************设置LCD显示区域****************************/
    LCD_WRITE_CMD(0x0036); // 显示行列设置
    LCD_WRITE_DATA(0x00E9);// 设置参数1110 1001
    /*前两位为调整方向*/
    //默认方向为A9，左下角与右上角 10
    // 29为右下角与左上角  00
    //E9为左上角，右下角  11
    // 69   01

    LCD_WRITE_CMD(0x003a); // RGB信号格式设置
    LCD_WRITE_DATA(0x0055);// 设置参数

    LCD_WRITE_CMD(0x0020);// RAM写入控制

    LCD_WRITE_CMD(0x2a); // 水平地址设置
    LCD_WRITE_DATA(0x00);// 设置参数
    LCD_WRITE_DATA(0x00);// 设置参数
    LCD_WRITE_DATA(0x01);// 设置参数
    LCD_WRITE_DATA(0xDF);// 设置参数

    LCD_WRITE_CMD(0x2b); // 垂直地址设置
    LCD_WRITE_DATA(0x00);// 设置参数
    LCD_WRITE_DATA(0x00);// 设置参数
    LCD_WRITE_DATA(0x01);// 设置参数
    LCD_WRITE_DATA(0x3F);// 设置参数

    HAL_Delay(5);      // 等待100毫秒
    LCD_WRITE_CMD(0x29);// 唤醒命令
    LCD_WRITE_CMD(0x2c);// 写入RAM命令
    HAL_Delay(5);      // 等待100毫秒
    TFTLED = 0x01;      // 背光寄存器初始化

    /*我觉得没必要清屏函数*/
    HAL_Delay(20);

//     LCD_Clear(0xFFFF);  // 清除屏幕，设置为白色
#endif
}

/**********************************绘制接口*********************************************/
/**
 * 设置LCD显示窗口
 * 该函数用于配置LCD的显示区域，通过指定窗口的起始和结束坐标
 *
 * @param sx 窗口起始点的X坐标
 * @param sy 窗口起始点的Y坐标
 * @param ex 窗口结束点的X坐标
 * @param ey 窗口结束点的Y坐标
 */
void LCD_Set_Window(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey)
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


void LCD_Clear(uint16_t color)
{
    LCD_Set_Window(0, 0, 479, 319);
    for (uint32_t i = 0; i < 0x25800; i++)
        LCD_WRITE_DATA(color);
}

/**
 * @brief 填充矩形区域
 * @param sx
 * @param sy
 * @param ex
 * @param ey
 * @param color
 * @note
 */
void LCD_Color_Fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, const uint16_t *color)
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

/**
 * @brief 清除矩形区域
 * @param sx
 * @param sy
 * @param ex
 * @param ey
 * @param color
 */
void LCD_Color_Clean(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t color)
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
void LCD_Set_Pixel(uint16_t x, uint16_t y, uint16_t color)
{
    LCD_Set_Window(x, y, x, y);
    LCD_WRITE_DATA(color);
}

void lcd_flush(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, const uint16_t *color_p)
{
#ifdef USE_FSMC_DMA
    LCD_Set_Window(x1, y1, x2, y2);//设置LCD屏幕的扫描区域
    HAL_DMA_Start_IT(&hdma_memtomem_dma2_stream6, (uint32_t) color_p, TFT_DATA_ADDR,
                     ((x2 + 1) - x1) * ((y2 + 1) - y1));
#else
    LCD_Color_Fill(area->x1, area->y1, area->x2, area->y2, (const uint16_t *)color_p);
#endif
}

