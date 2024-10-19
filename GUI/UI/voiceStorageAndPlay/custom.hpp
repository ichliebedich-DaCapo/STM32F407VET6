/*
* Copyright 2023 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#ifndef __CUSTOM_H_
#define __CUSTOM_H_




/********宏定义********/
#include "gui_guider.hpp"

#define DEBUG_PRINTF // 使用Debug打印
#ifdef DEBUG_PRINTF
#define print(x, ...) printf(x, ##__VA_ARGS__)
#else
#define print(x, ...) do { } while(0)
#endif

/********变量声明********/
enum PlaySpeed
{
    LV_PLAYSPEED_NORMAL,// 正常播放
    LV_PLAYSPEED_ACC,// 快进1.75
    LV_PLAYSPEED_SLOW,//慢放0.25
};


/*********接口********/
void custom_init(lv_ui *ui);

void lv_demo_music_resume(void);
void lv_demo_music_pause(void);



void lv_playspeed_print(enum PlaySpeed speed);// 播放速度信息打印
void lv_playspeed_set(enum PlaySpeed speed);// 播放速度设置

#endif /* EVENT_CB_H_ */
