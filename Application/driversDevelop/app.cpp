//
// Created by fairy on 2025/1/9 13:31.
//
#include "app.hpp"

#ifdef FreeRTOS_ENABLE
#include "cmsis_os2.h"
#endif
// 头文件
#include "adc.h"
#include "key.hpp"
#include "timer.h"
#include "RNG.h"
#include "lcd.h"
#include "ui.hpp"
#include "touch.h"
#ifndef GUI_DISABLE
#include "GUI.hpp"
#endif

#include "RCC.h"
#include "debug.h"

import async_delay;
#include <cstdio>

using AsyncDelay_HAL = AsyncDelay<HAL_GetTick>;
AsyncDelay_HAL async_delay(500);


const uint16_t touch_press_reg[2]={FT_TP1_REG,FT_TP2_REG};
static uint8_t touch_isOK=6;
stru_pos touch;

uint8_t  write_dat=0x55;
uint8_t  write_addr=0x8D;
uint8_t write_state;

uint8_t test_id;
uint8_t FT_ID=0x03;

uint8_t point_number=0;
// 函数

// 数组
const uint16_t color[120 * 120]={};
void app_init()
{
    adc1_temperature_sensor_init();
    RNG_Init();
    ITM_Init();
    usr_touchInit();
}

void key_handler()
{
    switch (Key::getCode())
    {

        case keyk0:
            if (Key::stateHandler(KEY_STATE_NONE))
            {
//                LCD_Clear(0x36ff);
//                for(uint16_t i=10;i<200;i++)LCD_Set_Pixel(i,20,0x0000);
//                for(uint16_t i=10;i<200;i++) UI::set_counter_value(i);
//                for(uint16_t i=20;i<200;i++)LCD_Set_Pixel(i,100,0x0000);
//                for(uint16_t i=30;i<100;i++)LCD_Set_Pixel(i,200,0x0000);
//                for(uint16_t i=40;i<50;i++)LCD_Set_Pixel(i,250,0x0000);
//                while (1)
//                {
//                    usr_ScanTouchProcess(&touch);
////                    HAL_Delay(10);
//                }

//                write_state = ft6336_WeReg(write_addr, &write_dat, 1);//I2C通信故障
                ft6336_RdReg(FT_REG_NUM_FINGER,&point_number,1);

            }
            break;


        case keyk1:
            if (Key::stateHandler(KEY_STATE_NONE))
            {
//                LCD_Clear(0xffff);
//                for(uint16_t i=10;i<200;i++) UI::set_button1_value(i);

//                ft6336_RdReg((FT_ID<<1)|0x01,&test_id, 1);
//                FT_ID++;
                ft6336_RdReg(0x85,&test_id, 1);
                write_addr++;

            }
            break;
        case keyk2:
            LCD_Clear(0xFF36); // 填充颜色 0xFF36
            lcd_flush(20,20,100,100,color);
//            for(uint16_t i=10;i<200;i++) UI::set_button2_value(i);
            break;

        case keyk3:
            LCD_Clear(0x4242); // 填充颜色 0x4242
            break;

        case keyk4:
            LCD_Clear(0x1234); // 填充颜色 0x1234
            break;

        case keyk5:
            LCD_Clear(0x5678); // 填充颜色 0x5678
            break;

        case keyk6:
            LCD_Clear(0x9ABC); // 填充颜色 0x9ABC
            break;

        case keyk7:

            LCD_Clear(0xDEF0); // 填充颜色 0xDEF0
            break;

        case keyk8:
            LCD_Clear(0x304F); // 填充颜色 0x304F
            break;

        case keyk9:
            LCD_Clear(0x1A2B); // 填充颜色 0x1A2B
            break;

        case keykA:
            LCD_Clear(0x3C4D); // 填充颜色 0x3C4D
            break;

        case keykB:
            LCD_Clear(0x5E6F); // 填充颜色 0x5E6F
            break;

        case keykC:
            LCD_Clear(0x7A8B); // 填充颜色 0x7A8B
            break;

        case keykD:
            LCD_Clear(0x9CAD); // 填充颜色 0x9CAD
            break;

        case keykE:
            LCD_Clear(0xBCDE); // 填充颜色 0xBCDE
            break;

        case keykF:
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
uint32_t rand;

void background_handler()
{
    if (async_delay.is_timeout())
    {
        printf("%f\r\n", get_adc1_temperature());

    }
//    CPU::print<get_adc1_temperature>();
}

void usr_touchInit()
{
    touch_isOK = touch_init();
}

uint8_t usr_ScanTouchProcess( stru_pos *pPos)
{
    uint8_t buf[4];
    uint8_t i = 0;
    uint8_t set = FT_FALSE;
    uint8_t pointNub = 0;
    static uint8_t cnt = 0;

    if( touch_isOK == FT_FALSE )
        return set;

    cnt++;
    if((cnt%10)==0 || cnt<10)
    {
        // read number of touch points
        ft6336_RdReg(FT_REG_NUM_FINGER,&pointNub,1);

        pointNub= pointNub&0x0f;
        if( pointNub && (pointNub < 3) )
        {
            cnt=0;
            // read the point value
            pPos->status_bit.tpDown = 1;
            pPos->status_bit.tpPress = 1;
            pPos->status_bit.ptNum = pointNub;

            for( i=0; i < CTP_MAX_TOUCH; i++)
            {

                ft6336_RdReg( touch_press_reg[i], buf, 4 );
                if( pPos->status_bit.ptNum )
                {
                    switch(0)
                    {
                        case 0:
                            pPos->xpox[i]=((uint16_t)(buf[0]&0X0F)<<8)+buf[1];
                            pPos->ypox[i]=((uint16_t)(buf[2]&0X0F)<<8)+buf[3];
                            break;
                        case 1:
                            pPos->ypox[i]=320-(((uint16_t)(buf[0]&0X0F)<<8)+buf[1]);
                            pPos->xpox[i]=((uint16_t)(buf[2]&0X0F)<<8)+buf[3];
                            break;
                        case 2:
                            pPos->xpox[i]=480-(((uint16_t)(buf[0]&0X0F)<<8)+buf[1]);
                            pPos->ypox[i]=320-(((uint16_t)(buf[2]&0X0F)<<8)+buf[3]);
                            break;
                        case 3:
                            pPos->ypox[i] = ((uint16_t)(buf[0]&0X0F)<<8)+buf[1];
                            pPos->xpox[i] = 480-(((uint16_t)(buf[2]&0X0F)<<8)+buf[3]);
                            break;
                    }
                    UI::set_counter_value(i);
                    UI::set_button1_value(pPos->xpox[i]);
                    UI::set_button2_value(pPos->ypox[i]);
//                    printf("x[%d]:%d,y[%d]:%d\r\n",i,pPos->xpox[i],i,pPos->ypox[i]);
                }
            }

            set = FT_TRUE;
            if( pPos->xpox[0]==0 && pPos->ypox[0]==0)
            {
                pPos->status = 0;
            }
        }
    }

    if( pPos->status_bit.ptNum == 0)
    {
        if( pPos->status_bit.tpDown )
        {
            pPos->status_bit.tpDown = 0;
        }
        else
        {
            pPos->xpox[0] = 0xffff;
            pPos->ypox[0] = 0xffff;
            pPos->status = 0;
        }
    }

    if( cnt>240 )
        cnt=10;

    return set;
}




