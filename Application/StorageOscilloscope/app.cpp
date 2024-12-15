//
// Created by DV on 2024/12/9.
//
/*
 * D     ------> 1：MOSI
 * Q     ------> 2：MISO
 * C     ------> 3：SCLOCK
 * S     ------> 4：CS
 */
#include <cstdio>
#include "key.hpp"
#include "w25qxx.h"
#include "timer.h"
#include "spi.h"

// 宏定义
// 读取数据基址
#define  READ_DATA_BASE (((volatile unsigned short *)0x60000000))
// 读取命令：0x0000
#define READ_COMMAND (*((volatile unsigned short *)0x60020000))
// 切换频率命令：0x0002
#define FREQ_COMMAND (*((volatile unsigned short *)0x60020002))

//全局变量
uint16_t continuous_read_times=0;//连续读取次数
//标志
bool read_flag=false;
bool continuous_read_flag=false;
bool latch_data_flag=false;
//变量
uint16_t temp_data[400];//读取到的临时数组
uint16_t test_data[400] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
    30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
    40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
    50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
    60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
    70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
    80, 81, 82, 83, 84, 85, 86, 87, 88, 89,
    90, 91, 92, 93, 94, 95, 96, 97, 98, 99,
    100, 101, 102, 103, 104, 105, 106, 107, 108, 109,
    110, 111, 112, 113, 114, 115, 116, 117, 118, 119,
    120, 121, 122, 123, 124, 125, 126, 127, 128, 129,
    130, 131, 132, 133, 134, 135, 136, 137, 138, 139,
    140, 141, 142, 143, 144, 145, 146, 147, 148, 149,
    150, 151, 152, 153, 154, 155, 156, 157, 158, 159,
    160, 161, 162, 163, 164, 165, 166, 167, 168, 169,
    170, 171, 172, 173, 174, 175, 176, 177, 178, 179,
    180, 181, 182, 183, 184, 185, 186, 187, 188, 189,
    190, 191, 192, 193, 194, 195, 196, 197, 198, 199,
    200, 201, 202, 203, 204, 205, 206, 207, 208, 209,
    210, 211, 212, 213, 214, 215, 216, 217, 218, 219,
    220, 221, 222, 223, 224, 225, 226, 227, 228, 229,
    230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
    240, 241, 242, 243, 244, 245, 246, 247, 248, 249,
    250, 251, 252, 253, 254, 255, 256, 257, 258, 259,
    260, 261, 262, 263, 264, 265, 266, 267, 268, 269,
    270, 271, 272, 273, 274, 275, 276, 277, 278, 279,
    280, 281, 282, 283, 284, 285, 286, 287, 288, 289,
    290, 291, 292, 293, 294, 295, 296, 297, 298, 299,
    300, 301, 302, 303, 304, 305, 306, 307, 308, 309,
    310, 311, 312, 313, 314, 315, 316, 317, 318, 319,
    320, 321, 322, 323, 324, 325, 326, 327, 328, 329,
    330, 331, 332, 333, 334, 335, 336, 337, 338, 339,
    340, 341, 342, 343, 344, 345, 346, 347, 348, 349,
    350, 351, 352, 353, 354, 355, 356, 357, 358, 359,
    360, 361, 362, 363, 364, 365, 366, 367, 368, 369,
    370, 371, 372, 373, 374, 375, 376, 377, 378, 379,
    380, 381, 382, 383, 384, 385, 386, 387, 388, 389,
    390, 391, 392, 393, 394, 395, 396, 397, 398, 399
};

void  app_init()

{
spi2_init();
w25qxx_init();
timer6_init(FREQ_84M_to_100);// 分频为16KHz
//w25qxx_sector_erase(0);
}

void key_handler()
{
switch (Key::getCode())
{
    case keyk0://启闭后台读取任务
        if (Key::stateHandler(KEY_STATE_NONE))
        {
            read_flag = !read_flag;
        }

        break;

    case keyk1://切换单次触发和连续触发模式
        if (Key::stateHandler(KEY_STATE_NONE))
        {
            continuous_read_flag=!continuous_read_flag;

        }
        break;

    case keyk2://启闭锁存模式
        if (Key::stateHandler(KEY_STATE_NONE))
        {
            latch_data_flag=!latch_data_flag;
        }
        break;

    case keyk3://图像左移
        // 测试1：测试写入数据
        w25qxx_buffer_write_uint16(test_data,0,400);
        __BKPT(0);
        break;
        // 测试2：测试读取数据
    case keyk4://图像右移
        w25qxx_buffer_read_uint16(temp_data,0,400);
        __BKPT(0);
        break;
        // 测试3：测试擦除扇区
    case keyk5://切换程控放大器放大倍数
        w25qxx_sector_erase(0);
        __BKPT(0);
        break;
        // 测试4：测试读取数据
    case keyk6://测试程序

        __BKPT(0);
        break;

    default:
        break;
}
}

void background_handler()
{
if (read_flag)
{
    // 发送读取数据命令
    READ_COMMAND = 0x000;// 传入参数0
    // 等待数据传输完成
    while (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4));// 假设PB4是等待引脚
    //读取数据
    for (int i = 0; i < 400; i++)
    {
//          temp_data[i] = READ_DATA_BASE[i];
        temp_data[i] = test_data[i];
    }
    if (latch_data_flag)
    {
        w25qxx_buffer_write_uint16(temp_data,continuous_read_times*400,400);//这里存储数据到flash
        continuous_read_times++;
    }
    /***************/
        //这里绘制图形
        /***************/
        if (!continuous_read_flag)
        {
            read_flag = false;
            continuous_read_times=0;
        }
    }
}




