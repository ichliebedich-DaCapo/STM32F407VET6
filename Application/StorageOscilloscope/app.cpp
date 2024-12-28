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
#include "ui.hpp"

// -----------宏定义-----------
// 读取数据基址0x0000
#define  READ_DATA_BASE (((volatile unsigned short *)0x60000000))
// 读取命令 0x2000
#define READ_COMMAND (*((volatile unsigned short *)0x60004000))
// 分数分频器的整数部分0x4000
#define FREQ_N (*((volatile unsigned short *)0x60008000))
// 分数分频器的小数部分的分子与分母0x6000
#define FREQ_M_P (*((volatile unsigned short *)0x6000C000))
// 触发阈值电平(12位)0x8000
#define TRIGGER_THRESHOLD (*((volatile unsigned short *)0x60010000))


//全局变量
uint16_t continuous_read_times = 0;//连续读取次数
//标志
bool read_flag = false;
bool continuous_read_flag = true;
bool latch_data_flag = false;
//变量
uint8_t read_wave[400];//读取到的临时数组
uint16_t j = 0;//测试用
uint8_t temp = 0;
uint16_t threshold=2000;

void app_init()
{
    spi2_init();
    w25qxx_init();
    timer6_init(FREQ_84M_to_100);// 分频为16KHz
    //w25qxx_sector_erase(0);

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);


    HAL_Delay(50);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);
    HAL_Delay(60);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);

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
                continuous_read_flag = !continuous_read_flag;

            }
            break;


        case keyk2://启闭锁存模式
            if (Key::stateHandler(KEY_STATE_NONE))
            {

                latch_data_flag = !latch_data_flag;

            }
            break;

        case keyk3://图像左移
            // 测试1：测试写入数据
//        w25qxx_buffer_write_uint16(test_data,0,400);

            UI_Interface::left_shift(read_wave);
            break;
            // 测试2：测试读取数据
        case keyk4://图像右移
//        w25qxx_buffer_read_uint16(read_wave,0,400);

            UI_Interface::right_shift(read_wave);
            break;
            // 测试3：测试擦除扇区

        case keyk5://切换程控放大器放大倍数
            w25qxx_sector_erase(0);
            __BKPT(0);
            break;
            // 测试4：测试读取数据
        case keyk8://测试程序
            read_flag = !read_flag;
            break;


        case keykB:
            // 测试读命令
            temp += 1;
            FREQ_N = temp;
            FREQ_M_P = 0;
            break;
        case keykC:
            temp--;
            FREQ_N = temp;
            FREQ_M_P = 0;
            break;

        case keykD:
            threshold -=500;
            TRIGGER_THRESHOLD = threshold ;
            break;
        case keykE:
            threshold +=500;
            TRIGGER_THRESHOLD = threshold ;
            break;

        case keykF://重置FPGA
            HAL_Delay(50);
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);
            HAL_Delay(60);
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
            break;

        default:
            break;
    }
}

//最终测试记得取消注释
void background_handler()
{
    if (read_flag)
    {
        // 发送读取数据命令
        READ_COMMAND = 0x000;// 传入参数0
        // 等待数据传输完成
        while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5));//PC5是等待引脚
        //读取数据
        for (int i = 0; i < 400; i++)
        {
            read_wave[i] = READ_DATA_BASE[i]>>4;
//            read_wave[i] = test_data[j];
//            j++;
//            if (j == 1600) {
//                j = 0; // 重置计数器
//            }
//            read_wave[i]=HAL_GetTick()&0xFF;
        }
        if (latch_data_flag)
        {
//        w25qxx_buffer_write_uint16(read_wave, continuous_read_times * 400, 400);//这里存储数据到flash
            continuous_read_times++;
        }
        //绘制波形
        UI_Interface::display(read_wave);
//        if (!continuous_read_flag)
//        {
//            read_flag = false;
//            continuous_read_times = 0;
//        }
    }
}




