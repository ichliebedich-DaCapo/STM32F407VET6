//
// Created by fairy on 2025/1/9 13:31.
//
#include <project_config.h>
#ifdef GUI_ENABLE
#include "GUI.hpp"
#endif
#ifdef FREERTOS_ENABLE
#include "cmsis_os2.h"
#endif
// 头文件
#include "adc.h"
#include "key.hpp"
#include "timer.h"
#include "RNG.h"
#include "lcd.h"
#ifdef  GUI_ENABLE
#include "ui.hpp"
#endif
#include "touch.h"
#include "delay.h"
#include "key_adapter.hpp"
#include "app.hpp"
#include "sd_spi.h"
#include "RCC.h"
#include "debug.h"
#include "spi.h"

import async_delay;
#include <cstdio>

using AsyncDelay_HAL = AsyncDelay<HAL_GetTick>;
AsyncDelay_HAL async_delay(500);


const uint16_t touch_press_reg[2]={FT_TP1_REG,FT_TP2_REG};
static uint8_t touch_isOK=6;

stru_pos pos;
uint8_t touch_state;
uint8_t touch_pos_buf1[4];
uint8_t touch_pos_buf2[4];

uint8_t  write_dat=0x55;
uint8_t  write_addr=0x8D;

uint8_t test_id;
uint8_t FT_ID=0x03;

uint8_t point_number=0;
SD_Error SD_init_Status=SD_DATA_INIT;


// 函数

// 数组
const uint16_t color[120 * 120]={};
void app_init()
{
    adc1_temperature_sensor_init();
    RNG_Init();
    ITM_Init();
    SD_init_Status=spi_sd_init();
}

void key_handler()
{
    switch (PlatformKey::getCode())
    {

        case keyK0:
            if (PlatformKey ::handle_state(KEY_STATE_NONE))
            {

                ft6336_RdReg(FT_REG_NUM_FINGER, &point_number, 1);
                ft6336_RdReg(touch_press_reg[0], touch_pos_buf1, 4);
                //竖屏
//                gui::interface::set_button1_value(((uint16_t)(touch_pos_buf1[0]&0X0F)<<8)+touch_pos_buf1[1]);
//                gui::interface::set_button2_value(((uint16_t)(touch_pos_buf1[2]&0X0F)<<8)+touch_pos_buf1[3]);
                //横屏
//                gui::interface::set_x1_value(480 - (((uint16_t) (touch_pos_buf1[2] & 0X0F) << 8) + touch_pos_buf1[3]));
//                gui::interface::set_y1_value(((uint16_t) (touch_pos_buf1[0] & 0X0F) << 8) + touch_pos_buf1[1]);

                ft6336_RdReg(touch_press_reg[1], touch_pos_buf2, 4);
                //竖屏
//                gui::interface::set_button1_value(((uint16_t)(touch_pos_buf1[0]&0X0F)<<8)+touch_pos_buf1[1]);
//                gui::interface::set_button2_value(((uint16_t)(touch_pos_buf1[2]&0X0F)<<8)+touch_pos_buf1[3]);
                //横屏
//                gui::interface::set_x2_value(480 - (((uint16_t) (touch_pos_buf2[2] & 0X0F) << 8) + touch_pos_buf2[3]));
//                gui::interface::set_y2_value(((uint16_t) (touch_pos_buf2[0] & 0X0F) << 8) + touch_pos_buf2[1]);

//                touch_state=usr_ScanTouchProcess(&touch_pos);

            }
            break;


        case keyK1:
            if (PlatformKey ::handle_state(KEY_STATE_NONE))
            {
//                LCD_Clear(0xffff);
//                for(uint16_t i=10;i<200;i++) gui::interface::set_button1_value(i);

//                ft6336_RdReg((FT_ID<<1)|0x01,&test_id, 1);
//                FT_ID++;

//                ft6336_RdReg(0xA8,&test_id, 1);
//                __BKPT(0);
//                gui::interface::set_counter_value(10);


//                write_addr++;


            }
            break;
        case keyK2:
            LCD_Clear(0xFF36); // 填充颜色 0xFF36
            lcd_flush(20,20,100,100,color);
//            for(uint16_t i=10;i<200;i++) gui::interface::set_button2_value(i);
            break;

        case keyK3:
//            LCD_Clear(0x4242); // 填充颜色 0x4242
//            ft6336_WeReg(write_addr,&write_dat,1);
//            ft6336_RdReg(write_addr,&test_id, 1);
//            write_addr++;
//            break;

        case keyK4:
            LCD_Clear(0x1234); // 填充颜色 0x1234
            break;

        case keyK5:
            LCD_Clear(0x5678); // 填充颜色 0x5678
            break;

        case keyK6:
            LCD_Clear(0x9ABC); // 填充颜色 0x9ABC
            break;

        case keyK7:

            LCD_Clear(0xdf10); // 填充颜色 0xDEF0
            break;

        case keyK8:
            LCD_Clear(0x304F); // 填充颜色 0x304F
            break;

        case keyK9:
            LCD_Clear(0x1A2B); // 填充颜色 0x1A2B
            break;

        case keyKA:
            LCD_Clear(0x3C4D); // 填充颜色 0x3C4D
            break;

        case keyKB:
            LCD_Clear(0x5E6F); // 填充颜色 0x5E6F
            break;

        case keyKC:
            LCD_Clear(0x7A8B); // 填充颜色 0x7A8B
            break;

        case keyKD:
            LCD_Clear(0x9CAD); // 填充颜色 0x9CAD
            break;

        case keyKE:
            LCD_Clear(0xBCDE); // 填充颜色 0xBCDE
            break;

        case keyKF:
            LCD_Clear(0xFEDC); // 填充颜色 0xFEDC
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

/***********测试两指代码*************/
//    ft6336_RdReg(FT_REG_NUM_FINGER, &point_number, 1);
//
//    ft6336_RdReg(touch_press_reg[0], touch_pos_buf1, 4);
//    //横屏
//    gui::interface::set_x1_value(480 - (((uint16_t) (touch_pos_buf1[2] & 0X0F) << 8) + touch_pos_buf1[3]));
//    gui::interface::set_y1_value(((uint16_t) (touch_pos_buf1[0] & 0X0F) << 8) + touch_pos_buf1[1]);
//
//    ft6336_RdReg(touch_press_reg[1], touch_pos_buf2, 4);
//    //横屏
//    gui::interface::set_x2_value(480 - (((uint16_t) (touch_pos_buf2[2] & 0X0F) << 8) + touch_pos_buf2[3]));
//    gui::interface::set_y2_value(((uint16_t) (touch_pos_buf2[0] & 0X0F) << 8) + touch_pos_buf2[1]);
//
//    delay_us(30000);
/*************************/

//    CPU::print<get_adc1_temperature>();
}

void usr_touchInit()
{
    touch_isOK = touch_init();
}





