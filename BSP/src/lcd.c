//
// Created by fairy on 2024/9/22.
//

#include <string.h>
#include "lcd.h"
#include "spi.h"
#include "fsmc.h"
#include "stm32f4xx_hal.h"

extern DMA_HandleTypeDef hdma_memtomem_dma2_stream6;
extern SPI_HandleTypeDef hspi2;
extern SPI_HandleTypeDef hspi3;
extern DMA_HandleTypeDef hdma_spi2_tx ;
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
#define LCD_INTERFACE_TYPE 1 // 0:8080接口 1:SPI接口 GUI.c中的同时修改
#define delay_ms(ms)   HAL_Delay(ms)

// 使用SPI时
#if LCD_INTERFACE_TYPE == 1
// GPIO引脚定义
#define LCD_RST_PIN GPIO_PIN_15
#define LCD_RST_PORT GPIOB
#define LCD_RS_PIN GPIO_PIN_13
#define LCD_RS_PORT GPIOB
#define LCD_RST_LOW()  HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, GPIO_PIN_RESET)
#define LCD_RST_HIGH()  HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, GPIO_PIN_SET)
#define LCD_RS_LOW()  HAL_GPIO_WritePin(LCD_RS_PORT, LCD_RS_PIN, GPIO_PIN_RESET)    // 低电平发送命令
#define LCD_RS_HIGH()  HAL_GPIO_WritePin(LCD_RS_PORT, LCD_RS_PIN, GPIO_PIN_SET)     // 高电平发送数据
#define LCD_CS_LOW()  spi2_cs_low()
#define LCD_CS_HIGH()  spi2_cs_high()

// 添加全局变量控制传输过程
static volatile uint32_t remaining_pixels = 0;
static uint8_t dma_buffer[512*2]; // 512像素的缓冲区

// 测试参数配置

#define BUF_SIZE      512  // 缓冲区大小（像素数）
uint16_t dma_test_buf[BUF_SIZE] __attribute__((aligned(4))); // 4字节对齐
// 函数
void LCD_WR_REG(uint8_t data)
{
    LCD_CS_LOW();
    LCD_RS_LOW();// 低电平发送命令
    spi2_sendByte(data);
    LCD_CS_HIGH();
}

void LCD_WR_DATA(uint8_t data)
{
    LCD_CS_LOW();
    LCD_RS_HIGH();// 高电平发送数据
    spi2_sendByte(data);
    LCD_CS_HIGH();
}


#endif


/********************************************************************
 * 名称 : LCD_Init9481
 * 功能 : 液晶初始化
 * 输入 : 无
 * 输出 : 无
 ***********************************************************************/
void lcd_init(void)
{
#if LCD_INTERFACE_TYPE == 0

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

#elif LCD_INTERFACE_TYPE == 1
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = LCD_RST_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(LCD_RST_PORT, &GPIO_InitStruct);
    LCD_RST_HIGH();

    GPIO_InitStruct.Pin = LCD_RS_PIN;
    HAL_GPIO_Init(LCD_RS_PORT, &GPIO_InitStruct);
#ifdef USE_SPI_DMA
    spi2_dma_Init();// 初始化SPI+DMA
#endif
    spi2_init(); //硬件SPI初始化

    //LCD 复位
    delay_ms(30);
    LCD_RST_LOW();
    delay_ms(100);
    LCD_RST_HIGH();
    delay_ms(50);

//*************3.5 ST7796S IPS初始化**********//
    LCD_WR_REG(0x11);

    delay_ms(120);                //Delay 120ms

    LCD_WR_REG(0x36);     // Memory Data Access Control MY,MX~~
    LCD_WR_DATA(0x48);// (1<<6|1<<3)

    LCD_WR_REG(0x3A);
    LCD_WR_DATA(0x55);   //LCD_WR_DATA(0x66);

    LCD_WR_REG(0xF0);     // Command Set Control
    LCD_WR_DATA(0xC3);

    LCD_WR_REG(0xF0);
    LCD_WR_DATA(0x96);

    LCD_WR_REG(0xB4);
    LCD_WR_DATA(0x01);

    LCD_WR_REG(0xB7);
    LCD_WR_DATA(0xC6);

    //LCD_WR_REG(0xB9);
    //LCD_WR_DATA(0x02);
    //LCD_WR_DATA(0xE0);

    LCD_WR_REG(0xC0);
    LCD_WR_DATA(0x80);
    LCD_WR_DATA(0x45);

    LCD_WR_REG(0xC1);
    LCD_WR_DATA(0x13);   //18  //00

    LCD_WR_REG(0xC2);
    LCD_WR_DATA(0xA7);

    LCD_WR_REG(0xC5);
    LCD_WR_DATA(0x0A);

    LCD_WR_REG(0xE8);
    LCD_WR_DATA(0x40);
    LCD_WR_DATA(0x8A);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x29);
    LCD_WR_DATA(0x19);
    LCD_WR_DATA(0xA5);
    LCD_WR_DATA(0x33);

    LCD_WR_REG(0xE0);
    LCD_WR_DATA(0xD0);
    LCD_WR_DATA(0x08);
    LCD_WR_DATA(0x0F);
    LCD_WR_DATA(0x06);
    LCD_WR_DATA(0x06);
    LCD_WR_DATA(0x33);
    LCD_WR_DATA(0x30);
    LCD_WR_DATA(0x33);
    LCD_WR_DATA(0x47);
    LCD_WR_DATA(0x17);
    LCD_WR_DATA(0x13);
    LCD_WR_DATA(0x13);
    LCD_WR_DATA(0x2B);
    LCD_WR_DATA(0x31);

    LCD_WR_REG(0xE1);
    LCD_WR_DATA(0xD0);
    LCD_WR_DATA(0x0A);
    LCD_WR_DATA(0x11);
    LCD_WR_DATA(0x0B);
    LCD_WR_DATA(0x09);
    LCD_WR_DATA(0x07);
    LCD_WR_DATA(0x2F);
    LCD_WR_DATA(0x33);
    LCD_WR_DATA(0x47);
    LCD_WR_DATA(0x38);
    LCD_WR_DATA(0x15);
    LCD_WR_DATA(0x16);
    LCD_WR_DATA(0x2C);
    LCD_WR_DATA(0x32);

    LCD_WR_REG(0xF0);
    LCD_WR_DATA(0x3C);

    LCD_WR_REG(0xF0);
    LCD_WR_DATA(0x69);

    delay_ms(120);
    LCD_WR_REG(0x21);
    LCD_WR_REG(0x29);
    LCD_direction(0);//下两句被封装为此句，默认显示方向为横屏 切记：切换屏幕方向，LCD_Clear函数内要交互x,y
//    LCD_WR_REG(0x36);
//    LCD_WR_DATA(0x60);
    LCD_Clear(0xffff);
#else
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
#if LCD_INTERFACE_TYPE == 0
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
#elif LCD_INTERFACE_TYPE == 1
    LCD_WR_REG(0x2A);
    LCD_WR_DATA(sx >> 8);
    LCD_WR_DATA(0x00FF & sx);
    LCD_WR_DATA(ex >> 8);
    LCD_WR_DATA(0x00FF & ex);

    LCD_WR_REG(0x2B);
    LCD_WR_DATA(sy >> 8);
    LCD_WR_DATA(0x00FF & sy);
    LCD_WR_DATA(ey >> 8);
    LCD_WR_DATA(0x00FF & ey);

    LCD_WR_REG(0x2C);//开始写入GRAM
#endif
}
/**
 * 设置LCD显示方向
 * 该函数用于设置LCD的显示方向，通过指定显示方向参数来确定LCD的显示方式
 * @param direction 显示方向参数，取值范围为0~3，分别表示0度、90度、180度、270度
 */
void LCD_direction(uint8_t direction)
{
#if LCD_INTERFACE_TYPE == 0
#elif LCD_INTERFACE_TYPE == 1
    // 初始为竖屏
    // 定义液晶屏顺时针旋转方向 	0-0度旋转，1-90度旋转，2-180度旋转，3-270度旋转
    LCD_WR_REG(0x36);
//注意：原始方向为竖屏
//    位	名称	描述
//    7	    MY	垂直方向镜像（1：镜像，0：正常）
//    6	    MX	水平方向镜像（1：镜像，0：正常）
//    5	    MV	行列交换（1：交换，0：不交换）
//    4   	ML	垂直刷新顺序（1：反向，0：正常）
//    3 	BGR	RGB/BGR 顺序（1：BGR，0：RGB）
//    2 	MH	水平刷新顺序（1：反向，0：正常）
//    1 	-	保留位
//    0 	-	保留位
    switch (direction)
    {
        case 0:
//            lcddev.width=LCD_W;
//            lcddev.height=LCD_H;

            LCD_WR_DATA((1<<5)|(1<<6)|(1<<7));// 横屏1; 0度
//            LCD_WR_DATA((1<<5)|(1<<6));// 横屏1; 0度
            break;
        case 1:

//            lcddev.width=LCD_H;
//            lcddev.height=LCD_W;
            LCD_WR_DATA((1<<6));// 竖屏1; 90度
//            LCD_WR_DATA(0);// 竖屏1; 90度


            break;
        case 2:
//            lcddev.width=LCD_W;
//            lcddev.height=LCD_H;
            LCD_WR_DATA(1<<5);// 横屏2; 180度
//            LCD_WR_DATA((1<<5)|(1<<7));// 横屏2; 180度
            break;
        case 3:
//            lcddev.width=LCD_H;
//            lcddev.height=LCD_W;
            LCD_WR_DATA(1<<7);// 竖屏2; 270度
//            LCD_WR_DATA((1<<6)|(1<<7));// 竖屏2; 270度
            break;

//        case 8:
//            LCD_WR_DATA(0);// 原始方向;
//            break;
//        case 9:
//            LCD_WR_DATA(1<<7);// 垂直方向镜像 ;
//            break;
//        case 10:
//            LCD_WR_DATA(1<<6);// 水平方向镜像;
//            break;
//        case 11:
//            LCD_WR_DATA((1<<6)|(1<<7));// 垂直方向镜像 水平方向镜像;
//            break;
//        case 12:
//            LCD_WR_DATA(1<<5);// 行列交换;
//            break;
//        case 13:
//            LCD_WR_DATA((1<<5)|(1<<7));// 行列交换 垂直方向镜像;
//            break;
//        case 14:
//            LCD_WR_DATA((1<<5)|(1<<6));// 行列交换 水平方向镜像;
//            break;
//        case 15:
//            LCD_WR_DATA((1 << 5) | (1 << 6) | (1 << 7));// 行列交换 水平方向镜像 垂直方向镜像;
//            break;
        default:
            break;
    }
#endif
}

//切记：切换屏幕方向，LCD_Clear函数内要交互x,y
void LCD_Clear(uint16_t color)
{

#if LCD_INTERFACE_TYPE == 0
    LCD_Set_Window(0, 0, 479, 319);
    for (uint32_t i = 0; i < 0x25800; ++i)
    {
        LCD_WRITE_DATA(color);
    }
#elif LCD_INTERFACE_TYPE == 1
#ifdef USE_SPI_DMA

/********************DMA轮询式多次传输*********************/

// 1. 设置局部窗口
    LCD_Set_Window(0, 0,480- 1,320- 1);
    for(int i=0; i<BUF_SIZE; i++){
        dma_test_buf[i] = color; // 直接存储16位颜色值
    }
    uint32_t total_pixels = 480 * 320;
    uint32_t transferred = 0;
    while(transferred < total_pixels) {
        LCD_CS_LOW();
        LCD_RS_HIGH(); // 进入GRAM数据模式
        uint32_t chunk = (total_pixels - transferred > BUF_SIZE)
                         ? BUF_SIZE : (total_pixels - transferred);

        // 更新缓冲区内容（可选）
        // for(int i=0; i<chunk; i++) dma_test_buf[i] = color;

        HAL_SPI_Transmit_DMA(&hspi2, (uint8_t*)dma_test_buf, chunk * 2);
        while(HAL_SPI_GetState(&hspi2) != HAL_SPI_STATE_READY);
        transferred += chunk;
        LCD_CS_HIGH();
    }
/********************SPI 无DMA*************************/
#else
    LCD_Set_Window(0, 0, 479, 319);//横屏
//    LCD_Set_Window(0, 0, 319, 479);//竖屏
    LCD_CS_LOW();
    LCD_RS_HIGH();
    for (uint32_t i = 0; i < 480 * 320; ++i)
    {
        spi2_sendByte(color >> 8);
        spi2_sendByte(color & 0xFF);

    }
    LCD_CS_HIGH();
#endif
#endif
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

#if LCD_INTERFACE_TYPE == 0
    LCD_Set_Window(x, y, x, y);
    LCD_WRITE_DATA(color);
#elif LCD_INTERFACE_TYPE == 1
    LCD_Set_Window(x, y, x, y);
    LCD_CS_LOW();
    LCD_RS_HIGH();
    spi2_sendByte(color >> 8);
    spi2_sendByte(color & 0xFF);
    LCD_CS_HIGH();
#endif

}

void lcd_flush(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, const uint16_t *color_p)
{
#if LCD_INTERFACE_TYPE == 0//使用8088
#define USE_FSMC_DMA
#ifdef USE_FSMC_DMA

    LCD_Set_Window(x1, y1, x2, y2);//设置LCD屏幕的扫描区域
    HAL_DMA_Start_IT(&hdma_memtomem_dma2_stream6, (uint32_t) color_p, TFT_DATA_ADDR,
                     ((x2 + 1) - x1) * ((y2 + 1) - y1));
#else
    LCD_Color_Fill(area->x1, area->y1, area->x2, area->y2, (const uint16_t *)color_p);
#endif
#elif LCD_INTERFACE_TYPE == 1//使用SPI

#ifdef USE_SPI_DMA

//    if (disp_flush_enabled)
//    {
//        unsigned int size = (x2 - x1 + 1) * (y2 - y1 + 1) * 2;
//        lcd_st7789_set_addr_win(x1, y1, x2, y2); // 指定填充区域
//        lcd_st7789_write_data_multiple((uint8_t *)color_p, size);
//    }


    /* 设置LCD窗口并启动DMA传输 */
//    LCD_Set_Window(x1, y1, x2, y2);
//    LCD_CS_LOW();
//    LCD_RS_HIGH();
//
//    uint32_t pixel_count = (x2 - x1 + 1) * (y2 - y1 + 1);
//
//    /* 启动SPI DMA传输 */
//    HAL_SPI_Transmit_DMA(&hspi2, (uint8_t*)color_p, pixel_count * 2);
//#else
    LCD_Set_Window(x1, y1, x2, y2); // 设置LCD屏幕的扫描区域
    LCD_CS_LOW(); // 使能LCD片选
    LCD_RS_HIGH(); // 设置为数据模式

    uint32_t pixel_count = (x2 - x1 + 1) * (y2 - y1 + 1); // 计算像素数量
    for (uint32_t i = 0; i < pixel_count; ++i)
    {
        spi2_sendByte(color_p[i] >> 8); // 发送高字节
        spi2_sendByte(color_p[i] & 0xFF); // 发送低字节
    }

    LCD_CS_HIGH(); // 禁用LCD片选
#endif
#endif
}
//需要一直发送或者接收就在回调里再调用一次接收或读取函数
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
    // 1. 校验SPI实例
    if(hspi == &hspi2)  // 确保是SPI2的传输完成
    {
        // 2. 更新剩余像素计数器
        if(remaining_pixels > 0)
        {
            // 计算本次传输量（不超过缓冲区容量）
            uint32_t transfer_size = (remaining_pixels > 512) ? 512 : remaining_pixels;

            // 3. 发起新DMA传输
            HAL_SPI_Transmit_DMA(&hspi2, dma_buffer, transfer_size * 2);  // 每个像素2字节

            // 4. 更新剩余像素计数
            remaining_pixels -= transfer_size;
        }
        else
        {
            // 5. 所有传输完成后的处理
            LCD_CS_HIGH();  // 关闭片选
        }
    }
}

