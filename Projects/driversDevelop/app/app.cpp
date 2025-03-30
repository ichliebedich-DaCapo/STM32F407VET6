//
// Created by fairy on 2025/1/9 13:31.
//
#include <project_config.h>

#ifdef GUI_ENABLE

#include "GUI.hpp"
#include "ui.hpp"

#endif
#ifdef FREERTOS_ENABLE
#include "cmsis_os2.h"
#endif
// 头文件
#include "adc.h"
#include "key.hpp"
#include "timer.h"
#include "rng.h"
#include "lcd.h"
#include "touch.h"
#include "delay.h"
#include "key_adapter.hpp"
#include "app.hpp"
#include "sd_spi.h"
#include "rcc.h"
#include "debug.h"
#include "spi.h"
#include "fatfs.h"
#include "stm32f4xx_hal.h"
import async_delay;
#include <cstdio>

using AsyncDelay_HAL = AsyncDelay<HAL_GetTick>;
AsyncDelay_HAL async_delay(500);

typedef struct
{
    uint16_t div;
    uint16_t gain;
    uint32_t period;
}WaveInfo;
#define FPGA_INFO_REG (*((volatile WaveInfo *)0x60020000))
#define FPGA_BUFFER_REG (*((volatile unsigned short *)0x60000000))
#define FPGA_READ_REG (*((volatile unsigned short *)0x60008000))

volatile static uint16_t read_reg;
volatile static uint16_t write_reg;
volatile static uint32_t pre_tick;
volatile static uint32_t current_tick;
uint8_t arr_error_fpga[1000];
uint16_t error_fpga_count = 0;
float error_fpga_rate = 0;

// 变量
SD_Error SD_init_Status = SD_DATA_INIT;
DSTATUS disk_init_Status;
uint32_t SD_SingleBlockTest_Status = 168;
uint32_t SD_multiBlockTest_Status = 168;
// 函数

// 数组

uint16_t color[120 * 120] = {};

void app_init()
{
    adc1_temperature_sensor_init();
    RNG_Init();
    ITM_Init();
    delay_Init();

    GPIO_InitTypeDef GPIO_InitStruct = {};
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
    HAL_Delay(10);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET);
    HAL_Delay(50);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);

#ifdef SD_SPI_ENABLE
    //
        disk_init_Status=fatfs_init(0);
#endif

}

void key_handler()
{
    switch (PlatformKey::getCode())
    {

        case keyK0:
            // 测试错误率
            break;


        case keyK1:
            // 测试访问速度

            break;

        case keyK2:

            break;

        case keyK3:

            break;

        case keyK4:
            break;

        case keyK5:
            // 测试写入速度
            break;

        case keyK6:
            break;

        case keyK7:
            break;

        case keyK8:
            break;

        case keyK9:
            break;

        case keyKA:
            break;

        case keyKB:
            break;

        case keyKC:
            break;

        case keyKD:
            break;

        case keyKE:
            break;

        case keyKF:
            break;

        default:
            break;

    }
}
/**实现中断服务例程*/
// 用于采集ADC数据
void adc1_isr()
{
    // 获取ADC值
}

float temp;

void background_handler()
{
    if (async_delay.is_timeout())
    {
        // printf("%f\r\n", get_adc1_temperature());
        const uint32_t period = FPGA_INFO_REG.period;
        printf("div:%d gain:%d period:%d freq:%d read:%d\r\n",FPGA_INFO_REG.div,FPGA_INFO_REG.gain,period,200000000/period,FPGA_READ_REG);
    }
}







