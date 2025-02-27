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
#include "key_adapter.hpp"
#include "w25qxx.h"
#include "timer.h"
#include "spi.h"
#include "ui.hpp"
import Flags;

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


// 定义不同的采样频率
#define SAMPLE_RATE_12_5M 0
#define SAMPLE_RATE_4_2M 2
#define SAMPLE_RATE_1M 11
#define SAMPLE_RATE_500K 24
#define SAMPLE_RATE_100K 124
#define SAMPLE_RATE_10K 1249
#define SAMPLE_RATE_2K 6249
#define SAMPLE_RATE_1K 12499
#define SAMPLE_RATE_500HZ 24999
#define SAMPLE_RATE_200HZ 62499

// 使用别名
namespace  ui_main =gui::widgets::main;
using namespace ui_main;


/*********类型声明**********/
enum class OSC_Flags : uint8_t
{
    READ = 1 << 0,// bit:0 读取数据 0表示不读，1表示读取
    OSC_TRIGGER_MODE = 1 << 1,// bit:1 触发模式 0表示连续触发，1表示单次触发
    LATCH_MODE = 1 << 2,// bit:2 锁存数据 0表示不锁存，1表示锁存
    READ_FLASH = 1 << 3,// bit:3 读取Flash 0表示不读，1表示读取Flash
};

/*********取别名**********/
using OSCFlags = Flags<OSC_Flags>;

/*********全局变量**********/
uint16_t continuous_read_times = 0;//连续读取次数

uint8_t read_wave[400];//读取到的临时数组
uint16_t j = 0;//测试用
uint8_t temp = 0;
uint8_t magnification = 1;

uint16_t threshold = 2000;
uint8_t sample_rate = 0;

void set_sample_rate(uint8_t &rate)
{
    switch (rate)
    {
        case 0:
            FREQ_N = SAMPLE_RATE_12_5M;
            break;
        case 1:
            FREQ_N = SAMPLE_RATE_4_2M;
            break;
        case 2:
            FREQ_N = SAMPLE_RATE_1M;
            break;
        case 3:
            FREQ_N = SAMPLE_RATE_500K;
            break;
        case 4:
            FREQ_N = SAMPLE_RATE_100K;
            break;
        case 5:
            FREQ_N = SAMPLE_RATE_10K;
            break;
        case 6:
            FREQ_N = SAMPLE_RATE_2K;
            break;
        case 7:
            FREQ_N = SAMPLE_RATE_1K;
            break;
        case 8:
            FREQ_N = SAMPLE_RATE_500HZ;
            break;
        case 9:
            FREQ_N = SAMPLE_RATE_200HZ;
        default:
            break;

    }
}

// 程控放大
static void set_k1_high()
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
}

static void set_k1_low()
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
}

static void set_k2_high()
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET);
}

static void set_k2_low()
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);
}

void set_magnification(auto magnification)
{
    switch (magnification)
    {
        case 1:
            set_k1_low();
            set_k2_low();
            break;
        case 2:
            set_k1_high();
            set_k2_low();
            break;
        case 3:
            set_k1_high();
            set_k2_high();
            break;
        default:
            set_k1_low();
            set_k2_low();
            break;
    }
}

// 找出峰峰值
uint8_t vpp_count = 0;
uint8_t vpp_flag = 0;
int vpp, vpp_max, vpp_min;

void find_peak_max_min()
{
    if (vpp_flag)
    {
        vpp_flag = 0;
        auto max = (float) vpp_max * 2.0f / 4095;
        auto min = (float) vpp_min * 2.0f / 4095;
        auto temp_vpp = (float) ((max - min));
        max += 1.5f;
        min += 1.5f;
        // 显示最值
        switch (magnification)
        {
//            case 1:
//                temp_vpp *=0.1;
//                max *=0.1;
//                min *=0.1;
//                break;
//            temp_vpp *= 1.1889;
//            max *= 1.1889;
//            min *= 1.1889;
            break;
            case 2:
//                temp_vpp *= 0.0855;
//                max *= 0.0855;
//                min *= 0.0855;
                temp_vpp *= 0.1;
                max *= 0.1;
                min *= 0.1;
                break;
            case 3:
//                temp_vpp *= 0.009;
//                max *= 0.009;
//                min *= 0.009;
                temp_vpp *= 0.01;
                max *= 0.01;
                min *= 0.01;
                break;
            default:
                break;
        }
        gui::interface::print_vpp_max_min(temp_vpp, max, min);
    }
}

void app_init()
{
    spi2_init();
    w25qxx_init();
    timer6_init(FREQ_84M_to_100);// 分频为16KHz
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    //w25qxx_sector_erase(0);

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);

    GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


    HAL_Delay(50);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);
    HAL_Delay(60);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);

}


void key_handler()
{
    switch (PlatformKey ::getCode())
    {
        case keyK0://启闭后台读取任务
            if (PlatformKey ::handle_state(KEY_STATE_NONE))
            {
                OSCFlags ::toggle_flag(OSC_Flags::READ);
            }

            break;

        case keyK1://切换单次触发和连续触发模式
            if (PlatformKey ::handle_state(KEY_STATE_NONE))
            {
                OSCFlags ::toggle_flag(OSC_Flags::OSC_TRIGGER_MODE);
                gui::interface::switch_trigger_mode(OSCFlags::get_flag(OSC_Flags::OSC_TRIGGER_MODE));
                trigger_mode.click();
            }
            break;


        case keyK2://启闭锁存模式
            if (PlatformKey ::handle_state(KEY_STATE_NONE))
            {
                OSCFlags ::toggle_flag(OSC_Flags::LATCH_MODE);
                gui::interface::switch_latch_mode(OSCFlags ::get_flag(OSC_Flags::LATCH_MODE));
            }
            break;

        case keyK3://图像左移
            gui::interface::left_shift(read_wave);
            break;

        case keyK4://图像右移
            gui::interface::right_shift(read_wave);
            break;


        case keyK5:


            break;
        case keyK6://切换程控放大器放大倍数
            if (magnification > 1)
                magnification--;
            set_magnification(magnification);
            gui::interface::print_magnification(magnification);
            break;
        case keyK7:
            if (magnification < 3)
                magnification++;
            set_magnification(magnification);
            gui::interface::print_magnification(magnification);
            break;

            // 测试4：测试读取数据
        case keyK8://测试程序
            OSCFlags ::toggle_flag(OSC_Flags::READ);
            break;


        case keyKB:
            // 测试读命令
            if (sample_rate < 9)
                sample_rate++;

            set_sample_rate(sample_rate);
            gui::interface::print_scan_speed(sample_rate);
            break;
        case keyKC:
            if (sample_rate > 0)
                sample_rate--;
            set_sample_rate(sample_rate);
            gui::interface::print_scan_speed(sample_rate);
            break;

        case keyKD:
            threshold -= 100;
            TRIGGER_THRESHOLD = threshold;
            break;
        case keyKE:
            threshold += 100;
            TRIGGER_THRESHOLD = threshold;
            break;

        case keyKF://重置FPGA
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
    if (OSCFlags::get_flag(OSC_Flags::READ))
    {
        // 发送读取数据命令
        READ_COMMAND = 0x000;// 传入参数0
        // 等待数据传输完成
        while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5));//PC5是等待引脚

//        if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1))
//        {
        // 读取数据
        ++vpp_count;
        if (vpp_count > 15)
        {
            vpp_count = 0;
            vpp_flag = 1;
        }
        vpp_max = 0;
        vpp_min = 4095;
        uint16_t temp_data;
        for (int i = 0; i < 400; i++)
        {
            temp_data = READ_DATA_BASE[i];
            read_wave[i] = temp_data >> 4;
            // 找到最值
            if (vpp_flag)
            {
                if (temp_data > vpp_max)
                {
                    vpp_max = temp_data;
                }
                if (temp_data < vpp_min)
                {
                    vpp_min = temp_data;
                }
            }
        }
        // 找出峰峰值、最大值、最小值
        find_peak_max_min();
        vpp_flag = 0;

        // 锁存数据
        if (OSCFlags ::get_flag(OSC_Flags::LATCH_MODE))
        {
            //这里存储数据到flash
            // w25qxx_buffer_write_uint16(read_wave, continuous_read_times * 400, 400);
            continuous_read_times++;
        }

        //绘制波形
        gui::interface::display(read_wave);


        if (OSCFlags::check_flag(OSC_Flags::OSC_TRIGGER_MODE))
        {
            continuous_read_times = 0;
        }
//        }
//        else
//        {
//            memset(read_wave, 0, sizeof(read_wave));
//            gui::interface::display(read_wave);
//        }


    }
    else if (OSCFlags ::check_flag(OSC_Flags::READ_FLASH))
    {
       // 存储数据到flash
    }
}




