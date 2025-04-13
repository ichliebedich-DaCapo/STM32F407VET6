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
} WaveInfo;

// 556KHz
#define FPGA_BUFFER_REG (*((volatile unsigned short *)0x60000000))
#define FPGA_READ_REG (*((volatile unsigned short *)0x60000800))
#define FPGA_DIV_REG (*((volatile unsigned short *)0x60000802))
#define FPGA_INFO_REG (*((volatile WaveInfo *)0x60020000))
#define FPGA_REGS ((volatile uint16_t  *)0x60000000)


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


uint16_t adc_data[1024];
uint16_t div;
uint32_t period;
void key_handler()
{
    switch (PlatformKey::getCode())
    {
        case keyK0:
            // 测试速度
            for (int j = 0; j < 1024; ++j)
                adc_data[j] = FPGA_REGS[j];
            __BKPT(4);
            break;


        case keyK1:
            if (FPGA_READ_REG)
            {
                FPGA_READ_REG = 1;
                div = FPGA_DIV_REG;
                for (int j = 0; j < 1024; ++j)
                    adc_data[j] = FPGA_REGS[j];
                FPGA_READ_REG = 0;
                __BKPT(3);
            }
            else
            {
                period = FPGA_INFO_REG.period;
                __BKPT(6);
            }

            break;

        case keyK2:


            break;

        case keyK3:

            break;

        case keyK4:

            break;
        default: ;
    }
}

/**实现中断服务例程*/
// 用于采集ADC数据
void adc1_isr()
{
    // 获取ADC值
}

// float temp;
uint32_t temp_period[10];
uint16_t per_index;

void background_handler()
{
    if (async_delay.is_timeout())
    {
        // printf("%f\r\n", get_adc1_temperature());
        const uint32_t period = FPGA_INFO_REG.period;
        printf("div:%d gain:%d period:%d freq:%d read:%d\r\n",FPGA_INFO_REG.div,FPGA_INFO_REG.gain, period,
               250000000 / period,FPGA_READ_REG);

        // min = 0xFFFFFFFF;
        // max = 0;
        // temp_period[per_index] = FPGA_PERIOD;
        // per_index = (per_index + 1) % 6;
        // if (per_index == 0)
        // {
        //     printf("period:");
        //     for (const unsigned long i: temp_period)
        //     {
        //         printf("%lu ", i);
        //         if (i > max)
        //             max = i;
        //         if (i < min && i != 0)
        //             min = i;
        //     }
        //     printf("max:%lu min:%lu\r\n",max,min);
        // }
    }
}
