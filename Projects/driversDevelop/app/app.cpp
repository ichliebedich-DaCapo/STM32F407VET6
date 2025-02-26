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

#define TEST_FPGA_REG (*((volatile unsigned short *)0x60020000))
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

const uint16_t color[120 * 120] = {};

void app_init()
{
    adc1_temperature_sensor_init();
    RNG_Init();
    ITM_Init();
    delay_Init();

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
    HAL_Delay(10);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);
    HAL_Delay(50);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);

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
            for (uint32_t i = 0; i < 100000; i++)
            {
                write_reg = Get_Random_Number() & 0xFFFF;
                TEST_FPGA_REG = write_reg;
                read_reg = TEST_FPGA_REG;
                if (write_reg != read_reg)
                {
                    arr_error_fpga[error_fpga_count++] = i;
                }

            }
            error_fpga_rate = error_fpga_count / 100000.0f;
            error_fpga_count = 0;
            __BKPT(2);
            break;


        case keyK1:
            // 测试访问速度
            pre_tick = HAL_GetTick();
            for (uint32_t i = 0; i < 1000000; ++i)
            {
                read_reg = TEST_FPGA_REG;
            }
            current_tick = HAL_GetTick();
            current_tick = current_tick - pre_tick;// 单位为ms
            __BKPT(0);
            break;

        case keyK2:

            break;

        case keyK3:
            SD_SingleBlockTest_Status = SD_SingleBlockTest();
            break;

        case keyK4:
            SD_multiBlockTest_Status = SD_MultiBlockTest();
            break;

        case keyK5:
            // 测试写入速度
            // 1798ms -> 1.798us一次
            pre_tick = HAL_GetTick();
            for (uint32_t i = 0; i < 1000000; ++i)
            {
                TEST_FPGA_REG = write_reg;
            }
            current_tick = HAL_GetTick();
            current_tick = current_tick - pre_tick;// 单位为ms
            __BKPT(1);
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
        printf("%f\r\n", get_adc1_temperature());
    }
}







